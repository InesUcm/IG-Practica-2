#include "Shader.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

// Inicializa el área visible a partir del tamaño del viewport y calcula la PM.
Camera::Camera(Viewport* vp)
	: mViewMat(1.0)
	, mProjMat(1.0)
	, xRight(vp->width() / 2.0)
	, xLeft(-xRight)
	, yTop(vp->height() / 2.0)
	, yBot(-yTop)
	, mViewPort(vp)
{
	setPM();
}

// ── View matrix ───────────────────────────────────────────────────────────────

// Sube mViewMat como uniform "modelView" a todos los shaders activos.
void Camera::uploadVM() const { Shader::setUniform4All("modelView", mViewMat); }

// Recalcula mViewMat usando glm::lookAt(ojo, objetivo, arriba).
void Camera::setVM()
{
	mViewMat = lookAt(mEye, mLook, mUp);
}

// Vista 2D: cámara en el eje Z mirando al origen, sin rotación.
void Camera::set2D()
{
	mEye = { 0, 0, 500 };
	mLook = { 0, 0, 0 };
	mUp = { 0, 1, 0 };
	setVM();
}

// Vista 3D: cámara en posición isométrica mirando ligeramente hacia arriba.
void Camera::set3D()
{
	mEye = { 500, 500, 500 };
	mLook = { 0, 10, 0 };
	mUp = { 0, 1, 0 };
	setVM();
}

// Rotaciones: modifican mViewMat directamente (giro sobre ejes locales).
void Camera::pitch(GLfloat a) { mViewMat = rotate(mViewMat, radians(a), vec3(1.0f, 0.0f, 0.0f)); }
void Camera::yaw(GLfloat a) { mViewMat = rotate(mViewMat, radians(a), vec3(0.0f, 1.0f, 0.0f)); }
void Camera::roll(GLfloat a) { mViewMat = rotate(mViewMat, radians(a), vec3(0.0f, 0.0f, 1.0f)); }

// ── Projection matrix ─────────────────────────────────────────────────────────

// Actualiza los límites del área visible y recalcula la PM.
// Se llama al redimensionar la ventana (IG1App::resize).
void Camera::setSize(GLdouble xw, GLdouble yh)
{
	xRight = xw / 2.0;
	xLeft = -xRight;
	yTop = yh / 2.0;
	yBot = -yTop;
	setPM();
}

// Modifica el factor de escala para hacer zoom in/out y recalcula la PM.
void Camera::setScale(GLdouble s)
{
	mScaleFact -= s;
	if (mScaleFact < 0.0) mScaleFact = 0.01; // evitar escala negativa o cero
	setPM();
}

// Recalcula la projection matrix. Solo implementa ortogonal por ahora.
// glm::ortho recibe (left, right, bottom, top, near, far).
void Camera::setPM()
{
	if (bOrto) {
		mProjMat = ortho(
			xLeft * mScaleFact,
			xRight * mScaleFact,
			yBot * mScaleFact,
			yTop * mScaleFact,
			mNearVal,
			mFarVal);
	}
}

// Sube mProjMat como uniform "projection" a todos los shaders activos.
void Camera::uploadPM() const { Shader::setUniform4All("projection", mProjMat); }

// ── Upload completo ───────────────────────────────────────────────────────────

// Sube viewport + view matrix + projection matrix a la GPU de una sola vez.
// Se llama al inicio de cada render.
void Camera::upload() const
{
	mViewPort->upload();
	uploadVM();
	uploadPM();
}