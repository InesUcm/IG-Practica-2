#include "Scene.h"
#include "RGBObjects.h"
#include "objects2D.h"
#include "objects3D.h"

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
	gObjects.push_back(new Star3D(100.0, 8, 100.0));
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
	RegularPolygon* hexagono = new RegularPolygon(6, 200.0);//hexagono
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
	triangulo->setModelMat(translate(glm::mat4(1.0f), glm::vec3(r, 0.0f, 0.0f)));//mueve el tri�ngulo
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
Scene1::~Scene1()
{
	destroy();
	resetGL();
}
Scene2::~Scene2()
{
	destroy();
	resetGL();
}
Scene3::~Scene3()
{
	destroy();
	resetGL();
}
Scene4::~Scene4()
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

// Apartado 37: escena con suelo, caja, estrella, cristalera y foto
void Scene5::init()
{
	setGL();

	// Ejes de referencia
	gObjects.push_back(new RGBAxes(400.0));

	// Suelo (400x400) con textura baldosa embaldosada 4x4
	gObjects.push_back(new Ground(400.0, 400.0));

	// Contorno de caja con dos texturas (exterior/interior), colocada en una esquina del suelo
	BoxOutline* caja = new BoxOutline(100.0);
	caja->setModelMat(glm::translate(glm::mat4(1.0f), glm::vec3(-150.0f, 50.0f, -150.0f)));
	gObjects.push_back(caja);

	// Estrella 3D dentro de la caja (ap. 29: con textura rueda.png)
	Star3D* estrella = new Star3D(40.0, 8, 40.0);
	estrella->setModelMat(glm::translate(glm::mat4(1.0f), glm::vec3(-150.0f, 50.0f, -150.0f)));
	gObjects.push_back(estrella);

	// Cristalera (GlassParapet) rodeando el suelo (ap. 32)
	GlassParapet* cristalera = new GlassParapet(400.0);
	// La cristalera se coloca a nivel del suelo
	cristalera->setModelMat(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	gObjects.push_back(cristalera);

	// Foto que captura el color buffer y lo muestra en un rectangulo sobre el suelo (ap. 35)
	Photo* foto = new Photo(100.0, 75.0);
	// Rotamos para que quede horizontal sobre el suelo y la colocamos en otra esquina
	glm::mat4 fotoMat = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 3.0f, 100.0f));
	fotoMat = fotoMat * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	foto->setModelMat(fotoMat);
	gObjects.push_back(foto);
}

void Scene5::render(Camera const& cam) const
{
	cam.upload();

	// Renderizar todos los objetos excepto la foto (último en gObjects)
	for (size_t i = 0; i + 1 < gObjects.size(); i++)
		gObjects[i]->render(cam.viewMat());

	// Capturar el buffer con lo que se acaba de renderizar
	// (esto es el update() de la Photo — lo llamamos aquí para garantizar
	//  que ocurre cada frame independientemente de si U está pulsado)
	glFlush(); // asegurar que todo está en el buffer frontal
	gObjects.back()->update();

	// Ahora renderizar la foto con la captura
	gObjects.back()->render(cam.viewMat());
}