#include "coord.h"

namespace coord {
	glm::mat4 view, projection;

	int coord_init() {
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective((GLfloat)glm::radians(45.0), (GLfloat)window_width / (GLfloat)window_height, 0.1f, 100.0f);

		return 0;
	}
}
