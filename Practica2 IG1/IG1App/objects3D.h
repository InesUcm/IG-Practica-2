#pragma once
#include "Entity.h"


class Cube : public SingleColorEntity
{
public:
	explicit Cube(GLdouble length);
	void render(const glm::mat4& modelViewMat) const override;
};

class BoxOutline : public EntityWithTexture
{
protected:
	Texture* mTexture2;
public:
	explicit BoxOutline(GLdouble length);
	~BoxOutline() override { delete mTexture2; mTexture2 = nullptr; } // ARREGLO: libera mTexture2
	void render(const glm::mat4& modelViewMat) const override;
};

// Apartado 29: Star3D ahora hereda de EntityWithTexture
class Star3D : public EntityWithTexture
{
public:
	explicit Star3D(GLdouble re, GLuint np, GLdouble h);
	void render(const glm::mat4& modelViewMat) const override;
	void update() override;
};

// Apartado 32: cristalera con textura traslucida (blending)
class GlassParapet : public EntityWithTexture
{
public:
	explicit GlassParapet(GLdouble length);
	void render(const glm::mat4& modelViewMat) const override;
};

// Apartado 35: foto que captura el color buffer
class Photo : public EntityWithTexture
{
	public:
		explicit Photo(GLdouble w, GLdouble h);
		void render(const glm::mat4& modelViewMat) const override;
		void update() override;
	private:
		bool mReady = false; // true a partir del segundo update (buffer con contenido)
};