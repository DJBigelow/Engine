#pragma once

#include "Vertex.h"
#include "Texture.h"
#include "Shader.h"

#include <vector>
#include <glad/glad.h>

class Mesh {
	public:
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