#include "Scene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
void
Scene::init()
{
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene

	gObjects.push_back(new RGBAxes(400.0));
}
void
Scene1::init()
{
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene

	gObjects.push_back(new RGBAxes(400.0));
	RegularPolygon* hexagono = new RegularPolygon(6,200.0);//hexagono
	glm::vec4 magenta(255.0, 0.0, 255.0, 1.0);
	hexagono->setColor(magenta);
	gObjects.push_back(hexagono);

	RegularPolygon* circulo = new RegularPolygon(50, 200.0);//circulo
	glm::vec4 amarillo(255.0, 255.0, 0.0, 1.0);
	circulo->setColor(amarillo);
	gObjects.push_back(circulo);
}

void
Scene2::init()
{
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene

	float r = 200.0f;//radio de la circunferencia
	gObjects.push_back(new RGBAxes(400.0));

	RGBTriangle* triangulo = new RGBTriangle(70.0, r); // triangulo
	triangulo->setModelMat(translate(glm::mat4(1.0f), glm::vec3(r, 0.0f, 0.0f)));//mueve el triángulo
	gObjects.push_back(triangulo);

	RGBRectangle* rectangulo = new RGBRectangle(280.0, 280.0); // rectangulo
	gObjects.push_back(rectangulo);

	RegularPolygon* circulo = new RegularPolygon(50, r);//circulo
	glm::vec4 amarillo(255.0, 255.0, 0.0, 1.0);
	circulo->setColor(amarillo);
	gObjects.push_back(circulo);
}

void
Scene3::init()
{
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene
	gObjects.push_back(new RGBAxes(400.0));

	Cube* cuboNgero = new Cube(200.0); // cubo
	glm::vec4 negro(0.0, 0.0, 0.0, 1.0);
	cuboNgero->setColor(negro);
	gObjects.push_back(cuboNgero);

	RGBCube* cuboPintado = new RGBCube(200.0);
	gObjects.push_back(cuboPintado);
}

void
Scene4::init()
{
	setGL();

	gObjects.push_back(new RGBAxes(400.0));
	gObjects.push_back(new Ground(400.0, 400.0));
	gObjects.push_back(new BoxOutline(200.0));
}

Scene::~Scene()
{
	destroy();
	resetGL();
}

void
Scene::destroy()
{ // release memory and resources

	for (Abs_Entity* el : gObjects)
		delete el;

	gObjects.clear();
}

void
Scene::load()
{
	for (Abs_Entity* obj : gObjects)
		obj->load();
}

void
Scene::unload()
{
	for (Abs_Entity* obj : gObjects)
		obj->unload();
}

void
Scene::setGL()
{
	// OpenGL basic setting
	glClearColor(0.6, 0.7, 0.8, 1.0); // background color (alpha=1 -> opaque) cambiado a azul.
	glEnable(GL_DEPTH_TEST);          // enable Depth test
}
void
Scene::resetGL()
{
	glClearColor(.0, .0, .0, .0); // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);     // disable Depth test
}

void
Scene::render(Camera const& cam) const
{
	cam.upload();

	for (Abs_Entity* el : gObjects)
		el->render(cam.viewMat());
}

void 
Scene::update()
{
	for (Abs_Entity* obj : gObjects)
		obj->update();
}
