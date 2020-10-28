#include "Model.h"

void Model::Draw(Shader& shader)
{
	for (Mesh mesh : meshes) {
		mesh.Draw(shader);
	}
}

void Model::loadModel(std::string filePath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode) {
		throw importer.GetErrorString();
	}

	directory = filePath.substr(0, filePath.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	//Each node contains an index to a mesh in the scene (node->mMeshes[i]). Here we access each mesh in the scene through the mesh indices of each node 
	for (int i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
	}

	//Nodes have a recursive strucure, meaning we need to recursively process each node and its children nodes
	for (int i = 0; node->mNumChildren; ++i) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	//Convert vertex data (positions and texture coords) to our format
	for (int i = 0; i < mesh->mNumVertices; ++i) {
		Vertex vertex;
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0]) {
			vertex.TextureCoordinates.x = mesh->mTextureCoords[0][i].x;
			vertex.TextureCoordinates.y = mesh->mTextureCoords[0][i].y;
		}
		else {
			vertex.TextureCoordinates = glm::vec2(0.0f);
		}

		vertices.push_back(vertex);
	}

	//Each face contains the ordered indices of the vertices required to draw it
	for (int i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];

		for (int j = 0; face.mNumIndices; ++j) {
			indices.push_back(face.mIndices[j]);
		}
	}

	//If mesh contains materials
	if (mesh->mMaterialIndex >= 0) {
		//Retrieve material through mesh's material index
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		//Insert diffuse maps between diffuseMaps.begin() and diffuseMaps.end() before textures.end()
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType textureType, std::string textureTypeName)
{
	std::vector<Texture> textures;

	for (int i = 0; i < material->GetTextureCount(textureType); ++i) {
		aiString texturePath;
		material->GetTexture(textureType, i, &texturePath);

		Texture texture;
		texture.id = TextureFromFile(texturePath.C_Str(), directory, false);
		texture.type = textureTypeName;
		texture.path = texturePath.C_Str();
		
		textures.push_back(texture);
	}

	return textures;
}

int Model::TextureFromFile(const char* filePath, const std::string& directory, bool gamma)
{
	std::string textureFilePath = directory + "/" + filePath;

	int width, height, numColorChannels;
	unsigned char* textureData = stbi_load(textureFilePath.c_str(), &width, &height, &numColorChannels, 0);


	//Generate and bind texture object
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);




	if (textureData) {
		//Arguments
		//1: -GL_TEXTURE_2D- Specifies the texture target, i.e. the texture currently bound to GL_TEXTURE_2D in the previous line
		//2: -0- Manually specifies the mipmap level of the texture, which we are currently leaving at the base level of 0
		//3: -GL_RGB- Specifies the color format the texture should be stored as
		//4 & 5: -width, height- Sets the width and height of the texture being created
		//6: -0- Always 0 due to legacy code
		//7, 8: -GL_RGB, GL_UNSINGED_BYTE- Specifies the format and datatype of the source image (GL_UNSIGNED_BYTE = unsigned char)
		//9: Image data for the textue
		GLenum format;
		if (numColorChannels == 1)
			format = GL_RED;
		else if (numColorChannels == 3)
			format = GL_RGB;
		else if (numColorChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);

		//Set texture wrapping/filtering options for currently bound texture object
		//These two near-identical calls are to set the texture wrapping options along the S and T axes (equivalent to x and y)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//Texture filtering option for minifying operations (scaling the texture down) and magnifying operations
		//When specifying these options for minifying operations, we can also set the method for mipmap filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
		stbi_image_free(textureData);
	}

	return texture;
}
