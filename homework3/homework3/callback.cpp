#include "callback.h"

namespace callback {
	bool keys[1024];
	bool mouse_keys[8];
	bool first_mouse;

	int show_staus = 1;

	GLfloat mouse_last_x, mouse_last_y, scroll_y, mouse_move_x, mouse_move_y;
	
	int callback_init() {
		first_mouse = true;
		return 0;
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		if (key < 0)
			return;
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

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		scroll_y = (GLfloat)yoffset;
	}

	int get_showStaus()	{
		return show_staus;
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

		mouse_move_x = xoffset;
		mouse_move_y = yoffset;
	}

	void handle_events(GLFWwindow* window) {
		if (mouse_keys[GLFW_MOUSE_BUTTON_RIGHT])
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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
		if (keys[GLFW_KEY_1]) {
			show_staus = 1;
		}
		if (keys[GLFW_KEY_2]) {
			show_staus = 2;
		}
		if (keys[GLFW_KEY_3]) {
			show_staus = 3;
		}


		if (false == coord::get_swithing()) {
			auto ptr = coord::get_current_camera();
			if (keys[GLFW_KEY_W])
				ptr->second.camera_front(time_system::delta_time());
			if (keys[GLFW_KEY_S])
				ptr->second.camera_back(time_system::delta_time());
			if (keys[GLFW_KEY_A])
				ptr->second.camera_left(time_system::delta_time());
			if (keys[GLFW_KEY_D])
				ptr->second.camera_right(time_system::delta_time());
			if (scroll_y != 0.0f)
				ptr->second.camera_fov((GLfloat)-scroll_y);
			if (mouse_move_x != 0.0f || mouse_move_y != 0.0f)
				ptr->second.camera_rotate(mouse_move_x, mouse_move_y);
			if (keys[GLFW_KEY_O])
				coord::set_current_camera("oppsite");
			if (keys[GLFW_KEY_N])
				coord::set_current_camera("normal");
		}
		mouse_move_x = mouse_move_y = scroll_y = 0.0f;
	}


}
