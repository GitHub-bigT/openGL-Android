#pragma once
#include <glad/glad.h>
#include <string>
#include <vector>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};
struct Texture
{
	GLuint id;
	std::string type;
	aiString path;
};

class Mesh
{
public:
	GLuint VAO;
	std::vector<Vertex> verVec;
	std::vector<Texture> texVec;
	std::vector<GLuint> indexVec;
	Mesh(std::vector<Vertex> verVec, std::vector<Texture> texVec, std::vector<GLuint> indexVec)
	{
		this->verVec = verVec;
		this->texVec = texVec;
		this->indexVec = indexVec;

		setupMesh();
	}

	~Mesh()
	{

	}

	void Draw(Shader shader)
	{
		for (unsigned int i = 0; i < texVec.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, texVec[i].id);
		}
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indexVec.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

private:
	GLuint  VBO, EBO;
	void setupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * verVec.size(), &verVec[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexVec.size(), &indexVec[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));
	}
};