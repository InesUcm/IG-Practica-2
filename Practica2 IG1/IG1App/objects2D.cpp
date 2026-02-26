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
