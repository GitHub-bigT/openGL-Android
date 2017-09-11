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
	//构造函数
	Model(char* path){
		this->loadModel(path);
	}
	//遍历所有网格  绘制
	void Draw(Shader shader){
		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i].Draw(shader);
		}
	}
private:
	//模型数据
	vector<Mesh> meshes;
	string directory;

	void loadModel(string path){
		//加载模型到assimp的scene模型中。scene是assimp的根对象
		Assimp::Importer importer;
		//aiProcess_Triangulate如果模型不是（不全是）三角形构成，则需要将模型所有的图元形状变换为三角形
		//aiProcess_FlipUVs 因为在纹理中，opengl大部分图像的Y轴都是反的。所以这个选项是翻转y轴的纹理坐标
		//aiProcess_GenNormals 如果模型不包含法线的话，则为每个顶点生成法线
		const aiScene *scene = importer.ReadFile(path,aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
		//检查scene和根节点是否为空，检查scene的一个标记：查看返回的数据是不是不完整的
		if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
		{
			cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
			return;
		}
		directory = path.substr(0,path.find_last_of('/'));

		printf("scene 中 mesh 的数量为：%d\n", scene->mNumMeshes);
		processNode(scene->mRootNode,scene);
	}

	//递归函数。处理scene中的所有节点
	void processNode(aiNode *node , const aiScene *scene){
		printf("node 中 mesh 的数量为：%d\n",node->mNumMeshes);
		//printf("node 中 节点 的数量为：%d\n", node->mNumChildren);
		//处理节点的所有网格
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh,scene));
		}
		//遍历所有子节点
		for (int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i],scene);
		}
	}

	//将assimp中的数据解析到Mesh类中
	Mesh processMesh(aiMesh *mesh , const aiScene *scene){
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		//处理顶点
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			//处理顶点坐标
			glm::vec3 posVector;
			posVector.x = mesh->mVertices[i].x;
			posVector.y = mesh->mVertices[i].y;
			posVector.z = mesh->mVertices[i].z;
			vertex.Position = posVector;
			//法线
			glm::vec3 norVector;
			norVector.x = mesh->mNormals[i].x;
			norVector.y = mesh->mNormals[i].y;
			norVector.z = mesh->mNormals[i].z;
			vertex.Normal = norVector;
			//纹理坐标
			if (!mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				//assimp允许一个模型在一个顶点上有八个不同的坐标
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

		//处理索引
		//索引在face中
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}
		//处理材质
		
		printf("材质数量%d\n", mesh->mMaterialIndex);
		if (mesh->mMaterialIndex >= 0)
		{
			//一个网格只包含了一个材质索引
			aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
			vector<Texture> diffuseMaps = loadMaterialTextures(material,aiTextureType_DIFFUSE,"texture_diffuse");
			//在textures的末尾加入diffuseMaps的所有元素
			textures.insert(textures.end(),diffuseMaps.begin(),diffuseMaps.end());
			vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_diffuse");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}
		
		return Mesh(vertices,indices,textures);
	}
	vector<Texture> loadMaterialTextures(aiMaterial *material , aiTextureType type,string typeName){
		vector<Texture> textures;
		printf("----- %s -----\n",typeName.c_str());
		printf("纹理数量:%d\n", material->GetTextureCount(type));
		for (int i = 0; i < material->GetTextureCount(type); i++)
		{	/*
			aiString str;
			material->GetTexture(type,i,&str);
			printf("纹理图片名称：%s\n",str);
			
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(),directory);
			texture.type = typeName;
			textures.push_back(texture);
			*/
		}
		return textures;
	}

};
//工具函数  加载一个纹理 并返回id
int TextureFromFile(const char* path, const string &direction, bool gamma){
	string fileName = string(path);
	fileName = direction + '/' + fileName;

	unsigned int textureId;
	glGenTextures(1,&textureId);
	int width, height, channels;
	unsigned char* data = stbi_load(fileName.c_str(),&width,&height,&channels,0);
	printf("颜色通道数量：%d\n", channels);
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