#ifndef _H_Camera_H_
#define _H_Camera_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Viewport.h"

// Gestiona la cámara: posición en el espacio (view matrix) y
// tipo de proyección (projection matrix). Se sube a la GPU llamando upload().
class Camera
{
public:
	explicit Camera(Viewport* vp);
	~Camera() = default;

	Viewport const& viewPort() const { return *mViewPort; }
	glm::mat4 const& viewMat()  const { return mViewMat; }
	glm::mat4 const& projMat()  const { return mProjMat; }

	// Coloca la cámara en vista 2D (ortogonal, eje Z) o 3D (perspectiva isométrica).
	void set2D();
	void set3D();

	// Rotaciones de la cámara sobre sus ejes locales (en grados).
	void pitch(GLfloat a); // eje X
	void yaw(GLfloat a); // eje Y
	void roll(GLfloat a); // eje Z

	// Ajusta el tamaño del área visible (se llama al redimensionar la ventana).
	void setSize(GLdouble xw, GLdouble yh);
	// Modifica el factor de escala de la proyección (zoom).
	void setScale(GLdouble s);

	// Sube viewport, view matrix y projection matrix a la GPU.
	void upload() const;

protected:
	glm::vec3 mEye = { 0.0, 0.0, 500.0 }; // posición de la cámara
	glm::vec3 mLook = { 0.0, 0.0,   0.0 }; // punto al que apunta
	glm::vec3 mUp = { 0.0, 1.0,   0.0 }; // vector "arriba"

	glm::mat4 mViewMat; // matriz de vista (inversa del modelado de la cámara)
	glm::mat4 mProjMat; // matriz de proyección (ortogonal o perspectiva)

	void setVM();       // recalcula mViewMat con lookAt
	void setPM();       // recalcula mProjMat según bOrto
	void uploadVM() const; // sube mViewMat a todos los shaders
	void uploadPM() const; // sube mProjMat a todos los shaders

	// Límites del volumen visible en coordenadas de mundo.
	// Se declaran GLdouble para evitar la advertencia C4244 al asignar desde
	// los parámetros GLdouble del constructor y de setSize().
	GLdouble xRight, xLeft, yTop, yBot;
	GLdouble mNearVal = 1.0;     // plano cercano del frustum
	GLdouble mFarVal = 10000.0; // plano lejano del frustum
	GLdouble mScaleFact = 1.0;    // factor de escala para zoom

	bool bOrto = true; // true = proyección ortogonal, false = perspectiva

	Viewport* mViewPort; // puntero al viewport activo (no tiene propiedad)
};

#endif //_H_Camera_H_