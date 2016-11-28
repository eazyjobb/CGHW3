#pragma once
#include "std_header.h"

namespace texture {
	class texture2D {
	private:
		int width, height;
		GLuint texture;
		bool load_failed;
	public:
		texture2D(std::string);
		const bool load_fail() const;
		const GLuint getTexture() const;
		const int getWidth() const;
		const int getHeight() const;
	};
	
	extern std::unordered_map <std::string, texture2D> texture2D_list;

	int texture_init();
}
