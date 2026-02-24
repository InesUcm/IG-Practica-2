#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Abs_Entity::upload(const mat4& modelViewMat) const
{
	mShader->setUniform("modelView", modelViewMat);
}

Abs_Entity::~Abs_Entity()
{
	delete mMesh;
	mMesh = nullptr;
}

void
Abs_Entity::load()
{
	mMesh->load();
}

void
Abs_Entity::unload()
{
	mMesh->unload();
}

EntityWithColors::EntityWithColors()
{
	mShader = Shader::get("vcolors");
}

SingleColorEntity::SingleColorEntity(glm::vec4 color)
{
	mShader = Shader::get("simple");
}

EntityWithTexture::EntityWithTexture()
{
	mShader = Shader::get("texture");
}


void
EntityWithColors::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		mMesh->render();
	}
}

void
SingleColorEntity::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		mShader->setUniform("color", mColor);
		upload(aMat);
		mMesh->render();
	}
}

RGBAxes::RGBAxes(GLdouble l)
{
	mMesh = Mesh::createRGBAxes(l);
}

RegularPolygon::RegularPolygon(GLuint num, GLdouble r)
{
	mMesh = Mesh::generateRegularPolygon(num, r, GL_LINE_LOOP);
}

RGBTriangle::RGBTriangle(GLdouble r, GLfloat Radio): radio(Radio)
{
	mMesh = Mesh::generateRegularPolygon(3, r, GL_TRIANGLES);
}

RGBRectangle::RGBRectangle(GLdouble w, GLdouble h)
{
	mMesh = Mesh::generateRGBRectangle(w, h);
}

Cube::Cube(GLdouble length)
{
	mMesh = Mesh::generateCube(length);
}

RGBCube::RGBCube(GLdouble length)
{
	mMesh = Mesh::generateRGBCubeTriangles(length);
}

void Cube::render(const glm::mat4& modelViewMat)const 
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat;
		mShader->use();
		mShader->setUniform("color", mColor);
		upload(aMat);

		glEnable(GL_CULL_FACE);

		//La cara front se pinta como lineas
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mMesh->render();

		//La cara back se pinta como puntos
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		mMesh->render();

		//Restaura el estado a fill
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);
	}
}
void RGBRectangle::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat;
		mShader->use();
		upload(aMat);

		glEnable(GL_CULL_FACE);

		//La cara front se pinta entera
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();

		//La cara back se pinta solo las lineas
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mMesh->render();

		//Restaura el estado a fill
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
		glDisable(GL_CULL_FACE);
	}
}

void EntityWithTexture::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glm::mat4 aMat = modelViewMat * mModelMat;
		mShader->use();
		mShader->setUniform("modulate", mModulate);
		upload(aMat);

		mTexture->bind();
		mMesh->render();
		mTexture->unbind();

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //vuelvo a renderizar con lineas
	}
}
void
RGBTriangle::update() 
{
	mModelMat = rotate(mModelMat, glm::radians(-2.0f), glm::vec3{ 0,0,1 });

	mModelMat = rotate(glm::mat4(1), glm::radians(1.0f), glm::vec3{ 0,0,1 }) * mModelMat;
}

Ground::Ground(GLdouble w, GLdouble h)
{
	GLuint rw; 
	GLuint rh;
	rw = rh = 4;

	mMesh = Mesh::generateRectangleTexCor(w, h, rw, rh);

	mTexture = new Texture();
	mTexture->load("..\\assets\\images\\baldosaC.png");

	mModelMat = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
}

BoxOutline::BoxOutline(GLdouble length) 
{
	mMesh = Mesh::generateBoxOutlineTexCor(length);
	mTexture = new Texture();
	mTexture->load("..\\assets\\images\\papelE.png");
}

