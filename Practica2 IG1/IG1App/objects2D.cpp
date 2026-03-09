#include "objects2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

// ── RegularPolygon ────────────────────────────────────────────────────────────

RegularPolygon::RegularPolygon(GLuint num, GLdouble r)
{
	// GL_LINE_LOOP: dibuja segmentos entre vértices consecutivos y cierra
	// automáticamente el polígono con un segmento del último al primero.
	mMesh = Mesh::generateRegularPolygon(num, r, GL_LINE_LOOP);
}

// ── Ground ────────────────────────────────────────────────────────────────────

Ground::Ground(GLdouble w, GLdouble h)
{
	const GLuint reps = 4; // la textura se repite 4×4 veces sobre el suelo
	mMesh = Mesh::generateRectangleTexCor(w, h, reps, reps);

	mTexture = new Texture();
	mTexture->load("..\\assets\\images\\baldosaC.png");

	// Sin esta rotación el rectángulo estaría en el plano XY (vertical).
	// Al rotar -90° en X queda en el plano XZ, es decir, horizontal (el suelo).
	mModelMat = rotate(mat4(1.0f), radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
}

// ── Photo ─────────────────────────────────────────────────────────────────────

Photo::Photo(GLdouble w, GLdouble h)
{
	mMesh = Mesh::generateRectangleTexCor(w, h, 1, 1);
	mTexture = new Texture(); // la textura se rellena cada frame en update()
}

// Captura el color buffer frontal (lo que acaba de renderizarse) como textura 2D.
// Se llama desde Scene4::render() justo después del glFlush(), garantizando
// que la escena ya está completamente pintada antes de la captura.
void Photo::update()
{
	// Leer las dimensiones reales del viewport para que la captura cubra toda la ventana.
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// glCopyTexImage2D copia el framebuffer directamente a la textura de la GPU,
	// más eficiente que leer a CPU y volver a subir.
	mTexture->loadColorBuffer(viewport[2], viewport[3], GL_FRONT);
	mReady = true; // a partir de aquí render() ya puede mostrar la textura
}

// No renderizar hasta que haya al menos una captura válida (mReady),
// para evitar mostrar una textura negra/vacía en el primer frame.
void Photo::render(const glm::mat4& modelViewMat) const
{
	if (!mReady) return;

	mat4 aMat = modelViewMat * mModelMat;
	mShader->use();
	mShader->setUniform("modulate", mModulate);
	upload(aMat);
	mTexture->bind();
	mMesh->render();
	mTexture->unbind();
}