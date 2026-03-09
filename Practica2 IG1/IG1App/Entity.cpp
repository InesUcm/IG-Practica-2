#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

// ── Abs_Entity — clase base abstracta ────────────────────────────────────────

// Libera la malla. El shader NO se libera aquí porque no es propiedad de la
// entidad: pertenece a la caché de Shader::get() y se gestiona con freeAll().
Abs_Entity::~Abs_Entity()
{
	delete mMesh;
	mMesh = nullptr;
}

// Llama a Mesh::load(), que sube vértices, colores y coordenadas de textura
// a la GPU creando los VBOs y el VAO.
void Abs_Entity::load() { mMesh->load(); }

// Libera los recursos GPU de la malla (VBO, CBO, TCO, VAO).
// Los datos en CPU (vectores) se mantienen para poder recargar si hace falta.
void Abs_Entity::unload() { mMesh->unload(); }

// Envía la matriz modelView al uniform "modelView" del shader activo.
// Se llama justo antes de cada draw call para colocar el objeto en la escena.
void Abs_Entity::upload(const mat4& modelViewMat) const
{
	mShader->setUniform("modelView", modelViewMat);
}

// ── EntityWithColors — colores por vértice, shader "vcolors" ─────────────────

// El shader "vcolors" interpola los colores definidos en cada vértice de la malla.
EntityWithColors::EntityWithColors()
{
	mShader = Shader::get("vcolors");
}

void EntityWithColors::render(const mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // combina view con el modelado del objeto
		mShader->use();
		upload(aMat);
		mMesh->render();
	}
}

// ── SingleColorEntity — color uniforme, shader "simple" ──────────────────────

// El shader "simple" pinta todos los fragmentos con el mismo color (mColor).
// Se inicializa mColor en la lista para que setColor() funcione antes del primer render.
SingleColorEntity::SingleColorEntity(glm::vec4 color)
	: mColor(color)
{
	mShader = Shader::get("simple");
}

void SingleColorEntity::render(const mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat;
		mShader->use();
		mShader->setUniform("color", mColor); // enviar el color al fragment shader
		upload(aMat);
		mMesh->render();
	}
}

// ── RGBAxes — ejes de coordenadas X(rojo) Y(verde) Z(azul) ──────────────────

RGBAxes::RGBAxes(GLdouble l)
{
	mMesh = Mesh::createRGBAxes(l); // crea 6 vértices: origen+extremo por cada eje
}

// ── EntityWithTexture — textura 2D, shader "texture" ─────────────────────────

// El shader "texture" muestrea una textura 2D y opcionalmente la modula con el color.
EntityWithTexture::EntityWithTexture()
	: mTexture(nullptr)
{
	mShader = Shader::get("texture");
}

// Libera mTexture (la textura principal).
// Las subclases con texturas adicionales (e.g. BoxOutline::mTexture2)
// deben liberar las suyas en su propio destructor.
EntityWithTexture::~EntityWithTexture()
{
	delete mTexture;
	mTexture = nullptr;
}

// Render genérico con textura: activa GL_FILL, sube matrices, vincula textura,
// dibuja y desvincula. Las subclases suelen sobreescribir esto para
// gestionar culling, blending u otras pasadas.
void EntityWithTexture::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mat4 aMat = modelViewMat * mModelMat;
		mShader->use();
		mShader->setUniform("modulate", mModulate); // si true, multiplica textura × color
		upload(aMat);
		mTexture->bind();
		mMesh->render();
		mTexture->unbind();
	}
}