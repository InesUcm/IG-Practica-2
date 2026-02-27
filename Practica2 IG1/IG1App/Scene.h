#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Entity.h"

#include <vector>

class Scene
{
public:
	Scene() = default;
	virtual ~Scene();

	Scene(const Scene& s) = delete;
	Scene& operator=(const Scene& s) = delete;

	virtual void init();
	virtual void update();

	void render(Camera const& cam) const;

	void load();
	void unload();

protected:
	void destroy();
	void setGL();
	void resetGL();

	std::vector<Abs_Entity*> gObjects;
};

class Scene1 : public Scene {
public:
	void init() override;
	virtual ~Scene1();
};

class Scene2 : public Scene {
public:
	void init() override;
	virtual ~Scene2();
};

class Scene3 : public Scene {
public:
	void init() override;
	virtual ~Scene3();
};

class Scene4 : public Scene {
public:
	void init() override;
	virtual ~Scene4();
};

// Apartado 37: escena final con todos los elementos
class Scene5 : public Scene {
public:
	void init() override;
};

#endif //_H_Scene_H_