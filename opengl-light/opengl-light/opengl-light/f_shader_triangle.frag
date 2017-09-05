#version 430 core

in vec3 Normal;

out vec4 oColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
//���λ������
uniform vec3 lightPos;
//Ƭ�ε�λ������
in vec3 fragPos;
//�۲�����
uniform vec3 viewPos;

void main(){

	//������ǿ��
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;
	//vec3 result = ambient * objectColor;

	//������
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm , lightDir),0.0);
	vec3 diffuse = diff * lightColor;

	//�������
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),256);
	vec3 specular = specularStrength * spec * lightColor;
	vec3 result = (ambient + diffuse + specular) * objectColor;
	oColor = vec4(result,1.0f);
}
