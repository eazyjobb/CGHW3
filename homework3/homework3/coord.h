#pragma once
#include "std_header.h"
#include "time_system.h"

namespace coord {
	extern glm::mat4 view, projection;
	extern glm::vec3 cameraPos, cameraFront, cameraUp, cameraRight;
	extern GLfloat yaw, pitch, fov;
	const static GLfloat cameraSpeed = 5.0f;
	const static GLfloat cameraSensitivityX = 0.1f, cameraSensitivityY = 0.05f;

	int coord_init();

	void camera_rotate(GLfloat delta_yaw, GLfloat delta_pitch);
	void camera_fov(GLfloat delta_fov);
	void camera_front(GLfloat movement);
	void camera_back(GLfloat movement);
	void camera_left(GLfloat movement);
	void camera_right(GLfloat movement);
	void refresh_camera();
	void refresh_view();
}