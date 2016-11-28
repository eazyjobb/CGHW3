#pragma once

#include "std_header.h"
#include "data_binding.h"
#include "coord.h"

namespace callback {
	/*
		键盘回调函数
	*/
	int callback_init();
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void mouse_button_callback(GLFWwindow* window, int key, int action, int mode);
	void handle_events(GLFWwindow*);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
}
