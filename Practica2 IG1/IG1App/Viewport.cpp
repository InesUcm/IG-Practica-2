#include "Viewport.h"

// Cambia el tamaño del viewport (se llama al redimensionar la ventana).
void Viewport::setSize(GLsizei xw, GLsizei yh)
{
	xWidth = xw;
	yHeight = yh;
}

// Cambia la posición de la esquina inferior izquierda del viewport.
// Útil para split-screen o renderizado en subbventanas.
void Viewport::setPos(GLsizei xl, GLsizei yb)
{
	xLeft = xl;
	yBot = yb;
}

// Llama a glViewport para aplicar la configuración actual en la GPU.
// Define la transformación de coordenadas normalizadas (NDC) a píxeles.
void Viewport::upload() const
{
	glViewport(xLeft, yBot, xWidth, yHeight);
}