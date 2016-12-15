#include "model.h"

namespace model {
	const std::string model_path("resources/model/");
	std::unordered_map<std::string, Model> model_list;

	Material::Material(const GLuint _diffuse, const GLuint _specular, const float _shininess)
		: diffuse(_diffuse), specular(_specular), shininess(_shininess) {}

	void Material::Bind(const GLuint program, const char * _diffuse, const char * _specular, const char * _shininess) const {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse);
		glUniform1i(glGetUniformLocation(program, _diffuse), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular);
		glUniform1i(glGetUniformLocation(program, _specular), 1);

		glUniform1f(glGetUniformLocation(program, _shininess), shininess);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	Mesh::Mesh(const std::vector<Vertex>& _vertices, const std::vector<GLuint>& _indices, const std::vector<std::string>& _texture, float _shininess):
		vertices(_vertices), indices(_indices), textures_name(_texture), shininess(_shininess)
	{
		auto texture_list = texture::get_texture2D_list();
		for (const auto & i : textures_name) {
			auto ptr = texture_list.find(i);
			if (ptr != texture_list.end()) {
				textures_index.push_back(ptr->second.getTexture());
				textures_type.push_back(ptr->second.getType());
			}
			else
				std::cout << "MESH::MESH::TEXTURE_DO_NOT_EXIST" << std::endl;
		}

		setupMesh();
	}

	void Mesh::Draw(GLuint program) const {
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;

		for (size_t i = 0; i < textures_index.size(); ++ i)
		{
			glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
											  // Retrieve texture number (the N in diffuse_textureN)
			char number;
			const std::string &name = textures_type[i];

			if (name == "texture_diffuse")
				number = 48 + (diffuseNr++); // Transfer GLuint to stream
			else if (name == "texture_specular")
				number = 48 + (diffuseNr++); // Transfer GLuint to stream

			// Now set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(program, (name + number).c_str()), i);
			// And finally bind the texture
			glBindTexture(GL_TEXTURE_2D, textures_index[i]);
		}

		glUniform1f(glGetUniformLocation(program, "shininess"), shininess);

		// Draw mesh
		glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Always good practice to set everything back to defaults once configured.
		for (size_t i = 0; i < textures_index.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	Mesh::~Mesh() {

	}

	void Mesh::release() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void Mesh::setupMesh() {		

		glGenVertexArrays(1, &this->VAO);

		std::cerr << "this Mesh's VAO is : " << this->VAO << std::endl;

		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);

			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}

	/////////////////////////////////////////////////////////////////////

	bool read_model(const std::string & name) {
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(model_path + name, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return false;
		}
		//didn't check the model whether exist! need fix!
		model_list.insert(std::make_pair(name, Model(scene)));

		return true;
	}

	void release_all_model() {
		for (auto &i : model_list) i.second.release();
	}

	const std::unordered_map<std::string, Model>& getModelList() {
		return model_list;
	}

	///////////////////////////////////////////////////////////////////////

	Model::Model(const aiScene * scene) {
		this->processNode(scene->mRootNode, scene);
	}

	Model::~Model() {}

	void Model::Draw(GLuint program) const{
		for (const auto & i : meshes)
			i.Draw(program);
	}

	void Model::release() {
		for (auto &i : meshes) i.release();
	}

	void Model::processNode(aiNode * node, const aiScene * scene) {
		for (GLuint i = 0; i < node->mNumMeshes; i++) {
			// The node object only contains indices to index the actual objects in the scene. 
			// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(this->processMesh(mesh, scene));
		}
		// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (GLuint i = 0; i < node->mNumChildren; i++)	{
			this->processNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene) {
		// Data to fill
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<std::string> textures;

		for (GLuint i = 0; i < mesh->mNumVertices; ++i) {
			Vertex vertex;
			glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
							  // Positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// Normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			// Texture Coordinates
			if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			vertices.push_back(vertex);
		}

		for (GLuint i = 0; i < mesh->mNumFaces; ++i) {
			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector
			for (GLuint j = 0; j < face.mNumIndices; ++j)
				indices.push_back(face.mIndices[j]);
		}

		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// Diffuse: texture_diffuseN
			// Specular: texture_specularN
			// Normal: texture_normalN

			// 1. Diffuse maps
			std::vector<std::string> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. Specular maps
			std::vector<std::string> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}
		
		return Mesh(vertices, indices, textures);
	}

	std::vector<std::string> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, const std::string & typeName)
	{
		std::vector<std::string> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
			aiString str;
			mat->GetTexture(type, i, &str);
			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture

			if (!texture::read_insert(str.C_Str())) {
				std::cerr << "model::model::loadMasterialTexutres loading texture: (" <<  str.C_Str() << ")failed!" << std::endl;
			}
			else {
				std::cerr << "loading texture: " << str.C_Str() << std::endl;
				texture::get_texture2D_list_fix().find(std::string(str.C_Str()))->second.setType(typeName.c_str());
				textures.push_back(str.C_Str());
			}			
		}
		return textures;
	}

}
