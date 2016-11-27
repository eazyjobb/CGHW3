#include "coord.h"

namespace coord {
	glm::mat4 view, projection;
	glm::vec3 cameraPos, cameraFront, cameraUp, cameraRight;
	GLfloat yaw, pitch;

	int coord_init() {
		yaw = -90.0f;
		pitch = 0.0f;

		cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		projection = glm::perspective((GLfloat)glm::radians(45.0), (GLfloat)window_width / (GLfloat)window_height, 0.1f, 100.0f);

		return 0;
	}

	void camera_rotate(GLfloat _yaw, GLfloat _pitch) {
		yaw += _yaw;
		pitch += _pitch;

		if (coord::pitch > 89.0f)
			coord::pitch = 89.0f;
		if (coord::pitch < -89.0f)
			coord::pitch = -89.0f;
		refresh_camera();
	}

	void camera_front(GLfloat movement) {
		coord::cameraPos += movement * coord::cameraFront;
		refresh_view();
	}

	void camera_back(GLfloat movement) {
		coord::cameraPos -= movement * coord::cameraFront;
		refresh_view();
	}

	void camera_left(GLfloat movement) {
		coord::cameraPos -= movement * coord::cameraRight;
		refresh_view();
	}

	void camera_right(GLfloat movement) {
		coord::cameraPos += movement * coord::cameraRight;
		refresh_view();
	}

	void refresh_view() {
		coord::view = glm::lookAt(coord::cameraPos, coord::cameraPos + coord::cameraFront, coord::cameraUp);
	}

	void refresh_camera() {
		glm::vec3 front;
		front.x = cos(glm::radians(coord::yaw)) * cos(glm::radians(coord::pitch));
		front.y = sin(glm::radians(coord::pitch));
		front.z = sin(glm::radians(coord::yaw)) * cos(glm::radians(coord::pitch));
		coord::cameraFront = glm::normalize(front);
		front.x = cos(glm::radians(coord::yaw)) * cos(glm::radians(coord::pitch + 90.0f));
		front.y = sin(glm::radians(coord::pitch + 90.0f));
		front.z = sin(glm::radians(coord::yaw)) * cos(glm::radians(coord::pitch + 90.0f));
		coord::cameraUp = glm::normalize(front);
		coord::cameraRight = glm::normalize(glm::cross(coord::cameraFront, coord::cameraUp));
		refresh_view();
	}
}
