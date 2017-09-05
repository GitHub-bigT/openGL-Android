#version 430 core

in vec3 Normal;

out vec4 oColor;

//Ƭ�ε�λ������
in vec3 fragPos;
//�۲�����
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
	//(����ռ��м�����Ϲ���)
	//������ǿ��
	vec3 ambient = light.ambient * material.ambient;

	//������
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm , lightDir),0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	//�������
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	//vec3 result = (ambient + diffuse + specular) * objectColor;
	vec3 result = ambient + diffuse + specular;

	oColor = vec4(result,1.0f);
}
