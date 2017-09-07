#version 430 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
	//���Դ
	//vec3 position;
	//����⣺����Ҫposition ����Զ
	//vec3 direction;
	//�۹�
	vec3 position;
	vec3 direction;
	float cutoff; //���й��
	float othercutoff; //���й��

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	//˥��ֵ
	float constant;
	float linear;
	float quadratic;
};

out vec4 oColor;
//Ƭ�ε�λ������
in vec3 fragPos;
in vec2 TexCoords;
in vec3 Normal;
//�۲�����
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main(){
	vec3 lightDir = normalize(light.position - fragPos);
	//����theta�� �� phi�ǽ��бȽ� ���Ƿ��ھ۹��ڲ�
	float theta = dot(lightDir,normalize(-light.direction));
	float epsilon = light.cutoff - light.othercutoff;
	//clamp���� ����һ�������̶���0-1֮�� ��֤����ֵ���ᳬ��[0,1]����
	//����0��1֮����Ǻ�ɫ
	float intensity = clamp((theta - light.othercutoff) / epsilon , 0.0f , 1.0f);
	//float intensity = (theta - light.othercutoff) / epsilon ;

	//if(theta > light.cutoff){
		//ִ�й��ռ���
		//����˥��ֵ
		//float distance = length(light.position - fragPos);
		//float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

		//(����ռ��м�����Ϲ���)
		//������ǿ��
		vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));

		//������
		vec3 norm = normalize(Normal);
		//light.direction �ӹ�Դ����  ȡ��
		//vec3 lightDir = normalize(-light.direction);
		float diff = max(dot(norm , lightDir),0.0);
		vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse , TexCoords));
		//�������
		vec3 viewDir = normalize(viewPos - fragPos);
		vec3 reflectDir = reflect(-lightDir,norm);
		float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
		vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));

		//�����
		vec3 emission = vec3(texture(material.emission,TexCoords));

		//ambient *= attenuation;
		//diffuse *= attenuation;
		//specular *= attenuation;

		diffuse *= intensity;
		specular *= intensity;

		vec3 result = ambient + diffuse + specular;
		oColor = vec4(result,3.0f);
		//oColor = vec4(-11.0f,-10.0f,-770.0f,1.0f);
	//}
	//else //����ʹ�û����⣬ʹ�ó�����������ȫ�ڰ�
	//oColor = vec4(light.ambient * vec3(texture(material.diffuse,TexCoords)),1.0f);
	//oColor = vec4(0.0f,0.0f,0.0f,1.0f);
}
