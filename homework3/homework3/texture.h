#pragma once
#include "std_header.h"

namespace texture {
	extern std::string resource_path;

	class texture2D {
	public:
		int width, height;
		GLuint texture;
		bool load_failed;

		texture2D(std::string);
	};
	
	extern std::unordered_map <std::string, texture2D> texture2D_list;

	int texture_init();
}
