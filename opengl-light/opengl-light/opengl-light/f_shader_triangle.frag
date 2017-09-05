#version 430 core

in vec3 Normal;

out vec4 oColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
//光的位置向量
uniform vec3 lightPos;
//片段的位置向量
in vec3 fragPos;

void main(){

	//环境光强度
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;
	//vec3 result = ambient * objectColor;
	//漫反射
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm , lightDir),0.0);
	vec3 diffuse = diff * lightColor;
	vec3 result = (ambient + diffuse) * objectColor;
	oColor = vec4(result,1.0f);
}
