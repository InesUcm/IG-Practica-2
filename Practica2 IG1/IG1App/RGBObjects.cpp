#include "RGBObjects.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

// ── RGBTriangle ───────────────────────────────────────────────────────────────

// Radio se ignora en la construcción de la malla (solo se usa r para el polígono).
RGBTriangle::RGBTriangle(GLdouble r, GLfloat Radio)
{
	// GL_TRIANGLES: tres vértices forman un triángulo relleno con colores RGB.
	mMesh = Mesh::generateRegularPolygon(3, r, GL_TRIANGLES);
}

// Animación: giro propio sobre Z en sentido antihorario (-2°) combinado con
// giro orbital sobre Z en sentido horario (+1°) → efecto de traslación circular.
void RGBTriangle::update()
{
	mModelMat = rotate(mModelMat, radians(-2.0f), vec3{ 0, 0, 1 }); // giro propio
	mModelMat = rotate(mat4(1), radians(1.0f), vec3{ 0, 0, 1 }) * mModelMat; // giro orbital
}

// ── RGBRectangle ──────────────────────────────────────────────────────────────

RGBRectangle::RGBRectangle(GLdouble w, GLdouble h)
{
	mMesh = Mesh::generateRGBRectangle(w, h);
}

// Renderizado con culling para mostrar efectos distintos en cada cara:
//   - Cara delantera (GL_CULL_FRONT → se ve la trasera): relleno completo.
//   - Cara trasera  (GL_CULL_BACK  → se ve la delantera): solo wireframe.
void RGBRectangle::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat;
		mShader->use();
		upload(aMat);

		glEnable(GL_CULL_FACE);

		// Se descartan frontales → se ve la cara trasera en relleno.
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();

		// Se descartan traseras → se ve la cara delantera en wireframe.
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mMesh->render();

		// Restaurar estado.
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);
	}
}

// ── RGBCube ───────────────────────────────────────────────────────────────────

// Cubo formado por triángulos con colores distintos en cada cara.
RGBCube::RGBCube(GLdouble length)
{
	mMesh = Mesh::generateRGBCubeTriangles(length);
}