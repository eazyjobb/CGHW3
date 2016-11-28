#include "time_system.h"

namespace time_system {
	GLfloat preframe_time, curframe_time;

	int time_system_init() {
		preframe_time = curframe_time = (GLfloat)glfwGetTime();
		return 0;
	}

	GLfloat delta_time() {
		return curframe_time - preframe_time;
	}

	const GLfloat get_preframe_time()
	{
		return preframe_time;
	}

	const GLfloat get_curframe_time()
	{
		return curframe_time;
	}

	void refresh() {
		preframe_time = curframe_time;
		curframe_time = (GLfloat)glfwGetTime();
	}
}
