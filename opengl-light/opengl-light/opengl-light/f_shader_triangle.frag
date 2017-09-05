#version 430 core

in vec3 Normal;

out vec4 oColor;

//片段的位置向量
in vec3 fragPos;
//观察向量
uniform vec3 viewPos;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;
struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

void main(){
	//(世界空间中计算冯氏光照)
	//环境光强度
	vec3 ambient = light.ambient * material.ambient;

	//漫反射
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm , lightDir),0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	//镜面光照
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	//vec3 result = (ambient + diffuse + specular) * objectColor;
	vec3 result = ambient + diffuse + specular;

	oColor = vec4(result,1.0f);
}
