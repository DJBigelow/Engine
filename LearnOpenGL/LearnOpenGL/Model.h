#pragma once
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"

#include "Shader.h"
#include "Mesh.h"

class Model {
	public:
		Model(char* filePath) {
			loadModel(filePath);
		}
		void Draw(Shader& shader);

	private:
		std::vector<Mesh> meshes;
		std::string directory;
		
		void loadModel(std::string filePath);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType textureType, std::string textureTypeName);
		int TextureFromFile(const char* filePath, const std::string& directory, bool gamma);

};
