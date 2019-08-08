#version 330 core
out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D floorTexture;
uniform sampler2D shadowMap;

uniform float cosInConeAngle;
uniform float cosOutConeAngle;
uniform vec3 coneDirection;
uniform vec3 lightPos;
uniform vec3 lightColor;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // ִ��͸�ӳ���
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // �任��[0,1]�ķ�Χ
    projCoords = projCoords * 0.5 + 0.5;
    // ȡ�����������(ʹ��[0,1]��Χ�µ�fragPosLight������)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // ȡ�õ�ǰƬԪ�ڹ�Դ�ӽ��µ����
    float currentDepth = projCoords.z;
    // ��鵱ǰƬԪ�Ƿ�����Ӱ��
	float bias = 0.005f;// �����Ӱʧ������
    float shadow = currentDepth - bias > closestDepth  ? 0.5 : 0.0;

    return shadow;
}

void main()
{           
    vec3 textureColor = texture(floorTexture, fs_in.TexCoords).rgb;
	float distance = length(lightPos - fs_in.FragPos);
	float attenuation = 1.0 / (1.0f + 0.014 * distance + 0.00007 * (distance * distance));// ˥������

	vec3 lightDir = normalize(fs_in.FragPos - lightPos);
	float theta = dot(lightDir, normalize(coneDirection - lightPos));
	float epsilon  = cosInConeAngle - cosOutConeAngle;
	float intensity = clamp((theta - cosOutConeAngle) / epsilon, 0.0, 0.75); 

	vec3 ambient;

	if(theta > cosOutConeAngle)
		ambient = 0.4 * (textureColor + lightColor) * intensity + 0.25 * textureColor; // ׵����
	else
		ambient = 0.25 * textureColor;

	float shadow = ShadowCalculation(fs_in.FragPosLightSpace);  
	if(shadow > 0)
		ambient = ambient = 0.25 * textureColor;//0.3 * (textureColor + lightColor) * intensity + 0.25 * textureColor;// ��Ӱ��
	
	//vec3 ambient;
	//if(theta > cosInConeAngle)// cosԽ�󣬽Ƕ�ԽС��
	//	ambient = 0.8 * (textureColor + lightColor) * attenuation;
	//else if(theta > cosOutConeAngle)
	//	ambient = 0.4 * (textureColor + lightColor) * attenuation;
	//else
	//	ambient = 0.1 * textureColor * attenuation * intensity;

	FragColor = vec4(ambient, 1.0f);
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);

	return;
}