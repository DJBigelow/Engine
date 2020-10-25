#include "Mesh.h"

std::vector<Vertex> Mesh::getVertices()
{
    return vertices;
}

std::vector<unsigned int> Mesh::getIndices()
{
    return indices;
}

std::vector<Texture> Mesh::getTextures()
{
    return textures;
}



void Mesh::setVertices(std::vector<Vertex> vertices)
{
    this->vertices = vertices;
}

void Mesh::setIndices(std::vector<unsigned int> indices)
{
    this->indices = indices;
}

void Mesh::setTextures(std::vector<Texture> textures)
{
    this->textures = textures;
}



Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    intializeBuffers();
}

void Mesh::intializeBuffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 &indices[0],
                 GL_STATIC_DRAW);

    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoordinates));

    glBindVertexArray(0);
}
