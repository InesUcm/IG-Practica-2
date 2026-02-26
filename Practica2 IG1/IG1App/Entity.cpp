#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Abs_Entity::upload(const mat4& modelViewMat) const
{
	mShader->setUniform("modelView", modelViewMat);
}

Abs_Entity::~Abs_Entity()
{
	delete mMesh;
	mMesh = nullptr;
}

void
Abs_Entity::load()
{
	mMesh->load();
}

void
Abs_Entity::unload()
{
	mMesh->unload();
}

EntityWithColors::EntityWithColors()
{
	mShader = Shader::get("vcolors");
}

SingleColorEntity::SingleColorEntity(glm::vec4 color)
{
	mShader = Shader::get("simple");
}

EntityWithTexture::EntityWithTexture()
{
	mShader = Shader::get("texture");
}


void
EntityWithColors::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		mMesh->render();
	}
}

void
SingleColorEntity::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		mShader->setUniform("color", mColor);
		upload(aMat);
		mMesh->render();
	}
}

RGBAxes::RGBAxes(GLdouble l)
{
	mMesh = Mesh::createRGBAxes(l);
}

RegularPolygon::RegularPolygon(GLuint num, GLdouble r)
{
	mMesh = Mesh::generateRegularPolygon(num, r, GL_LINE_LOOP);
}

RGBTriangle::RGBTriangle(GLdouble r, GLfloat Radio)
{
	mMesh = Mesh::generateRegularPolygon(3, r, GL_TRIANGLES);
}

RGBRectangle::RGBRectangle(GLdouble w, GLdouble h)
{
	mMesh = Mesh::generateRGBRectangle(w, h);
}

Cube::Cube(GLdouble length)
{
	mMesh = Mesh::generateCube(length);
}

RGBCube::RGBCube(GLdouble length)
{
	mMesh = Mesh::generateRGBCubeTriangles(length);
}

void Cube::render(const glm::mat4& modelViewMat)const 
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat;
		mShader->use();
		mShader->setUniform("color", mColor);
		upload(aMat);

		glEnable(GL_CULL_FACE);

		//La cara front se pinta como lineas
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mMesh->render();

		//La cara back se pinta como puntos
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		mMesh->render();

		//Restaura el estado a fill
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);
	}
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

void EntityWithTexture::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glm::mat4 aMat = modelViewMat * mModelMat;
		mShader->use();
		mShader->setUniform("modulate", mModulate);
		upload(aMat);

		mTexture->bind();
		mMesh->render();
		mTexture->unbind();

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //vuelvo a renderizar con lineas
	}
}
void BoxOutline::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat;
		mShader->use();
		mShader->setUniform("modulate", mModulate);
		upload(aMat);


		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//que las caras se pinten enteras
        glEnable(GL_CULL_FACE);//activamos el culling

		glCullFace(GL_BACK);//Descartamos la cara de dentro para ver la de fuera
		mTexture->bind();//con la textura de fuera (mTexture)
		mMesh->render();
		mTexture->unbind();
		

		glCullFace(GL_FRONT);//Descartamos la cara de fuera para ver la de dentro
		mTexture2->bind();//con la textura de dentro (mTexture2)
		mMesh->render();
		mTexture2->unbind();

		//Restaura el estado a fill
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
	}
}
void 
Star3D::render(const glm::mat4& modelViewMat) const {
	if (mMesh != nullptr) {
		mShader->use();
		mShader->setUniform("color", mColor);

		//estrella 1
		glm::mat4 aMat1 = modelViewMat * mModelMat;
		upload(aMat1);
		mMesh->render();

		//estrella 2, rotada 180 grados
		glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1, 0, 0));
		glm::mat4 aMat2 = modelViewMat * mModelMat * rotateMat;
		upload(aMat2);
		mMesh->render();

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}
void
RGBTriangle::update() 
{
	mModelMat = rotate(mModelMat, glm::radians(-2.0f), glm::vec3{ 0,0,1 });

	mModelMat = rotate(glm::mat4(1), glm::radians(1.0f), glm::vec3{ 0,0,1 }) * mModelMat;
}

void 
Star3D::update()
{
	// rota sobre su eje Z
	mModelMat = glm::rotate(mModelMat, glm::radians(1.0f), glm::vec3(0, 0, 1));
	// rota sobre el eje Y
	mModelMat = glm::rotate(glm::mat4(1), glm::radians(0.5f), glm::vec3(0, 1, 0)) * mModelMat;

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

BoxOutline::BoxOutline(GLdouble length) 
{
	mMesh = Mesh::generateBoxOutlineTexCor(length);
	//cargamos la textura exterior
	mTexture = new Texture();
	mTexture->load("..\\assets\\images\\papelE.png");
	//cargamos la textura interior
	mTexture2 = new Texture();
	mTexture2->load("..\\assets\\images\\container.jpg");
}
Star3D::Star3D(GLdouble re, GLuint np, GLdouble h)
{
	mMesh = Mesh::generateStar3D(re, np, h);
}

