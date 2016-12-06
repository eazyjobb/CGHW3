#pragma once

#include "std_header.h"
#include "shader.h"
#include "texture.h"
#include "coord.h"
#include "light.h"
#include "model.h"

namespace coord_tester {
	void init();
	void refresh();
	void release();
}

namespace light_tester {
	void init();
	void refresh();
	void release();
}
