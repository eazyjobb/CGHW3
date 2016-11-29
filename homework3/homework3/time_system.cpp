#include "time_system.h"

namespace time_system {
	GLfloat preframe_time, curframe_time, max_time, min_time;
	bool first;

	int time_system_init() {
		max_time = 0.0f;
		min_time = 100.0f;
		first = true;
		preframe_time = curframe_time = (GLfloat)glfwGetTime();
		return 0;
	}

	GLfloat delta_time() {
		return curframe_time - preframe_time;
	}

	const GLfloat &get_preframe_time()
	{
		return preframe_time;
	}

	const GLfloat &get_curframe_time()
	{
		return curframe_time;
	}

	void refresh() {
		preframe_time = curframe_time;
		curframe_time = (GLfloat)glfwGetTime();
#ifdef fps_tester
		if (first) {
			first = false;
			return;
		}
		min_time = std::min(min_time, curframe_time - preframe_time);
		max_time = std::max(max_time, curframe_time - preframe_time);
		std::cout << 1.0f / min_time << ' ' << 1.0f / max_time << ' ' << 1.0f / (curframe_time - preframe_time) << std::endl;
#endif // fps_tester
	}
}
