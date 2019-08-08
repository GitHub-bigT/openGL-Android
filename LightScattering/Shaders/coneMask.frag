#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

in vec3 CurrentPosInScreenSpace;
in vec3 LightPosInScreenSpace;
in vec3 ConeViewPosInScreenSpace;
in vec4 FragPosLightSpace;

//uniform sampler2D gNormal;
//uniform sampler2D gDepth;
uniform sampler2D depthMap;

uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 coneDirection;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform mat4 lightSpaceMatrix;
uniform mat4 invView;
uniform mat4 invProjection;

uniform float coneAngle;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	// 做透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// 深度贴图的深度在0到1的范围， 所以需要将坐标从[-1,1] 转换到 [0,1]
    projCoords = projCoords * 0.5 + 0.5;
	// 拿当前片元在光源下的深度信息和z-buffer中的深度信息（距离光源最近的深度值）做比较
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
	float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
    return shadow;
}

float near = 0.1; 
float far  = 1000.0;
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

void main()
{
	vec3 currentFragCoord = CurrentPosInScreenSpace;
	//FragColor = vec4(currentFragCoord.x, currentFragCoord.y, 0.0f, 1.0f);
	//return;

	vec3 rayDirectionNor = currentFragCoord - LightPosInScreenSpace;// 光照到片元的向量
	vec3 coneDirectionNor = ConeViewPosInScreenSpace - LightPosInScreenSpace;// 光锥的向量

	vec2 rayDirectionNor_2 = normalize(vec2(rayDirectionNor.x, rayDirectionNor.y));
	vec2 coneDirectionNor_2 = normalize(vec2(coneDirectionNor.x, coneDirectionNor.y));
	//float debugValue = acos(dot(rayDirectionNor_2, coneDirectionNor_2)) / 3.1415926;
	//FragColor = vec4(debugValue, debugValue, debugValue, 1.0f);

	float lightToSurfaceAngle = degrees(acos(dot(rayDirectionNor_2, coneDirectionNor_2)));// 算夹角

	//float closestDepth = texture(shadowMap, projCoords.xy).r; 
	//float shadow = ShadowCalculation(FragPosLightSpace);
	//FragColor = vec4(shadow, shadow, shadow, 1.0f);

	if(lightToSurfaceAngle > coneAngle)
		FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	else
		FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);

	return;

	// 深度可视化
	//float depth = LinearizeDepth(texture(gDepth, TexCoords).r) / far;
	//FragColor = vec4(vec3(depth), 1.0);
	////color = vec4(vec3(texture(depthTexture, TexCoords).r), 1.0);
	//return;

	//float Depth = texture(gDepth, TexCoords).r;// 得到每个坐标的深度
	//vec4 clipSpacePosition = vec4(TexCoords* 2.0 - 1.0, Depth * 2.0 - 1.0, 1.0);
	//vec4 viewSpacePosition = invProjection * clipSpacePosition;
	//viewSpacePosition /= viewSpacePosition.w;
	//vec3 FragPos = (invView * viewSpacePosition).xyz;// 转换到世界空间坐标系
	//
	//vec3 rayDirection = normalize(FragPos-lightPos);// 光照到片元的向量
	//vec3 coneDirectionNor = normalize(coneDirection-lightPos);// 光锥的向量
	//float debugValue = acos(dot(rayDirection, coneDirectionNor)) / 3.1415936;
	//FragColor = vec4(LightPos.rgb, 1.0f);
	//return;

	//float lightToSurfaceAngle = degrees(acos(dot(rayDirection, coneDirectionNor)));// 算夹角
	//if(lightToSurfaceAngle > 10)
	//	FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	//else
	//	FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
} 
