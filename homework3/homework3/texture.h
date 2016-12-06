#pragma once
#include "std_header.h"

namespace texture {
	class texture2D {
	private:
		int width, height;
		GLuint texture;
		std::string type;
		bool load_failed;
	public:
		texture2D(std::string);
		const bool & load_fail() const;
		//获取纹理绑定编号
		const GLuint & getTexture() const;
		const int & getWidth() const;
		const int & getHeight() const;
		const std::string & getType() const;
		void setType(std::string);
	};
	
	//全局纹理列表
	const std::unordered_map <std::string, texture2D> & get_texture2D_list();
	//全局纹理初始化
	int texture_init();
}
