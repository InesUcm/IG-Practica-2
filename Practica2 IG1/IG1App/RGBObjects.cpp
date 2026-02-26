#include "RGBObjects.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

RGBTriangle::RGBTriangle(GLdouble r, GLfloat Radio)
{
	mMesh = Mesh::generateRegularPolygon(3, r, GL_TRIANGLES);
}

void
RGBTriangle::update()
{
	mModelMat = rotate(mModelMat, glm::radians(-2.0f), glm::vec3{ 0,0,1 });

	mModelMat = rotate(glm::mat4(1), glm::radians(1.0f), glm::vec3{ 0,0,1 }) * mModelMat;
}




RGBRectangle::RGBRectangle(GLdouble w, GLdouble h)
{
	mMesh = Mesh::generateRGBRectangle(w, h);
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




RGBCube::RGBCube(GLdouble length)
{
	mMesh = Mesh::generateRGBCubeTriangles(length);
}
