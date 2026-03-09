#include "objects2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

RegularPolygon::RegularPolygon(GLuint num, GLdouble r)
{
	mMesh = Mesh::generateRegularPolygon(num, r, GL_LINE_LOOP);
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
	// Solo renderizamos si la textura ya fue cargada
	if (mReady) {

		glm::mat4 aMat = modelViewMat * mModelMat;
		mShader->use();
		mShader->setUniform("modulate", mModulate);

		upload(aMat);

		mTexture->bind();
		mMesh->render();
		mTexture->unbind();
	}
}