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

vector<Texture> textures_loaded;//�����м��ع�������洢����һ��vector��
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
		printf("---------- Assimp:Load Model Start ----------\n");
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
		//���scene�а���������
		printf("\nģ���ܹ��� %d ��mNumMeshes\n", scene->mNumMeshes);
		printf("\nģ���ܹ��� %d ��mNumMaterials\n", scene->mNumMaterials);
		printf("\nģ���ܹ��� %d ��mNumTextures\n", scene->mNumTextures);
		printf("\nģ���ܹ��� %d ��mNumLights\n", scene->mNumLights);
		printf("\nģ���ܹ��� %d ��mNumCameras\n", scene->mNumCameras);
		printf("\nģ���ܹ��� %d ��mNumAnimations\n", scene->mNumAnimations);

		printf("load scene success.\n");
		directory = path.substr(0,path.find_last_of('/'));
		printf("directory:%s\n",directory.c_str());
		//printf("scene �� mesh ������Ϊ��%d\n", scene->mNumMeshes);
		printf("\n==========   start Process node   ===========\n");
		

		processNode(scene->mRootNode,scene);
		
		printf("���������н�㣬meshes������һ���� %d ��Ԫ��\n", meshes.size());

		for (int i = 0; i < meshes.size(); i++)
		{
			printf("\n�� %d ������\n", i);
			printf("���еĶ������Ϊ%d��\n", meshes[i].vertices.size());
			printf("���е���������Ϊ%d��\n", meshes[i].indices.size());
			printf("���еĲ��ʸ���Ϊ%d��\n", meshes[i].textures.size());
		}
	}
	//�ݹ麯��������scene�е����нڵ�
	void processNode(aiNode *node , const aiScene *scene){
			//����ڵ����������
			for (int i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.push_back(processMesh(mesh, scene));
				
			}
			//���������ӽڵ�
			for (int i = 0; i < node->mNumChildren; i++)
			{
				processNode(node->mChildren[i], scene);
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
//******************************************************************************************88
			//����
			glm::vec3 norVector;
			norVector.x = mesh->mNormals[i].x;
			norVector.y = mesh->mNormals[i].y;
			norVector.z = mesh->mNormals[i].z;
			vertex.Normal = norVector;
			//��������
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
//******************************************************************************************88
				//assimp����һ��ģ����һ���������а˸���ͬ������
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
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
		/*
		//�������
		if (mesh->mMaterialIndex >= 0)
		{
				//һ������ֻ������һ����������
				aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
				printf("\n----- -------------------------------------------- -----\n");
				printf("----- �����䣺%d -----\n", material->GetTextureCount(aiTextureType_DIFFUSE));
				printf("----- ���棺%d -----\n", material->GetTextureCount(aiTextureType_SPECULAR));
				printf("----- ������%d -----\n", material->GetTextureCount(aiTextureType_AMBIENT));
				printf("----- ���䣺%d -----\n", material->GetTextureCount(aiTextureType_EMISSIVE));
				printf("----- ���ߣ�%d -----\n", material->GetTextureCount(aiTextureType_NORMALS));
				printf("----- ��λ���滻��%d -----\n", material->GetTextureCount(aiTextureType_DISPLACEMENT));//��λ���滻
				printf("----- ���գ�%d -----\n", material->GetTextureCount(aiTextureType_LIGHTMAP));//����
				printf("----- �ߣ���%d -----\n", material->GetTextureCount(aiTextureType_HEIGHT));
				printf("----- ��͸���ȣ�%d -----\n", material->GetTextureCount(aiTextureType_OPACITY));//��͸����
				printf("----- shininess��%d -----\n", material->GetTextureCount(aiTextureType_SHININESS));//shininess
				printf("----- ���䣺%d -----\n", material->GetTextureCount(aiTextureType_REFLECTION));
				printf("----- δ֪��%d -----\n", material->GetTextureCount(aiTextureType_UNKNOWN));
				printf("----- NONE��%d -----\n", material->GetTextureCount(aiTextureType_NONE));
				printf("\n----- -------------------------------------------- -----\n");
				vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
				//��textures��ĩβ����diffuseMaps������Ԫ��
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
				vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_diffuse");
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}
		*/
		return Mesh(vertices,indices,textures);
	}

	vector<Texture> loadMaterialTextures(aiMaterial *material , aiTextureType type,string typeName){
		vector<Texture> textures;
		//printf("----- %s -----\n",typeName.c_str());
		
		for (int i = 0; i < material->GetTextureCount(type); i++)
		{	
			aiString texPath;
			material->GetTexture(type, i, &texPath);
			bool skip = false;
			for (int j = 0; j < textures_loaded.size(); j++)
			{
				//strcmp �Ƚ��ַ���
				if (std::strcmp(textures_loaded[j].path.C_Str(), texPath.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				//printf("����ͼƬ���ƣ�%s\n",str.C_Str());
				Texture texture;
				texture.id = TextureFromFile(texPath.C_Str(), directory);
				texture.type = typeName;
				texture.path = texPath;
				textures.push_back(texture);
				textures_loaded.push_back(texture);
			}
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
	//printf("��ɫͨ��������%d\n", channels);
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