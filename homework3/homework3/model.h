#pragma once
#include "std_header.h"
#include "texture.h"
#include "light.h"

namespace model {
	class Material {
	private:
		GLuint diffuse;		//漫反射编号
		GLuint specular;	//光照贴图编号
		float shininess;	//镜面反射参数：越大约光滑
	public:
		Material(const GLuint _diffuse, const GLuint _specular, const float _shininess);
		void Bind(const GLuint program, const char* _diffuse, const char* _specular, const char* _shininess) const;
	};

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	class Mesh
	{
	public:
		Mesh(const std::vector<Vertex> &_vertices, const std::vector<GLuint> &_indices, const std::vector<std::string> &_texture, float _shininess = 16.0f);
		void Draw(GLuint program) const;
		~Mesh();

	private:
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<GLuint> textures_index;
		std::vector<std::string> textures_name;
		std::vector<std::string> textures_type;
		float shininess;
		GLuint VAO, VBO, EBO;
		void setupMesh();
	};

	class Model {
	public:
		Model(const aiScene* scene);
		~Model();
		void Draw(GLuint program) const;
	private:
		std::vector <Mesh> meshes;

	};

	bool read_model(const std::string &);
	const std::unordered_map <std::string, Model> & getModelList();
}
