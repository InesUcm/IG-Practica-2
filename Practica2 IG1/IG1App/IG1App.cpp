#include "IG1App.h"
#include "Scene.h"
#include "Shader.h"
#include <iostream>

using namespace std;

// Instancia estática única de la aplicación (patrón Singleton).
IG1App IG1App::s_ig1app;

// Callback de depuración OpenGL: se registra con glDebugMessageCallback
// y escribe por consola cualquier error o advertencia del driver.
void GLAPIENTRY debugCallback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length,
	const GLchar* message, const void* userParam)
{
	const char* prefix = (type == GL_DEBUG_TYPE_ERROR)
		? "\x1b[31m[ERROR]\x1b[0m "
		: "\x1b[33m[WARNING]\x1b[0m ";
	cout << prefix << message << '\n';
}

// Señala a GLFW que cierre la ventana al final del frame actual.
void
IG1App::close()
{
	cout << "Closing GLFW...\n";
	glfwSetWindowShouldClose(mWindow, true);
}

// Bucle principal: inicializa si aún no se ha hecho, procesa eventos y
// redibuја cuando es necesario. Al salir del bucle llama a destroy().
void
IG1App::run()
{
	if (mWindow == 0)
		init();

	while (!glfwWindowShouldClose(mWindow)) {

		// Redibujar si algún evento lo ha marcado necesario.
		if (mNeedsRedisplay) {
			display();
			mNeedsRedisplay = false;
		}

		if (mUpdateEnabled || mUpdateFrame) {
			double currentTime = glfwGetTime();

			// Avanzar la simulación cuando llegue el momento del siguiente frame.
			if (currentTime >= mNextUpdate) {
				mScenes[mCurrentScene]->update();
				mNeedsRedisplay = true;
				mNextUpdate += FRAME_DURATION;
			}

			// Esperar solo hasta el próximo frame para no quemar CPU.
			double timeout = mNextUpdate - currentTime;
			if (timeout < 0.0) timeout = 0.0;
			glfwWaitEventsTimeout(timeout);
			mUpdateFrame = false;
		}
		else {
			// Sin animación: bloquear hasta el próximo evento de ventana.
			glfwWaitEvents();
			mNextUpdate = glfwGetTime();
		}
	}

	destroy();
}

// Crea el contexto OpenGL, el viewport, la cámara y todas las escenas.
void
IG1App::init()
{
	iniWinOpenGL();

	mViewPort = new Viewport(mWinW, mWinH);
	mCamera = new Camera(mViewPort);

	// Registrar todas las escenas. El índice en el vector es el número de escena.
	mScenes.push_back(new Scene);   // 0 — escena base (solo ejes)
	mScenes.push_back(new Scene1);  // 1 — polígonos 2D
	mScenes.push_back(new Scene2);  // 2 — objetos RGB 2D
	mScenes.push_back(new Scene3);  // 3 — objetos 3D básicos
	mScenes.push_back(new Scene4);  // 4 — escena completa

	mCamera->set2D();
	for (Scene* scene : mScenes)
		scene->init();

	// Solo la escena activa tiene sus recursos GPU cargados.
	mScenes[mCurrentScene]->load();
}

// Crea la ventana GLFW, inicializa OpenGL 3.3 Core, carga extensiones con
// GLEW y registra los callbacks de entrada y redimensionado.
void
IG1App::iniWinOpenGL()
{
	cout << "Starting GLFW...\n";
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // activa mensajes de debug

	mWindow = glfwCreateWindow(mWinW, mWinH, "IG1App", nullptr, nullptr);
	if (mWindow == nullptr) {
		glfwTerminate();
		throw logic_error("No se pudo crear la ventana GLFW.");
	}

	glfwMakeContextCurrent(mWindow);

	// GLEW carga los punteros a las funciones de extensiones OpenGL.
	if (GLenum err = glewInit(); err != GLEW_OK) {
		glfwTerminate();
		throw logic_error("Error al cargar extensiones GLEW: "s +
			reinterpret_cast<const char*>(glewGetErrorString(err)));
	}

	// Registrar callbacks de GLFW (usan funciones estáticas que delegan en el singleton).
	glfwSetWindowSizeCallback(mWindow, s_resize);
	glfwSetCharCallback(mWindow, s_key);
	glfwSetKeyCallback(mWindow, s_specialkey);
	glfwSetWindowRefreshCallback(mWindow, s_display);

	// Activar el callback de mensajes de depuración de OpenGL.
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0u, 0, GL_TRUE);
	glDebugMessageCallback(debugCallback, nullptr);

	cout << glGetString(GL_VERSION) << '\n';
	cout << glGetString(GL_VENDOR) << '\n';
}

