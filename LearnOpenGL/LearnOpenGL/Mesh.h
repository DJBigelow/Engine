#pragma once

#include "Vertex.h"
#include "Texture.h"
#include "Shader.h"

#include <vector>
#include <glad/glad.h>

class Mesh {
	public:
		std::vector<Vertex> getVertices();
		std::vector<unsigned int> getIndices();
		std::vector<Texture> getTextures();

		void setVertices(std::vector<Vertex> vertices);
		void setIndices(std::vector<unsigned int> indices);
		void setTextures(std::vector<Texture> textures);

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
		void Draw(Shader& shader);

	private:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;

		void intializeBuffers();
};