#include "Scene.h"
#include "RGBObjects.h"
#include "objects2D.h"
#include "objects3D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

// ── Scene base ────────────────────────────────────────────────────────────────

// Escena mínima: solo los ejes de referencia RGB.
void Scene::init()
{
	setGL();
	gObjects.push_back(new RGBAxes(400.0));
}

// El destructor llama a destroy() (libera entidades) y resetGL() (restaura estado GL).
Scene::~Scene()
{
	destroy();
	resetGL();
}

// Libera la memoria de todas las entidades. delete llama al destructor de cada
// entidad, que a su vez libera su malla (Mesh) y su textura si la tiene.
void Scene::destroy()
{
	for (Abs_Entity* el : gObjects) delete el;
	gObjects.clear();
}

// Carga los recursos GPU (VBOs/VAOs) de todas las entidades de la escena.
// Solo se llama para la escena activa (IG1App::changeScene).
void Scene::load()
{
	for (Abs_Entity* obj : gObjects) obj->load();
}

// Descarga los recursos GPU sin destruir las entidades en CPU.
// Permite cambiar de escena sin perder el estado de las entidades.
void Scene::unload()
{
	for (Abs_Entity* obj : gObjects) obj->unload();
}

// Configura el estado OpenGL de la escena.
// Sufijo f en los literales para evitar la advertencia C4305 (double → GLfloat).
void Scene::setGL()
{
	glClearColor(0.6f, 0.7f, 0.8f, 1.0f); // fondo azul claro, totalmente opaco
	glEnable(GL_DEPTH_TEST);               // activar test de profundidad (z-buffer)
}

// Restaura el estado OpenGL al destruir la escena.
void Scene::resetGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDisable(GL_DEPTH_TEST);
}

// Sube la cámara a la GPU y renderiza todas las entidades.
void Scene::render(Camera const& cam) const
{
	cam.upload(); // sube viewport, view matrix y projection matrix
	for (Abs_Entity* el : gObjects)
		el->render(cam.viewMat());
}

// Avanza la animación de todas las entidades (se llama cada frame).
void Scene::update()
{
	for (Abs_Entity* obj : gObjects)
		obj->update();
}

// ── Scene1 — polígonos 2D ────────────────────────────────────────────────────

void Scene1::init()
{
	setGL();
	gObjects.push_back(new RGBAxes(400.0));

	// Hexágono magenta con 6 lados y radio 200
	RegularPolygon* hexagono = new RegularPolygon(6, 200.0);
	hexagono->setColor(glm::vec4(255.0f, 0.0f, 255.0f, 1.0f));
	gObjects.push_back(hexagono);

	// Círculo aproximado con 50 lados, color amarillo
	RegularPolygon* circulo = new RegularPolygon(50, 200.0);
	circulo->setColor(glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
	gObjects.push_back(circulo);
}

// ── Scene2 — objetos RGB 2D ──────────────────────────────────────────────────

void Scene2::init()
{
	setGL();
	const float r = 200.0f; // radio de referencia para posicionar objetos

	gObjects.push_back(new RGBAxes(400.0));

	// Triángulo RGB desplazado a la derecha del origen
	RGBTriangle* triangulo = new RGBTriangle(70.0, r);
	triangulo->setModelMat(translate(mat4(1.0f), vec3(r, 0.0f, 0.0f)));
	gObjects.push_back(triangulo);

	// Rectángulo RGB centrado en el origen
	gObjects.push_back(new RGBRectangle(280.0, 280.0));

	// Círculo amarillo
	RegularPolygon* circulo = new RegularPolygon(50, r);
	circulo->setColor(glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
	gObjects.push_back(circulo);
}

// ── Scene3 — objetos 3D básicos ──────────────────────────────────────────────

void Scene3::init()
{
	setGL();
	gObjects.push_back(new RGBAxes(400.0));

	// Cubo monocromático negro
	Cube* cuboNegro = new Cube(200.0);
	cuboNegro->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	gObjects.push_back(cuboNegro);

	// Cubo con caras de distintos colores RGB
	gObjects.push_back(new RGBCube(200.0));
}

// ── Scene4 — escena completa ─────────────────────────────────────────────────

void Scene4::init()
{
	setGL();

	gObjects.push_back(new RGBAxes(400.0));

	// Suelo texturado con baldosas
	gObjects.push_back(new Ground(400.0, 400.0));

	// Caja con textura exterior e interior, situada en una esquina del suelo
	BoxOutline* caja = new BoxOutline(100.0);
	caja->setModelMat(translate(mat4(1.0f), vec3(-100.0f, 50.0f, -100.0f)));
	gObjects.push_back(caja);

	// Estrella 3D texturada y animada, posicionada dentro de la caja
	Star3D* estrella = new Star3D(40.0, 8, 40.0);
	estrella->setModelMat(translate(mat4(1.0f), vec3(-100.0f, 50.0f, -100.0f)));
	gObjects.push_back(estrella);

	// Cristalera traslúcida que rodea el suelo, escala al 50% en Y
	GlassParapet* cristalera = new GlassParapet(400.0);
	cristalera->setModelMat(scale(mat4(1.0f), vec3(1.0f, 0.5f, 1.0f)));
	gObjects.push_back(cristalera);

	// Foto horizontal sobre el suelo. Elevada 1u en Y para evitar z-fighting.
	// DEBE ser el último objeto: render() la gestiona de forma especial.
	Photo* foto = new Photo(100.0, 75.0);
	mat4 fotoMat = translate(mat4(1.0f), vec3(0.0f, 1.0f, 0.0f));
	fotoMat = fotoMat * rotate(mat4(1.0f), radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
	foto->setModelMat(fotoMat);
	gObjects.push_back(foto);
}

// Renderizado en dos pasadas para la foto:
//   1ª: renderizar todos los objetos excepto la foto.
//   Captura: update() de la foto copia el framebuffer a su textura.
//   2ª: renderizar la foto con la textura ya actualizada.
// Esto garantiza que la captura ocurre cada frame aunque la animación esté pausada.
void Scene4::render(Camera const& cam) const
{
	cam.upload();

	// Renderizar todo excepto el último objeto (la foto).
	for (size_t i = 0; i + 1 < gObjects.size(); ++i)
		gObjects[i]->render(cam.viewMat());

	// Forzar que los comandos GL se ejecuten antes de leer el framebuffer.
	glFlush();

	// Capturar el framebuffer como textura de la foto y renderizarla.
	gObjects.back()->update();
	gObjects.back()->render(cam.viewMat());
}