#pragma once
#include "std_header.h"
#include "time_system.h"

namespace coord {
	class camera {
	private:
		glm::vec3 cameraPos, cameraFront, cameraUp, cameraRight;
		glm::mat4 view, projection;
		GLfloat yaw, pitch, fov;
		void refresh_camera();
		void refresh_view();
	public:
		GLfloat cameraSpeedZ, cameraSpeedX, cameraSensitivityX, cameraSensitivityY, cameraSensitivityFov;

		camera();

		void camera_rotate(GLfloat delta_yaw, GLfloat delta_pitch);
		void camera_fov(GLfloat delta_fov);
		void camera_front(GLfloat movement);
		void camera_back(GLfloat movement);
		void camera_left(GLfloat movement);
		void camera_right(GLfloat movement);
		const glm::vec3 getCameraPos() const;
		const glm::vec3 getCameraFront() const;
		const glm::vec3 getCameraUp() const;
		const glm::vec3 getCameraRight() const;
		const glm::mat4 getView() const;
		const glm::mat4 getProjection() const;
		void setYaw(GLfloat);
		void setPitch(GLfloat);
		void setFov(GLfloat);
		void setCameraPos(glm::vec3);
	};

	extern std::unordered_map <std::string, camera> camera_list;
	extern std::unordered_map <std::string, camera>::iterator current_camera;

	int coord_init();
}
