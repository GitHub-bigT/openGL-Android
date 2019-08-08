#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

//uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDepth;
uniform sampler2D depthMap;

uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform mat4 lightSpaceMatrix;
uniform mat4 invView;
uniform mat4 invProjection;

uniform float coneAngle;
uniform vec3 coneDirection;

//#define STEPLEN  0.1
//#define G_SCATTERING 0.3
//#define NUM_SAMPLES 512

#define PI 3.14159265358979
#define NUM_SAMPLES_RCP 0.001953125
#define PI_RCP 0.31830988618379067
//vec3 coneDirection = normalize( vec3(-2.2f, -2.0f, -2.0f) );
//vec3 coneDirection = normalize( vec3(-10.0f, 0.0f, 0.0f) );
//float coneAngle = 10.0f;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 FragPos)
{
	// 做透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// 深度贴图的深度在0到1的范围， 所以需要将坐标从[-1,1] 转换到 [0,1]
    projCoords = projCoords * 0.5 + 0.5;
	// 拿当前片元在光源下的深度信息和z-buffer中的深度信息（距离光源最近的深度值）做比较
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
	float shadow = currentDepth < closestDepth  ? 1.0 : 0.0;

    //float shadow = 1.0;
    //if(projCoords.z > closestDepth)
    //    shadow = 0.0;
        
    return shadow;
}

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

//float ComputeScattering(float lightDotView)
//{
//	float result = 3*(1.0f - G_SCATTERING*G_SCATTERING);
//	result /= (4.0f * PI * pow(1.0f + G_SCATTERING * G_SCATTERING - (2.0f * G_SCATTERING) *  ((lightDotView)), 1.5f));
//	return result;
//}

//void iSphere(in vec3 ro, in vec3 rd, in vec3 sph, in float rad, out vec3 iP0, out vec3 iP1, out float iD0, out float iD1) {
//	iP0 = vec3(0.0, 0.0, 0.0);
//	iP1 = vec3(0.0, 0.0, 0.0);
// 	vec3 oc = ro - sph;
//    float b = dot(oc, rd);
//    float c = dot(oc, oc) - rad*rad;
//    float t = sqrt(b*b - c);
//    if( t > 0.0){ 
//		iD0 = -b - t;
//		iD1 = -b + t;
//        iP0 = ro + iD0*rd;
//		iP1 = ro + iD1*rd;
//    }
//
//}

vec3 evaluateLight(in vec3 pos)
{
    vec3 L = lightPos-pos;
    float distanceToL = length(L);
    return lightColor * 1.0/(distanceToL*distanceToL);
	//return vec3(10.0f, 100.0f, 0.0f) * 1.0/(distanceToL*distanceToL);
}
//vec3 evaluateLight(in vec3 pos, in vec3 normal)
//{
//   // vec3 lightPos = LPOS;
//    vec3 L = lightPos-pos;
//    float distanceToL = length(L);
//    vec3 Lnorm = L/distanceToL;
//    return max(0.0,dot(normal,Lnorm)) * evaluateLight(pos);
//}


vec3 evaluateSpotLight(in vec3 pos, in vec3 normal)
{
   // vec3 lightPos = LPOS;
    vec3 L = lightPos-pos;
	float distanceToL = length(L);
    vec3 Lnorm = L/distanceToL;
	vec3 rayDirection = -Lnorm;
	float lightToSurfaceAngle = degrees(acos(dot(rayDirection, coneDirection)));
	if(lightToSurfaceAngle < coneAngle)
		return max(0.0,dot(normal,Lnorm)) * evaluateLight(pos);
	return vec3(0,0,0);
}



float phaseFunction()
{
    return 1.0/(4.0*3.14);
}



void main()
{

	float Depth = texture(gDepth, TexCoords).r;// 得到每个坐标的深度
    vec3 Normal = texture(gNormal, TexCoords).rgb;// 得到每个坐标的法线

	// 计算当前片元的世界坐标
	vec4 clipSpacePosition = vec4(TexCoords* 2.0 - 1.0, Depth * 2.0 - 1.0, 1.0);// 转换到裁剪空间坐标系
	vec4 viewSpacePosition = invProjection * clipSpacePosition;// 转换到视图空间坐标系
	viewSpacePosition /= viewSpacePosition.w;
	vec3 FragPos = (invView * viewSpacePosition).xyz;// 转换到世界空间坐标系

	// 通过camera坐标系下的坐标转换到light坐标系下，然后进行深度比较
	//vec4 curPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
	//float shadow =  ShadowCalculation(curPosLightSpace) ;// 阴影计算
	//if(shadow > 0.1)
	//	FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);// 在阴影中
	//else
	//	FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	//return;

	//FragColor = vec4(Normal, 1.0f);
	//return;

	vec3 marchingRay = viewPos - FragPos ;// 苹果指向摄像机的向量
	float marchingRayLength = trunc( length(marchingRay) );// 苹果指向摄像机的向量长度
	float stepSize = marchingRayLength * NUM_SAMPLES_RCP;// 步长0.002
	vec3 marchingRayDir = normalize(marchingRay);
	float VLI = 0.0;
	float transmittance = 1;// 透射比,透光率
	vec3 curPos = FragPos;
	float muS = 0.01;
	for(float l = marchingRayLength; l > stepSize; l -= stepSize)
	{
		curPos += stepSize * marchingRayDir;// 苹果从世界坐标位置一直加到摄像机的位置

		// 把苹果的世界坐标转换到光照下的坐标
		// applePos * lightView * lightProj
		vec4 curPosLightSpace = lightSpaceMatrix * vec4(curPos, 1.0);
		float shadowTerm =  ShadowCalculation(curPosLightSpace, curPos) ;// 阴影计算,no

		float d = length(curPos- lightPos);// 光指向物体的向量长度
		float dRcp = 1.0/d;// 倒数，reciprocal

		// 锥体
		vec3 rayDirection = (curPos-lightPos)*dRcp;
		float lightToSurfaceAngle = degrees(acos(dot(rayDirection, coneDirection)));// 算夹角
		if(lightToSurfaceAngle > coneAngle)
			shadowTerm = 0.0;

		float intens = 10000.0*muS*(shadowTerm* ( 0.25 * PI_RCP) * dRcp * dRcp ) * transmittance *stepSize;
		transmittance *= exp(-muS * stepSize);
		VLI += intens;
	}

	//if(intSphere0 != intSphere1)
		//FragColor = vec4(color + vec3(0.1,0.0,0.0),1.0);
	//else
	//vec3 SurfaceColor = evaluateLight(FragPos, Normal);

	vec3 SurfaceColor = evaluateSpotLight(FragPos, Normal);

	//FragColor = vec4(SurfaceColor ,1.0); 
	//return;

	vec3 gammacolor = vec3(VLI,0,0) + SurfaceColor;
	FragColor = vec4(pow(gammacolor, vec3(1.0/2.2)) ,1.0); 

	//FragColor = vec4(Normalee.r,Normalee.r,Normalee.r,1.0);
	//FragColor = vec4(-dot(marchingRayDir, lightDir),-dot(marchingRayDir, lightDir),-dot(marchingRayDir, lightDir), 1.0);
} 
