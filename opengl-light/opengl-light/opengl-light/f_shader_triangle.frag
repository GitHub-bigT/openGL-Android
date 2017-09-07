#version 430 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
	//点光源
	//vec3 position;
	//定向光：不需要position 无限远
	//vec3 direction;
	//聚光
	vec3 position;
	vec3 direction;
	float cutoff; //内切光角
	float othercutoff; //外切光角

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	//衰减值
	float constant;
	float linear;
	float quadratic;
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
	vec3 lightDir = normalize(light.position - fragPos);
	//计算theta角 和 phi角进行比较 看是否在聚光内部
	float theta = dot(lightDir,normalize(-light.direction));
	float epsilon = light.cutoff - light.othercutoff;
	//clamp函数 将第一个参数固定在0-1之间 保证亮度值不会超出[0,1]以外
	//超出0，1之外就是黑色
	float intensity = clamp((theta - light.othercutoff) / epsilon , 0.0f , 1.0f);
	//float intensity = (theta - light.othercutoff) / epsilon ;

	//if(theta > light.cutoff){
		//执行光照计算
		//计算衰减值
		//float distance = length(light.position - fragPos);
		//float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

		//(世界空间中计算冯氏光照)
		//环境光强度
		vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));

		//漫反射
		vec3 norm = normalize(Normal);
		//light.direction 从光源发出  取反
		//vec3 lightDir = normalize(-light.direction);
		float diff = max(dot(norm , lightDir),0.0);
		vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse , TexCoords));
		//镜面光照
		vec3 viewDir = normalize(viewPos - fragPos);
		vec3 reflectDir = reflect(-lightDir,norm);
		float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
		vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));

		//放射光
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
	//else //否则使用环境光，使得场景不至于完全黑暗
	//oColor = vec4(light.ambient * vec3(texture(material.diffuse,TexCoords)),1.0f);
	//oColor = vec4(0.0f,0.0f,0.0f,1.0f);
}
