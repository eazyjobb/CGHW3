#include "texture.h"

namespace texture {
	std::string resource_path = "resources/textures/";
	std::unordered_map <std::string, texture2D> texture2D_list;

	const std::unordered_map<std::string, texture2D>& get_texture2D_list()
	{
		return texture2D_list;
	}
	int texture_init() {
		std::ifstream lin(resource_path + "texture2D_list.txt");

		std::string str; size_t n;
		lin >> n;

		for (size_t i = 0; i < n; ++i) {
			lin >> str;
			if (!read_insert(str.c_str())) {
				std::cerr << "ERROR::TEXTURE::TEXTURE2D::NAME_CONFLICT " << str << std::endl; 
				return -1;
			}
		}
		lin.close();

		return 0;
	}
	bool read_insert(const char * str) {
		if (texture2D_list.count(std::string(str))) return true;

		//img loading and check that whether the img exist
		std::string imgf = resource_path + str;
		std::ifstream is(imgf.c_str());
		if (!is.is_open()) return false;

		GLuint texture = 0; int width, height;

		unsigned char* image = SOIL_load_image(imgf.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		if (image == NULL) {
			std::cerr << "ERROR::TEXTURE::TEXTURE2D::LOAD_FAILED " << imgf << std::endl;
			SOIL_free_image_data(image);			
			return false;
		}

		//binding and check whether the texture be gen
		glGenTextures(1, &texture);
		if (!texture) return false;

		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);

		texture2D_list.insert(std::make_pair(std::string(str), texture2D(texture, width, height)));

		return true;
	}
	texture2D::texture2D(GLuint text, int w, int h): texture(text), width(w), height(h)	{
		
	}
	const GLuint & texture2D::getTexture() const
	{
		return texture;
	}
	const int & texture2D::getWidth() const
	{
		return width;
	}
	const int & texture2D::getHeight() const
	{
		return height;
	}
<<<<<<< HEAD
	const std::string & texture2D::getType() const {
		return type;
	}
	void texture2D::setType(std::string x)
	{
		type = x;
=======
	const std::string & texture2D::getType() const
	{
		return type;
	}
	void texture2D::setType(const char * str) 
	{
		type = std::string(str);
>>>>>>> origin/master
	}
}
