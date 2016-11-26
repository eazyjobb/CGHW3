#include "texture.h"

namespace texture {
	std::string resource_path = "resources/textures/";
	std::unordered_map <std::string, texture2D> texture2D_list;

	int texture_init() {
		std::ifstream lin(resource_path + "texture2D_list.txt");

		std::string str; size_t n;
		lin >> n;

		for (size_t i = 0; i < n; ++i) {
			lin >> str;
			if (texture2D_list.count(str)) {
				std::cout << "ERROR::TEXTURE::TEXTURE2D::NAME_CONFLICT " << str << std::endl;
				return -1;
			}
			texture2D_list.insert(std::make_pair(str, texture2D(resource_path + str)));
			auto ptr = texture2D_list.find(str);
			if (ptr == texture2D_list.end() || ptr->second.load_failed) {
				std::cout << "ERROR::TEXTURE::TEXTURE2D::LOAD_FAILED " << str << std::endl;
				return -1;
			}
		}

		lin.close();

		return 0;
	}

	texture2D::texture2D(std::string name) {
		load_failed = false;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		unsigned char* image = SOIL_load_image(name.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		if (image == NULL) {
			std::cout << "ERROR::TEXTURE::TEXTURE2D::LOAD_FAILED " << name << std::endl;
			load_failed = true;
			SOIL_free_image_data(image);
			glBindTexture(GL_TEXTURE_2D, 0);
			return;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
