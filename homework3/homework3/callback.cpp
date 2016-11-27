#include "callback.h"

namespace callback {
	bool keys[1024];
	bool mouse_keys[8];
	bool first_mouse;
	GLfloat mouse_last_x, mouse_last_y;
	
	int callback_init() {
		first_mouse = true;
		return 0;
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
	void mouse_button_callback(GLFWwindow* window, int key, int action, int mode) {
		if (action == GLFW_PRESS)
			mouse_keys[key] = true;
		else if (action == GLFW_RELEASE)
			mouse_keys[key] = false;
	}

	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (first_mouse)
		{
			mouse_last_x = (GLfloat)xpos;
			mouse_last_y = (GLfloat)ypos;
			first_mouse = false;
		}

		GLfloat xoffset = (GLfloat)xpos - mouse_last_x;
		GLfloat yoffset = mouse_last_y - (GLfloat)ypos;
		mouse_last_x = (GLfloat)xpos;
		mouse_last_y = (GLfloat)ypos;

		if (false == mouse_keys[GLFW_MOUSE_BUTTON_RIGHT])
			return;
		
		xoffset *= coord::cameraSensitivity;
		yoffset *= coord::cameraSensitivity;

		coord::camera_rotate(xoffset, yoffset);
	}

	void handle_events(GLFWwindow* window) {
		if (mouse_keys[GLFW_MOUSE_BUTTON_RIGHT]) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		if (keys[GLFW_KEY_ESCAPE] || keys[GLFW_KEY_Q])
			glfwSetWindowShouldClose(window, GL_TRUE);
		if (keys[GLFW_KEY_L]) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			std::cout << "glPolygonMode :: Line" << std::endl;
		}
		if (keys[GLFW_KEY_F]) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			std::cout << "glPolygonMode :: Fill" << std::endl;
		}

		if (keys[GLFW_KEY_UP]) {
			texture_tester::ratio = std::min(1.0f, texture_tester::ratio + 0.05f);
		}
		if (keys[GLFW_KEY_DOWN]) {
			texture_tester::ratio = std::max(0.0f, texture_tester::ratio - 0.05f);
		}


		if (keys[GLFW_KEY_W])
			coord::camera_front(time_system::delta_time() * coord::cameraSpeed);
		if (keys[GLFW_KEY_S])
			coord::camera_back(time_system::delta_time() * coord::cameraSpeed);
		if (keys[GLFW_KEY_A])
			coord::camera_left(time_system::delta_time() * coord::cameraSpeed);
		if (keys[GLFW_KEY_D])
			coord::camera_right(time_system::delta_time() * coord::cameraSpeed);
	}
}
