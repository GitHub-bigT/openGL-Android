#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
// GL Includes
#include <../GL/glew.h> // Contains all the necessery OpenGL includes
#include <../glm/glm.hpp>
#include <../glm/gtc/matrix_transform.hpp>

#include "BTShader.h"
#include <../stdImage/stb_image.h>

struct Vertex{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};
struct Texture{
	unsigned int id;
	string type;
	aiString path;//�洢�����·�����ں�����������бȽ�
};
enum Attrib_ID{
	vPosition = 1,
	vNormal = 2,
	vTexCoords = 3
};
unsigned int textureId;

class Mesh{
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	//��������
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures){
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		this->setupMesh();
	}
	void Draw(Shader shader){
		// diffuseNum++  �ȷ��ر����ڵ����� ���Գ�ʼ��Ϊ1
		unsigned int diffuseNum = 1;
		unsigned int specularNum = 1;
		/*
		for (int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			//��ȡ�������
			stringstream ss;
			string number;
			string name = textures[i].type;
			if (name == "texture_diffuse")
			{
				ss << diffuseNum++;
			}
			else if (name == "texture_specular")
			{
				ss << specularNum++;
			}
			number == ss.str();

			glUniform1i(glGetUniformLocation(shader.program, (name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D,textures[i].id);
		}
		*/


		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(shader.program, "texture_diffuse1"),0);
		glBindTexture(GL_TEXTURE_2D,textureId);
		

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}
private:
	//��Ⱦ����
	unsigned int VAO, VBO, EBO;
	//��ʼ������
	void setupMesh(){
		
		glGenTextures(1, &textureId);
		int width, height, channels;
		unsigned char* data = stbi_load("earth.bmp", &width, &height, &channels, 0);
		printf("��ɫͨ��������%d\n", channels);

		glBindTexture(GL_TEXTURE_2D, textureId);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
		glGenVertexArrays(1,&VAO);
		glGenBuffers(1,&VBO);
		glGenBuffers(1,&EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Vertex),&vertices[0],GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(unsigned int),&indices[0],GL_STATIC_DRAW);

		//����λ��
		glVertexAttribPointer(vPosition , 3 , GL_FLOAT , GL_FALSE , sizeof(Vertex) , (const GLvoid*) 0);
		//���㷨��
		glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, Normal));
		//������������
		glVertexAttribPointer(vTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, TexCoords));
		glEnableVertexAttribArray(vPosition);
		glEnableVertexAttribArray(vNormal);
		glEnableVertexAttribArray(vTexCoords);
	}
};


