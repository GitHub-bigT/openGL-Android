#version 430 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 oColor;
//片段的位置向量
in vec3 fragPos;
in vec2 TexCoords;
in vec3 Normal;
//观察向量
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main(){
	//(世界空间中计算冯氏光照)
	//环境光强度
	vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));

	//漫反射
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm , lightDir),0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse , TexCoords));

	//镜面光照
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));
	//放射光
	vec3 emission = vec3(texture(material.emission,TexCoords));

	vec3 result = ambient + diffuse + specular + emission;

	oColor = vec4(result,1.0f);
	//oColor = vec4(vec3(texture(material.emission,TexCoords)),1.0f);
}
