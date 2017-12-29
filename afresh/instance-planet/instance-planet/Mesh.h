#pragma once
#include <glad/glad.h>
#include <string>
#include <vector>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

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
};

class Mesh
{
public:
	std::vector<Vertex> verVec;
	std::vector<Texture> texVec;
	std::vector<GLuint> indexVec;
	Mesh(std::vector<Vertex> verVec, std::vector<Texture> texVec, std::vector<GLuint> indexVec);
	~Mesh();
	void Draw();

private:
	GLuint VAO, VBO, EBO;
	void setupMesh();
};