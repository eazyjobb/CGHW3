#include "model.h"
#include "global_utility.h"

#define FOR_DEBUG

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
		for (auto &i : vertices) {
			if (i.Weights[0] < 1e-6) {
				i.Weights[0] = 1;
				i.IDs[0] = 0;
			}
		}

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

	void Mesh::Draw(GLuint program) {
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

			//fix in 2016 12 16  10:15 am
			glEnableVertexAttribArray(3);
			glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, IDs));

			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, Weights));
			
		glBindVertexArray(0);
	}

	/////////////////////////////////////////////////////////////////////
	Assimp::Importer import;	//外移，用于测试

	bool read_model(const std::string & name) {
		
		const aiScene* scene = import.ReadFile(model_path + name, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return false;
		}
		#ifdef FOR_DEBUG
				std::cout << "animation number: " << scene->mNumAnimations << std::endl;
		#endif
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

	std::unordered_map<std::string, Model>& getModelList_notConst() {
		return model_list;
	}

	///////////////////////////////////////////////////////////////////////

	Model::Model(const aiScene * scene) {
		m_Scene = scene;

		assignment(m_GlobalInverseTransform, m_Scene->mRootNode->mTransformation);
		m_GlobalInverseTransform = glm::inverse(m_GlobalInverseTransform);

		Transforms.push_back(glm::mat4(1.0f));
		m_boneInfo.push_back(BoneInfo());
		assignment(m_boneInfo[0].FinalTransformation, glm::mat4(1.0f));
		assignment(m_boneInfo[0].BoneOffset, glm::mat4(1.0f));

		

		meshCnt = 0;
		root = this->processNode(scene->mRootNode, scene);
	}

	Model::~Model() {}

	void Model::Draw(GLuint program) {
		BoneTransform(time_system::get_preframe_time() / 30.0);
		for (auto & i : meshes) {
			i.Draw(program);
		}
		//assert(false);
	}

	void Model::release() {
		for (auto &i : meshes) i.release();
	}

	void fixedZero(glm::mat4 &u) {
		return;
		static const double eps = 1e-7;
		for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) if (-eps < u[i][j] && u[i][j] < eps) u[i][j] = 0;
		return;
		std::cout << std::endl;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) std::cout << u[i][j] << ' '; std::cout << std::endl;
		}			

		return;
	}

	glm::mat4 Model::BoneTransform(GLfloat TimeInSeconds) {
		
		glm::mat4 identity(1.0f);

		GLfloat TicksPerSecond = (m_Scene->mAnimations[0]->mTicksPerSecond != 0) ?
			m_Scene->mAnimations[0]->mTicksPerSecond : 25.0f;

		GLfloat TimeInTicks = TimeInSeconds * TicksPerSecond;
		
		GLfloat AnimationTime = fmod(TimeInTicks, std::max(0.0, m_Scene->mAnimations[0]->mDuration - 0.1));

		ReadNodeHeirarchy(AnimationTime, m_Scene->mRootNode, identity);

		if (Transforms.size() != m_boneInfo.size())
			Transforms.resize(m_boneInfo.size());

		//need fix
		
		auto ptr = shader::get_shader_list().find("model");

		//std::cout << "====================================" << std::endl;

		for (GLuint i = 0; i < m_boneInfo.size(); i++) {
			assignment(Transforms[i], m_boneInfo[i].FinalTransformation);
			fixedZero(Transforms[i]);

		//	Transforms[i] = glm::mat4(1.0f);			
			std::string name = "gBones[";
			std::stringstream ss;
			ss << i;
			name = name + ss.str() + "]";
			
			glUniformMatrix4fv(glGetUniformLocation(ptr->second.getProgram(), name.c_str()), 1, GL_FALSE, glm::value_ptr(Transforms[i]));
		}
		
		return glm::mat4();		
	}

	const aiNodeAnim * FindNodeAnim(const aiAnimation *pAnimation, std::string nodeName) {
		for (GLuint i = 0; i < pAnimation->mNumChannels; ++i) {			
			if (pAnimation->mChannels[i]->mNodeName.data == nodeName) return pAnimation->mChannels[i];
		}
		return nullptr;
	}
	
	GLuint FindPosition(GLfloat AnimationTime, const aiNodeAnim* pNodeAnim)	{
		
		for (GLuint i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
			if (AnimationTime < (GLfloat)pNodeAnim->mPositionKeys[i + 1].mTime) {
				return i;
			}
		}
		return 0;
	}
	
	GLuint FindRotation(GLfloat AnimationTime, const aiNodeAnim* pNodeAnim)	{
		
		assert(pNodeAnim->mNumRotationKeys > 0);

		for (GLuint i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
			if (AnimationTime < (GLfloat)pNodeAnim->mRotationKeys[i + 1].mTime) {
				return i;
			}
		}
		return 0;
	}

	GLuint FindScaling(GLfloat AnimationTime, const aiNodeAnim* pNodeAnim) {
		
		assert(pNodeAnim->mNumScalingKeys > 0);

		for (GLuint i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
			if (AnimationTime < (GLfloat)pNodeAnim->mScalingKeys[i + 1].mTime) {
				return i;
			}
		}		
		return 0;
	}


	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)	{
		if (pNodeAnim->mNumScalingKeys == 1) {
			Out = pNodeAnim->mScalingKeys[0].mValue;
			return;
		}

		GLuint ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
		GLuint NextScalingIndex = (ScalingIndex + 1);
		assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
		GLfloat DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
		GLfloat Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
		//assert(Factor >= 0.0f && Factor <= 1.0f);
		if (!(Factor >= 0.0f && Factor <= 1.0f)) Factor = 0.5f;
		const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
		const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}

	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim) {
		// we need at least two values to interpolate...

		assert(pNodeAnim->mNumPositionKeys > 0);

		if (pNodeAnim->mNumRotationKeys == 1) {
			Out = pNodeAnim->mRotationKeys[0].mValue;
			return;
		}

		GLuint RotationIndex = FindRotation(AnimationTime, pNodeAnim);
		GLuint NextRotationIndex = (RotationIndex + 1);
		assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
		float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
		//assert(Factor >= 0.0f && Factor <= 1.0f);
		if (!(Factor >= 0.0f && Factor <= 1.0f)) Factor = 0.5f;
		const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
		const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
		aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
		Out = Out.Normalize();
	}

	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim) {
		
		if (pNodeAnim->mNumPositionKeys == 1) {
			Out = pNodeAnim->mPositionKeys[0].mValue;
			return;
		}

		GLuint PositionIndex = FindPosition(AnimationTime, pNodeAnim);
		GLuint NextPositionIndex = (PositionIndex + 1);
		assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
		float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
		if (!(Factor >= 0.0f && Factor <= 1.0f)) Factor = 0.5f;
		//assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
		const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}

	int cnt = 0;
	
	void Model::ReadNodeHeirarchy(float AnimationTime, const aiNode * pNode, const glm::mat4 & ParentTransform, glm::vec4 tester) {
		//if (cnt > 6) return;

		++cnt;
		

		std::string NodeName(pNode->mName.data);


		for (int i = 0; i < cnt; ++i) std::cout << " " ;
		std::cout << "" << pNode->mName.data << ' ' << pNode->mNumMeshes << ' ' << (m_boneMap.find(NodeName) != m_boneMap.end()) << std::endl;
		//	std::cout << "" << pNode->mNumMeshes << std::endl;

		const aiAnimation* pAnimation = m_Scene->mAnimations[0];

		glm::mat4 ThisTrans; assignment(ThisTrans, pNode->mTransformation);

		glm::mat4 NodeTransformation(1.0f); assignment(NodeTransformation, pNode->mTransformation);
		
		//NodeTransformation = glm::transpose(NodeTransformation);

		const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

		if (pNodeAnim != nullptr) {
			// Interpolate scaling and generate scaling transformation matrix
			aiVector3D Scaling;
			CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
			glm::mat4 ScalingM(1.0f);
			//ScalingM = glm::scale(ScalingM, glm::vec3(Scaling.x, Scaling.y, Scaling.z));
			// Interpolate rotation and generate rotation transformation matrix
			aiQuaternion RotationQ;
			CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
			glm::mat4 RotationM(1.0f);

			aiMatrix3x3 s1 = RotationQ.GetMatrix();
			aiMatrix4x4 s2(s1);

			assignment(RotationM, s2);
			// Interpolate translation and generate translation transformation matrix
			aiVector3D Translation;
			CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
			glm::mat4 TranslationM(1.0f);
			TranslationM = glm::translate(TranslationM, glm::vec3(Translation.x, Translation.y, Translation.z));
			// Combine the above transformations

			//glm::mat4(1.0f);//
			//NodeTransformation = TranslationM * RotationM * ScalingM;
			int a;
			a = 1;
		}
		
		glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

		if (m_boneMap.find(NodeName) != m_boneMap.end()) {
			GLuint boneIndex = m_boneMap[NodeName];
			glm::mat4 boneOffset(1.0f); assignment(boneOffset, m_boneInfo[boneIndex].BoneOffset);
			//boneOffset = glm::transpose(boneOffset);
			
			assignment(m_boneInfo[boneIndex].FinalTransformation, m_GlobalInverseTransform * GlobalTransformation *
				 boneOffset);
			if (boneIndex == 1) {
				/*
				glm::mat4 ans = GlobalTransformation * boneOffset;

				for (int i = 0; i < 4; ++i) {
					for (int j = 0; j < 4; ++j) std::cout << GlobalTransformation[j][i] << ' '; std::cout << std::endl;
				}
				for (int i = 0; i < 4; ++i) {
					for (int j = 0; j < 4; ++j) std::cout << boneOffset[j][i] << ' '; std::cout << std::endl;
				}
				*/
				int a = 1;
			}
		}
		
		for (GLuint i = 0; i < pNode->mNumChildren; i++) {
			ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], m_boneMap.find(NodeName) != m_boneMap.end() || 0 ? GlobalTransformation : glm::mat4(1.0f));
		}
		
		--cnt;
	}

	std::ofstream os("log.txt"); 

	GLuint Model::processNode(aiNode * node, const aiScene * scene) {

		GLuint nt = meshNode.size();
		meshNode.push_back(MeshEntry(nt));
		meshNode[nt].MeshBase = meshes.size();
		meshNode[nt].MeshSize = node->mNumMeshes;

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

		return nt;
	}

	Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene) {
		std::cout << "bones number : " << mesh->mNumBones << std::endl;
		
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
			#ifdef FOR_DEBUG
				//vector.x /= 30.0; vector.y /= 30.0; vector.z /= 30.0;
			#endif

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
		
		// loading bone
		//mesh->mBones[0]->mOffsetMatrix 

		for (GLuint i = 0; i < mesh->mNumBones; ++i) {
			std::cerr << mesh->mBones[i]->mName.data << std::endl;

			GLuint boneIndex = 0;
			std::string boneName(mesh->mBones[i]->mName.data);
			if (m_boneMap.count(boneName) == 0) {
				m_boneMap[boneName] = boneIndex = m_boneInfo.size();

				m_boneInfo.push_back(BoneInfo());

				m_boneInfo[boneIndex].BoneOffset = mesh->mBones[i]->mOffsetMatrix;
				m_boneInfo[boneIndex].FinalTransformation = aiMatrix4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
			}
			else {
				boneIndex = m_boneMap[boneName];
			}
			
			for (GLuint j = 0; j < mesh->mBones[i]->mNumWeights; ++j) {
				GLuint vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
				GLfloat weight = mesh->mBones[i]->mWeights[j].mWeight;
				vertices[vertexID].addBoneData(boneIndex, weight);
			}
		}

		#ifdef FOR_DEBUG
		//for debug!
		#endif

		return Mesh(vertices, indices, textures);
	}

	std::vector<std::string> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, const std::string & typeName)
	{
		std::vector<std::string> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
			aiString str;
			mat->GetTexture(type, i, &str);
			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture

			std::string fixed(str.C_Str());

			for (int i = fixed.size() - 1; i >= 0; --i) if (fixed[i] == '\\' || fixed[i] == '/') {		//修改绝对路径为相对路径
				fixed.erase(0, i + 1);
				break;
			}

			if (!texture::read_insert(fixed.c_str())) {
				std::cerr << "model::model::loadMasterialTexutres loading texture: (" <<  fixed << ")failed!" << std::endl;
			}
			else {
				std::cerr << "loading texture: " << fixed << std::endl;
				texture::get_texture2D_list_fix().find(std::string(fixed.c_str()))->second.setType(typeName.c_str());
				textures.push_back(fixed.c_str());
			}			
		}
		return textures;
	}

}


