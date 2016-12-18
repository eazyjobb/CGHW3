#pragma once
#include "std_header.h"
#include "texture.h"
#include "light.h"

#define NUM_BONES_PER_VEREX 4

namespace model {
	//////////////////////debug
	void debugAddLine(glm::vec3 a, glm::vec3 b, glm::vec3 rgb);
	void debugDrawLine();
	void debugRelease();

	//////////////////////debug


	class Material {
	private:
		GLuint diffuse;		//漫反射编号
		GLuint specular;	//光照贴图编号
		float shininess;	//镜面反射参数：越大约光滑
	public:
		Material(const GLuint _diffuse, const GLuint _specular, const float _shininess);
		void Bind(const GLuint program, const char* _diffuse, const char* _specular, const char* _shininess) const;
	};

	struct Vertex {

		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		GLuint IDs[NUM_BONES_PER_VEREX];
		GLfloat Weights[NUM_BONES_PER_VEREX];

		Vertex() {
			memset(IDs, 0, sizeof(IDs));
			memset(Weights, 0, sizeof(Weights));
		}

		void addBoneData(GLuint boneID, GLfloat weight) {
			for (int i = 0; i < NUM_BONES_PER_VEREX; ++i) if (Weights[i] < 1e-4) {
				IDs[i] = boneID; Weights[i] = weight; 
				return ;
			}
			assert(0);
		}
	};

	struct MeshEntry {
		GLuint VertexBase, VertexSize;
		GLuint MeshBase, MeshSize;
		GLuint index;
		std::vector<GLuint > son;
		MeshEntry(GLuint index) : index(index) {}
	};

	struct BoneInfo {
		aiMatrix4x4 BoneOffset, FinalTransformation;
	};

	class Mesh
	{
	public:
		Mesh(const std::vector<Vertex> &_vertices, const std::vector<GLuint> &_indices, const std::vector<std::string> &_texture, float _shininess = 16.0f);
		void Draw(GLuint program) ;
		~Mesh();
		void release();
		
								
	private:
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<GLuint> textures_index;
		std::vector<std::string> textures_name;
		std::vector<std::string> textures_type;
		float shininess;
		GLuint VAO, VBO, EBO;

		
		GLfloat TicksPerSecond;				//一秒多少个tick，为了同步动画速率，为了测试代码，后续可能移动其所在位置
		GLfloat Duration;					//动画持续时间（？），为了测试代码，后续可能移动其所在位置
		GLuint m_NumBones;					//骨骼数量
		
	private:
		void setupMesh();
	};

	class Model {
	public:
		Model(const aiScene* scene);
		~Model();
		void Draw(GLuint program) ;
		void release();

		glm::mat4 BoneTransform(GLfloat TimeInSeconds);
		void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4 &ParentTransform, glm::vec4 tester = glm::vec4(0, 0, 0, 1));
	private:
		GLuint processNode(aiNode *node, const aiScene *scene);			//遍历mesh，同时构建树结构以便维护骨骼
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<std::string> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string &typeName);

	private:
		


		std::vector <Mesh> meshes;
		std::vector <MeshEntry> meshNode;
		GLuint meshCnt, root;

		const aiScene *m_Scene;	//for test
		
		std::vector<BoneInfo> m_boneInfo;
		std::unordered_map<std::string, GLuint> m_boneMap;
		std::vector<glm::mat4> Transforms;	//对应骨骼的最终变换矩阵

		glm::mat4 m_GlobalInverseTransform;		//模型的全局变换
	};

	bool read_model(const std::string &);
	void release_all_model();
	const std::unordered_map <std::string, Model> & getModelList();
	std::unordered_map <std::string, Model> & getModelList_notConst();
}
