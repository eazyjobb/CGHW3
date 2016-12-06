#pragma once
#include "std_header.h"

namespace texture {
	class texture2D {
	private:
		int width, height;
		GLuint texture;
		std::string type;
	public:
		texture2D(GLuint texture_number, int width, int height);
		//获取纹理绑定编号
		const GLuint & getTexture() const;
		const int & getWidth() const;
		const int & getHeight() const;
		const std::string & getType() const;
		void setType(const char *str);
	};
	
	//全局纹理列表
	const std::unordered_map <std::string, texture2D> & get_texture2D_list();
	//全局纹理初始化
	int texture_init();
	//尝试读取一个纹理并插入到全局纹理列表中，如果已存在则不做重复插入，如果文件不存在或者纹理分配失败返回false
	bool read_insert(const char * str);
}
