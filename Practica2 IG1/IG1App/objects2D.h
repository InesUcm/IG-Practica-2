#pragma once
#include "Entity.h"

class RegularPolygon : public SingleColorEntity
{
public:
	explicit RegularPolygon(GLuint num, GLdouble r);
};

class Ground : public EntityWithTexture
{
public:
	explicit Ground(GLdouble w, GLdouble h);
};

class Photo : public EntityWithTexture
{
public:
	explicit Photo(GLdouble w, GLdouble h);
	void render(const glm::mat4& modelViewMat) const override;
	void update() override;
private:
	bool mReady = false; // true a partir del segundo update (buffer con contenido)
};