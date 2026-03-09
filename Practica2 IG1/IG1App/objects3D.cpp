#include "objects3D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

// ── Cube ──────────────────────────────────────────────────────────────────────

Cube::Cube(GLdouble length)
{
	mMesh = Mesh::generateCube(length);
}

// Renderizado en dos pasadas con face culling para mostrar interior y exterior:
//   1ª pasada — culling de cara trasera: se ven las caras frontales en WIREFRAME.
//   2ª pasada — culling de cara frontal: se ven las caras traseras como PUNTOS.
// Esto da el efecto de ver el cubo "por dentro y por fuera" a la vez.
void Cube::render(const glm::mat4& modelViewMat) const
{
	if (mMesh == nullptr) return;

	mat4 aMat = modelViewMat * mModelMat;
	mShader->use();
	mShader->setUniform("color", mColor);
	upload(aMat);

	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);                          // descartar traseras → se ven frontales
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    // wireframe
	mMesh->render();

	glCullFace(GL_FRONT);                         // descartar frontales → se ven traseras
	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);   // puntos
	mMesh->render();

	// Restaurar estado para no afectar objetos siguientes.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_CULL_FACE);
}

// ── BoxOutline ────────────────────────────────────────────────────────────────

// mTexture2 se inicializa a nullptr en la lista de inicialización para
// garantizar que el destructor puede comprobarlo con seguridad.
BoxOutline::BoxOutline(GLdouble length)
	: mTexture2(nullptr)
{
	mMesh = Mesh::generateBoxOutlineTexCor(length);

	mTexture = new Texture(); // textura exterior (cara visible desde fuera)
	mTexture->load("..\\assets\\images\\papelE.png");

	mTexture2 = new Texture(); // textura interior (cara visible desde dentro)
	mTexture2->load("..\\assets\\images\\container.jpg");
}

// mTexture es liberada por ~EntityWithTexture(); aquí solo liberamos mTexture2.
BoxOutline::~BoxOutline()
{
	delete mTexture2;
	mTexture2 = nullptr;
}

// Dos pasadas con culling para separar interior y exterior de la caja:
//   1ª — culling trasero desactivado (GL_FRONT): se ven caras traseras → EXTERIOR.
//   2ª — culling frontal desactivado (GL_BACK):  se ven caras frontales → INTERIOR.
// Se desactiva el blend para evitar heredar transparencia de objetos anteriores.
void BoxOutline::render(const glm::mat4& modelViewMat) const
{
	if (mMesh == nullptr) return;

	glDisable(GL_BLEND); // limpiar estado blend por si acaso
	mat4 aMat = modelViewMat * mModelMat;
	mShader->use();
	mShader->setUniform("modulate", mModulate);
	upload(aMat);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	// Exterior: se descartan las caras frontales, se ven las traseras.
	glCullFace(GL_FRONT);
	mTexture->bind();
	mMesh->render();
	mTexture->unbind();

	// Interior: se descartan las caras traseras, se ven las frontales.
	glCullFace(GL_BACK);
	mTexture2->bind();
	mMesh->render();
	mTexture2->unbind();

	glDisable(GL_CULL_FACE);
}

// ── Star3D ────────────────────────────────────────────────────────────────────

Star3D::Star3D(GLdouble re, GLuint np, GLdouble h)
{
	// Malla de estrella con coordenadas de textura. re=radio exterior, np=puntas, h=altura.
	mMesh = Mesh::generateStar3DTexCor(re, np, h);
	mTexture = new Texture();
	mTexture->load("..\\assets\\images\\rueda.png");
}

// Dos pasadas: cara frontal con la transformación actual, y cara trasera
// rotada 180° en X para que ambas caras queden enfrentadas y visibles.
void Star3D::render(const glm::mat4& modelViewMat) const
{
	if (mMesh == nullptr) return;

	mShader->use();
	mShader->setUniform("modulate", mModulate);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (mTexture != nullptr) mTexture->bind();

	// Cara frontal
	upload(modelViewMat * mModelMat);
	mMesh->render();

	// Cara trasera: rotada 180° en X para estar enfrentada a la frontal.
	mat4 flip = rotate(mat4(1.0f), radians(180.0f), vec3(1.0f, 0.0f, 0.0f));
	upload(modelViewMat * mModelMat * flip);
	mMesh->render();

	if (mTexture != nullptr) mTexture->unbind();
}

// Animación combinada: giro propio sobre Z (rueda sobre su eje) y
// giro orbital sobre Y (órbita alrededor del origen de la caja).
void Star3D::update()
{
	mModelMat = rotate(mModelMat, radians(1.0f), vec3(0.0f, 0.0f, 1.0f));
	mModelMat = rotate(mat4(1.0f), radians(0.5f), vec3(0.0f, 1.0f, 0.0f)) * mModelMat;
}

// ── GlassParapet ──────────────────────────────────────────────────────────────

GlassParapet::GlassParapet(GLdouble length)
{
	mMesh = Mesh::generateBoxOutlineTexCor(length);
	mTexture = new Texture();
	// Alpha 200/255: semitransparente para simular vidrio.
	mTexture->load("..\\assets\\images\\windowC.png", 200);
}

// Renderizado con blending alpha para transparencia:
//   - glBlendFunc(SRC_ALPHA, 1-SRC_ALPHA): mezcla el cristal con lo que hay detrás.
//   - glDepthMask(GL_FALSE): no escribir en el depth buffer para que los objetos
//     detrás del cristal sigan siendo visibles correctamente.
//   - Sin culling: se renderizan ambas caras del cristal.
void GlassParapet::render(const glm::mat4& modelViewMat) const
{
	if (mMesh == nullptr) return;

	mat4 aMat = modelViewMat * mModelMat;
	mShader->use();
	mShader->setUniform("modulate", mModulate);
	upload(aMat);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);               // no escribir en z-buffer
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_CULL_FACE);             // ambas caras visibles

	if (mTexture != nullptr) mTexture->bind();
	mMesh->render();
	if (mTexture != nullptr) mTexture->unbind();

	// Restaurar estado GL para que no afecte a objetos renderizados después.
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}