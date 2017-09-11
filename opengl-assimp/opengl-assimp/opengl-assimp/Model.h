//std
#include <string>
#include <vector>
using namespace std;

//assimp
#include <../assimp/Importer.hpp>
#include <../assimp/scene.h>
#include <../assimp/postprocess.h>

#include "BTShader.h"
#include "Mesh.h"
#include <../stdImage/stb_image.h>

int TextureFromFile(const char* path , const string &direction , bool gamma = false);

class Model{
public:
	//���캯��
	Model(char* path){
		this->loadModel(path);
	}
	//������������  ����
	void Draw(Shader shader){
		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i].Draw(shader);
		}
	}
private:
	//ģ������
	vector<Mesh> meshes;
	string directory;

	void loadModel(string path){
		//����ģ�͵�assimp��sceneģ���С�scene��assimp�ĸ�����
		Assimp::Importer importer;
		//aiProcess_Triangulate���ģ�Ͳ��ǣ���ȫ�ǣ������ι��ɣ�����Ҫ��ģ�����е�ͼԪ��״�任Ϊ������
		//aiProcess_FlipUVs ��Ϊ�������У�opengl�󲿷�ͼ���Y�ᶼ�Ƿ��ġ��������ѡ���Ƿ�תy�����������
		//aiProcess_GenNormals ���ģ�Ͳ��������ߵĻ�����Ϊÿ���������ɷ���
		const aiScene *scene = importer.ReadFile(path,aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
		//���scene�͸��ڵ��Ƿ�Ϊ�գ����scene��һ����ǣ��鿴���ص������ǲ��ǲ�������
		if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
		{
			cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
			return;
		}
		directory = path.substr(0,path.find_last_of('/'));

		printf("scene �� mesh ������Ϊ��%d\n", scene->mNumMeshes);
		processNode(scene->mRootNode,scene);
	}

	//�ݹ麯��������scene�е����нڵ�
	void processNode(aiNode *node , const aiScene *scene){
		printf("node �� mesh ������Ϊ��%d\n",node->mNumMeshes);
		//printf("node �� �ڵ� ������Ϊ��%d\n", node->mNumChildren);
		//����ڵ����������
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh,scene));
		}
		//���������ӽڵ�
		for (int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i],scene);
		}
	}

	//��assimp�е����ݽ�����Mesh����
	Mesh processMesh(aiMesh *mesh , const aiScene *scene){
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		//������
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			//����������
			glm::vec3 posVector;
			posVector.x = mesh->mVertices[i].x;
			posVector.y = mesh->mVertices[i].y;
			posVector.z = mesh->mVertices[i].z;
			vertex.Position = posVector;
			//����
			glm::vec3 norVector;
			norVector.x = mesh->mNormals[i].x;
			norVector.y = mesh->mNormals[i].y;
			norVector.z = mesh->mNormals[i].z;
			vertex.Normal = norVector;
			//��������
			if (!mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				//assimp����һ��ģ����һ���������а˸���ͬ������
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].x;
				vertex.TexCoords = vec;
			}
			else
			{
				vertex.TexCoords = glm::vec2(0.0f,0.0f);
			}
			vertices.push_back(vertex);
		}

		//��������
		//������face��
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}
		//�������
		
		printf("��������%d\n", mesh->mMaterialIndex);
		if (mesh->mMaterialIndex >= 0)
		{
			//һ������ֻ������һ����������
			aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
			vector<Texture> diffuseMaps = loadMaterialTextures(material,aiTextureType_DIFFUSE,"texture_diffuse");
			//��textures��ĩβ����diffuseMaps������Ԫ��
			textures.insert(textures.end(),diffuseMaps.begin(),diffuseMaps.end());
			vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_diffuse");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}
		
		return Mesh(vertices,indices,textures);
	}
	vector<Texture> loadMaterialTextures(aiMaterial *material , aiTextureType type,string typeName){
		vector<Texture> textures;
		printf("----- %s -----\n",typeName.c_str());
		printf("��������:%d\n", material->GetTextureCount(type));
		for (int i = 0; i < material->GetTextureCount(type); i++)
		{	/*
			aiString str;
			material->GetTexture(type,i,&str);
			printf("����ͼƬ���ƣ�%s\n",str);
			
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(),directory);
			texture.type = typeName;
			textures.push_back(texture);
			*/
		}
		return textures;
	}

};
//���ߺ���  ����һ������ ������id
int TextureFromFile(const char* path, const string &direction, bool gamma){
	string fileName = string(path);
	fileName = direction + '/' + fileName;

	unsigned int textureId;
	glGenTextures(1,&textureId);
	int width, height, channels;
	unsigned char* data = stbi_load(fileName.c_str(),&width,&height,&channels,0);
	printf("��ɫͨ��������%d\n", channels);
	if (data)
	{
		GLenum format;
		switch (channels)
		{
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			break;
		}
		glBindTexture(GL_TEXTURE_2D,textureId);
		glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else
	{
		printf("TEXTURE failed to load at path : %s",path);
	}
	
	return textureId;
}