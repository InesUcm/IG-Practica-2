#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

class Abs_Entity // abstract class
{
public:
	Abs_Entity()
	  : mModelMat(1.0)  // 4x4 identity matrix
	  , mShader(nullptr) {};
	virtual ~Abs_Entity();

	Abs_Entity(const Abs_Entity& e) = delete;            // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete; // no copy assignment

	virtual void render(const glm::mat4& modelViewMat) const = 0; // abstract method
	virtual void update() {};

	// modeling matrix
	glm::mat4 const& modelMat() const { return mModelMat; };
	void setModelMat(glm::mat4 const& aMat) { mModelMat = aMat; };

	// load or unload entity data into the GPU
	void load();
	void unload();

protected:
	Mesh* mMesh = nullptr; // the mesh
	glm::mat4 mModelMat;  // modeling matrix
	Shader* mShader; // shader

	// transfers modelViewMat to the GPU
	virtual void upload(const glm::mat4& mModelViewMat) const;
};

class EntityWithColors : public Abs_Entity
{
public:
	explicit EntityWithColors();
	void render(const glm::mat4& modelViewMat) const override;
};

class EntityWithTexture : public Abs_Entity
{
protected:
	Texture* mTexture;
	bool mModulate = false;
public:
	explicit EntityWithTexture();
	void render(const glm::mat4& modelViewMat) const override;
};

class SingleColorEntity : public Abs_Entity
{
protected: 
	glm::vec4 mColor;
public:
	explicit SingleColorEntity(glm::vec4 color = glm::vec4(1.0f));

	glm::vec4 getColor() {
		return mColor;
	};
	void setColor(const glm::vec4& color) {
		mColor = color;
	}
	void render(const glm::mat4& modelViewMat) const override;
};

class RGBAxes : public EntityWithColors
{
public:
	explicit RGBAxes(GLdouble l);
};

class RGBTriangle : public EntityWithColors {
public:
	explicit RGBTriangle(GLdouble r, GLfloat Radio);
	virtual void update();
protected:
	float mSpinAngle = 0.0f;  // rotación sobre sí mismo
	float mOrbitAngle = 0.0f; //rotación sobre la circunferencia
	float radio; //radio de la circunferencia
};

class RGBRectangle : public EntityWithColors {
public:
	explicit RGBRectangle(GLdouble w, GLdouble h);
	void render(const glm::mat4& modelViewMat) const override;
};

class RegularPolygon : public SingleColorEntity
{
public:
	explicit RegularPolygon(GLuint num, GLdouble r);
};

class Cube : public SingleColorEntity
{
public:
	explicit Cube(GLdouble length);
	void render(const glm::mat4& modelViewMat) const override;
};
class RGBCube : public EntityWithColors
{
public:
	explicit RGBCube(GLdouble length);
};

class Ground : public EntityWithTexture
{
public:
	explicit Ground(GLdouble w, GLdouble h);
};

class BoxOutline : public EntityWithTexture 
{
protected: 
	Texture* mTexture2;
public:
	explicit BoxOutline(GLdouble length);
	void render(const glm::mat4& modelViewMat) const override;
};

#endif //_H_Entities_H_
