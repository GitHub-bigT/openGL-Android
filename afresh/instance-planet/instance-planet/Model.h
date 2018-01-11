/*
#pragma once
#include <string>
#include <vector>

#include <glad/glad.h> 

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Mesh2.h"

class Model
{
public:
	Model();
	void Draw(Shader shader);
	void loadModel(std::string path);
	~Model();

private:
	std::vector<Mesh> meshes;
	std::string directory;

	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
};

*/
