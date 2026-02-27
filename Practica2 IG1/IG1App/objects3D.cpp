#include "objects3D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;


Cube::Cube(GLdouble length)
{
	mMesh = Mesh::generateCube(length);
}

void Cube::render(const glm::mat4& modelViewMat)const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat;
		mShader->use();
		mShader->setUniform("color", mColor);
		upload(aMat);

		glEnable(GL_CULL_FACE);

		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mMesh->render();

		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		mMesh->render();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);
	}
}


BoxOutline::BoxOutline(GLdouble length)
{
	mMesh = Mesh::generateBoxOutlineTexCor(length);
	mTexture = new Texture();
	mTexture->load("..\\assets\\images\\papelE.png");
	mTexture2 = new Texture();
	mTexture2->load("..\\assets\\images\\container.jpg");
}

void BoxOutline::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		glDisable(GL_BLEND); // evita heredar blending de objetos anteriores
		mat4 aMat = modelViewMat * mModelMat;
		mShader->use();
		mShader->setUniform("modulate", mModulate);
		upload(aMat);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);
		mTexture->bind();
		mMesh->render();
		mTexture->unbind();

		glCullFace(GL_FRONT);
		mTexture2->bind();
		mMesh->render();
		mTexture2->unbind();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);
	}
}

// ------- Apartado 29: Star3D con textura -------

Star3D::Star3D(GLdouble re, GLuint np, GLdouble h)
{
	mMesh = Mesh::generateStar3DTexCor(re, np, h);
	mTexture = new Texture();
	mTexture->load("..\\assets\\images\\rueda.png");
}

void Star3D::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mShader->use();
		mShader->setUniform("modulate", mModulate);

		if (mTexture != nullptr) mTexture->bind();

		// estrella 1
		glm::mat4 aMat1 = modelViewMat * mModelMat;
		upload(aMat1);
		mMesh->render();

		// estrella 2, rotada 180 grados sobre X para quedar enfrentada
		glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1, 0, 0));
		glm::mat4 aMat2 = modelViewMat * mModelMat * rotateMat;
		upload(aMat2);
		mMesh->render();

		if (mTexture != nullptr) mTexture->unbind();
	}
}

void Star3D::update()
{
	mModelMat = glm::rotate(mModelMat, glm::radians(1.0f), glm::vec3(0, 0, 1));
	mModelMat = glm::rotate(glm::mat4(1), glm::radians(0.5f), glm::vec3(0, 1, 0)) * mModelMat;
}

// ------- Apartado 32: GlassParapet -------

GlassParapet::GlassParapet(GLdouble length)
{
	mMesh = Mesh::generateBoxOutlineTexCor(length);
	mTexture = new Texture();
	mTexture->load("..\\assets\\images\\windowC.png", 70);
}

void GlassParapet::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat;
		mShader->use();
		mShader->setUniform("modulate", mModulate);
		upload(aMat);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);

		if (mTexture != nullptr) mTexture->bind();
		mMesh->render();
		if (mTexture != nullptr) mTexture->unbind();

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
}

// ------- Apartado 35: Photo -------

Photo::Photo(GLdouble w, GLdouble h)
{
	mMesh = Mesh::generateRectangleTexCor(w, h, 1, 1);
	mTexture = new Texture();
	// La textura se actualiza en update()
}

void Photo::update()
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLsizei w = viewport[2];
	GLsizei h = viewport[3];
	mTexture->loadColorBuffer(w, h, GL_FRONT);
	mReady = true; // ya tenemos al menos un frame capturado
}

void Photo::render(const glm::mat4& modelViewMat) const
{
	// Solo renderizamos si la textura ya fue cargada (mId != 0)
	if (mMesh != nullptr && mTexture != nullptr && mReady) {
		
		glm::mat4 aMat = modelViewMat * mModelMat;
		mShader->use();
		mShader->setUniform("modulate", mModulate);
		upload(aMat);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mTexture->bind();
		mMesh->render();
		mTexture->unbind();
	}
}