// Libera todos los recursos en orden seguro:
//   1. Escenas (entidades, texturas, meshes GPU).
//   2. Cámara y viewport.
//   3. Shaders GPU — ANTES de glfwTerminate para que el contexto siga activo.
//   4. glfwTerminate destruye el contexto OpenGL.
void
IG1App::destroy()
{
	for (Scene* scene : mScenes)
		delete scene;
	mScenes.clear();

	delete mCamera;   mCamera = nullptr;
	delete mViewPort; mViewPort = nullptr;

	// freeAll() hace `delete shaders` y libera el mapa COMPLETAMENTE del heap,
	// evitando los falsos leaks del nodo centinela del std::map en el CRT.
	Shader::freeAll();

	glfwTerminate();
}

// Limpia el back buffer, renderiza la escena activa y hace swap de buffers.
void
IG1App::display() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mScenes[mCurrentScene]->render(*mCamera);
	glfwSwapBuffers(mWindow); // intercambia front/back buffer (double buffering)
}

// Ajusta viewport y cámara cuando se redimensiona la ventana.
void
IG1App::resize(int newWidth, int newHeight)
{
	mWinW = newWidth;
	mWinH = newHeight;
	mViewPort->setSize(newWidth, newHeight);
	mCamera->setSize(mViewPort->width(), mViewPort->height());
}

// Gestión de teclas de carácter (callback GLFW char).
void
IG1App::key(unsigned int key)
{
	bool need_redisplay = true;

	switch (key) {
	case '+': mCamera->setScale(+0.01);         break; // zoom in
	case '-': mCamera->setScale(-0.01);         break; // zoom out
	case 'l': mCamera->set3D();                 break; // vista perspectiva
	case 'o': mCamera->set2D();                 break; // vista ortogonal
	case 'u': mUpdateFrame = true;              break; // avanzar un frame
	case 'U': mUpdateEnabled = !mUpdateEnabled; break; // activar/pausar animación
	default:
		// Cambio de escena con teclas numéricas (0-9).
		if (key >= '0' && key <= '9') {
			if (changeScene(key - '0')) break;
			cout << "[NOTE] No existe la escena " << char(key) << ".\n";
		}
		need_redisplay = false;
		break;
	}

	if (need_redisplay)
		mNeedsRedisplay = true;
}

// Gestión de teclas especiales: flechas (rotar cámara) y Escape (salir).
// action == GLFW_PRESS o GLFW_REPEAT; se ignoran los eventos GLFW_RELEASE.
void
IG1App::specialkey(int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE) return;

	bool need_redisplay = true;

	switch (key) {
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(mWindow, true);
		break;
		// Flecha derecha: pitch+1; con Ctrl: pitch-1
	case GLFW_KEY_RIGHT:
		mCamera->pitch(mods == GLFW_MOD_CONTROL ? -1.0f : 1.0f);
		break;
		// Flecha izquierda: yaw-1; con Ctrl: yaw+1
	case GLFW_KEY_LEFT:
		mCamera->yaw(mods == GLFW_MOD_CONTROL ? 1.0f : -1.0f);
		break;
	case GLFW_KEY_UP:   mCamera->roll(1.0f); break; // roll positivo
	case GLFW_KEY_DOWN: mCamera->roll(-1.0f); break; // roll negativo
	default:
		need_redisplay = false;
		break;
	}

	if (need_redisplay)
		mNeedsRedisplay = true;
}

// Descarga los recursos GPU de la escena actual y carga los de la nueva.
// Devuelve false si el índice está fuera de rango.
bool
IG1App::changeScene(size_t sceneNr)
{
	if (sceneNr >= mScenes.size())
		return false;

	if (sceneNr != mCurrentScene) {
		mScenes[mCurrentScene]->unload();
		mCurrentScene = sceneNr;
		mScenes[mCurrentScene]->load();
	}

	return true;
}