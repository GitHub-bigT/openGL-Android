#version 430 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

//ƽ�й⡢�����
struct DirLight{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
//���Դ
struct PointLight{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
//�۹�
struct SpotLight{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;   
};

out vec4 color;
in vec3 fragPos;
in vec2 TexCoords;
in vec3 Normal;
uniform vec3 viewPos;
uniform Material material;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform DirLight dirLight;

//ƽ�й⡢�����
vec3 CalcDirLight(DirLight light , vec3 normal , vec3 viewDir);
//���Դ
vec3 CalcPointLight(PointLight light , vec3 normal, vec3 fragPos , vec3 viewDir);
//�۹�
vec3 CalcSpotLight(SpotLight light , vec3 normal, vec3 fragPos , vec3 viewDir);

//ƽ�й⡢�����
vec3 CalcDirLight(DirLight light , vec3 normal , vec3 viewDir){
	vec3 lightDir = normalize(-light.direction);
	//������ǿ��
	float diff = max(dot(lightDir , normal),0.0f);
	//���淴��ǿ��
	vec3 reflectDir = reflect(-lightDir , normal);
	float spec = pow(max(dot(viewDir , reflectDir) , 0.0f) , material.shininess);
	//�ϲ�
	vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));
	vec3 diffuse = light.diffuse * vec3(texture(material.diffuse,TexCoords));
	vec3 specular = light.specular * vec3(texture(material.specular , TexCoords));

	return (ambient + diffuse + specular);
}

//���Դ
vec3 CalcPointLight(PointLight light , vec3 normal , vec3 viewDir , vec3 fragPos){
	vec3 lightDir = normalize(light.position - fragPos);
	//����������ǿ��
	float diff = max(dot(lightDir,normal),0.0f);
	//���㾵�淴��ǿ��
	vec3 reflectDir = reflect(-lightDir , normal);
	float spec = pow(max(dot(reflectDir , viewDir),0.0f),material.shininess);
	//����˥��
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f/(light.constant + light.linear * distance + light.quadratic * (distance * distance));
	//�ϲ�
	vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

//�۹�
vec3 CalcSpotLight(SpotLight light , vec3 normal, vec3 viewDir , vec3 fragPos){
	vec3 lightDir = normalize(light.position - fragPos);
	//����������ǿ��
	float diff = max(dot(lightDir,normal),0.0f);
	//���㾵�淴��ǿ��
	vec3 reflectDir = reflect(-lightDir , normal);
	float spec = pow(max(dot(reflectDir , viewDir),0.0f),material.shininess);
	//����˥��
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f/(light.constant + light.linear * distance + light.quadratic * (distance * distance));  
    //ƽ��
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    //�ϲ�
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
}

void main(){
	//һЩ��Ҫ������
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - fragPos);
	
	//1.����ƽ�й���
	vec3 result = CalcDirLight(dirLight,norm,viewDir);
	//2.������Դ����
	for(int i = 0 ; i < NR_POINT_LIGHTS ; i++)
	{
		result += CalcPointLight(pointLights[i],norm,viewDir,fragPos);
	}
	//3.����۹����
	result += CalcSpotLight(spotLight,norm,viewDir,fragPos);

	//float theta = dot(normalize(spotLight.position - fragPos), normalize(-spotLight.direction)); 
	//if(theta > spotLight.cutOff){
	//	color = vec4(1.0f,0.0f,0.0f,1.0f);
	//}
	//else
	//color = vec4(0.0f,1.0f,0.0f,1.0f);
	color = vec4(result,1.0f);
}
