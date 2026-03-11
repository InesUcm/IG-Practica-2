#ifndef _H_Camera_H_
#define _H_Camera_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Viewport.h"

class Camera
{
public:
	explicit Camera(Viewport* vp);
	~Camera() = default;

	Viewport const& viewPort() const { return *mViewPort; }
	glm::mat4 const& viewMat()  const { return mViewMat; }
	glm::mat4 const& projMat()  const { return mProjMat; }

	void set2D();
	void set3D();

	void pitch(GLfloat a);
	void yaw(GLfloat a);
	void roll(GLfloat a);

	void setSize(GLdouble xw, GLdouble yh);
	void setScale(GLdouble s);

	void upload() const;

protected:
	glm::vec3 mEye = { 0.0, 0.0, 500.0 };
	glm::vec3 mLook = { 0.0, 0.0,   0.0 }; 
	glm::vec3 mUp = { 0.0, 1.0,   0.0 };

	glm::mat4 mViewMat;
	glm::mat4 mProjMat;

	void setVM();
	void setPM();
	void uploadVM() const;
	void uploadPM() const;

	GLdouble xRight, xLeft, yTop, yBot;
	GLdouble mNearVal = 1.0; 
	GLdouble mFarVal = 10000.0;
	GLdouble mScaleFact = 1.0;

	bool bOrto = true;

	Viewport* mViewPort;
};

#endif //_H_Camera_H_