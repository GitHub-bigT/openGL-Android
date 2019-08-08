#version 330 core
in vec2 TexCoords;
out vec4 finalColor;

uniform mat4 lightSpaceMatrix;
uniform mat4 screenSpaceMatrix;
uniform mat4 viewInv;
uniform mat4 projInv;
uniform mat4 cameraProj;

uniform float coneAngle;
uniform vec3 coneDirection;

uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform vec4 rayleighBeta;
uniform vec4 mieBeta;
uniform vec4 summTotalBeta;
uniform float m_fMaxTracingDistance;
uniform vec4 m_f4ZBufferParams;
uniform vec4 f4SpotLightAxisAndCosAngle;

uniform sampler2D screenTexture;
uniform sampler2D screenDepth;
uniform sampler2D depthMap;
uniform sampler2D preComputeTexture;

float near = 0.1; 
float far  = 1000.0;
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

//Z buffer to linear 0..1 depth
float Linear01Depth( float z )
{
	return 1.0f / (m_f4ZBufferParams.x * z + m_f4ZBufferParams.y);
}
// Z buffer to linear depth
float LinearEyeDepth( float z )
{
	return 1.0f / (m_f4ZBufferParams.z * z + m_f4ZBufferParams.w);
}

vec3 getCurrentFsWorldPos()
{
	vec4 worldPos;

	float Depth = texture(screenDepth, TexCoords).r;
	//float fDepth = LinearizeDepth(Depth);//LinearizeDepth(Depth);//cameraProj[2][2] + cameraProj[2][3] / Depth;
	vec4 clipSpacePosition = vec4(TexCoords * 2.0 - 1.0, Depth * 2.0 - 1.0, 1.0);

	vec4 viewSpacePosition = viewInv * projInv * clipSpacePosition;
	worldPos = viewSpacePosition / viewSpacePosition.w;

	//vec4 f4WorldPos = viewInv * projInv * clipSpacePosition;
	//worldPos = (f4WorldPos / f4WorldPos.w).xyz;

	return worldPos.xyz;
}

float CalculateShadow(vec4 fragPosLightSpace)
{
    // 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;
    // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    // 取得当前片元在光源视角下的深度
    float currentDepth = projCoords.z;
    // 检查当前片元是否在阴影中
	float bias = 0.005f;// 解决阴影失真问题
    float shadow = currentDepth - bias > closestDepth  ? 0.5 : 0.0;

    return shadow;
}

#define PI 3.14159265358979
#define PI_RCP 0.31830988618379067
#define NUM_SAMPLES_RCP 0.0001
#define RAYLEIGHT 1

// https://github.com/ucanbizon/volumelight
vec4 CalculateInscatteringRayMarching1()
{
	vec4 color;

	vec3 worldPos = getCurrentFsWorldPos();
	vec3 marchingRay = viewPos - worldPos ;
	float marchingRayLength = trunc( length(marchingRay) );
	float stepSize = marchingRayLength * NUM_SAMPLES_RCP;
	vec3 marchingRayDir = normalize(marchingRay);
	float VLI = 0.0;
	vec3 curPos = worldPos;
	float muS = 0.01;

	for(float l = marchingRayLength; l > stepSize; l -= stepSize)
	{
		curPos += stepSize * marchingRayDir;

		vec4 curPosLightSpace = lightSpaceMatrix * vec4(curPos, 1.0);
		float shadowTerm = 1;//ShadowCalculation(curPosLightSpace, curPos) ;// ÒõÓ°¼ÆËã,no

		float d = length(curPos- lightPos);// L -> P
		float dRcp = 1.0/d;// µ¹Êý£¬reciprocal

		// ×¶Ìå
		vec3 rayDirection = normalize(curPos-lightPos);
		float lightToSurfaceAngle = degrees(acos(dot(rayDirection, normalize(coneDirection - lightPos))));
		if(lightToSurfaceAngle > coneAngle)
			shadowTerm = 0.0;

		float intens = 10000.0*muS*(shadowTerm* ( 0.25 * PI_RCP) * dRcp * dRcp ) * stepSize;
		VLI += intens;
	}
	color = vec4(VLI * lightColor, 1.0f);

	return color;
}

vec4 CalculateOpticalDepthWithRayleight(float length)
{
	vec4 opticalDepth;

	opticalDepth = rayleighBeta * length;

	return opticalDepth;
}

float CalculatePhasefunctionWithRayleight()
{
	return 0.25 * PI_RCP;// 决定外散射到摄像机的能量大小的相位函数(各向同性)
}

vec4 CalculateOpticalDepthWithMie(float length)
{
	vec4 opticalDepth;

	opticalDepth = mieBeta * length;

	return opticalDepth;
}

vec4 CalculateOpticalDepth(float length)
{
	if(RAYLEIGHT == 1)
	{
		return CalculateOpticalDepthWithRayleight(length);
	}
	else
	{
		return CalculateOpticalDepthWithMie(length);
	}
}

vec4 CalculatePhasefunction(float cosTheta)// 决定外散射到摄像机的能量大小的相位函数
{
	//    sun
    //      \
    //       \
    //    ----\------eye
    //         \theta 
    //          \
    //  
	if(RAYLEIGHT == 1)
	{
		return vec4(0.25*PI_RCP, 0.25*PI_RCP, 0.25*PI_RCP, 0.25*PI_RCP);// * rayleighBeta;//各向同性
	}
	else
	{
		float g = 0.99;//-0.75f;
		return vec4(0.25*PI_RCP,0.25*PI_RCP,0.25*PI_RCP,0.25*PI_RCP) * 1.5f * (1.0f-g*g) * (1.0f+cosTheta*cosTheta) / ((2.0f + g*g) * (pow(3.0f/2.0f, 1.0f+g*g - 2.0f*g*cosTheta)));// * mieBeta;//各向异性
	}
}

vec4 CalculateInscatteringRayMarchingIntel1()//无简化公式, 0.0002 453.687ms 2.2fps
{
	vec4 color;

	float radiantIntensity = 12.0f;// 光强
	float visible = 1;// V(P)

	vec3 worldPos = getCurrentFsWorldPos();
	vec3 marchingRay = viewPos - worldPos ;
	float marchingRayLength = trunc( length(marchingRay) );
	float stepSize = marchingRayLength * NUM_SAMPLES_RCP;
	vec3 marchingRayDir = normalize(marchingRay);
	vec4 inscatteringIntergral;
	vec3 curWorldPos = worldPos;

	for(float l = marchingRayLength; l > stepSize; l -= stepSize)
	{
		curWorldPos += stepSize * marchingRayDir;

		vec4 curPosLightSpace = lightSpaceMatrix * vec4(curWorldPos, 1.0);
		visible = 1.0f - CalculateShadow(curPosLightSpace); 

		float distLtoP = length(curWorldPos - lightPos);
		float distLtoP_Rcp = 1.0/distLtoP;

		float distPtoC = length(curWorldPos - viewPos);

		vec3 rayDirection = normalize(curWorldPos - lightPos);
		float lightToSurfaceAngle = degrees(acos(dot(rayDirection, normalize(coneDirection - lightPos))));
		if(lightToSurfaceAngle > coneAngle)
			visible = 0.0;

		float cosTheta = dot(normalize(marchingRay), rayDirection);
		vec4 isotropicPhaseFunction = vec4(1.0f, 1.0f, 1.0f, 1.0f);//CalculatePhasefunction(cosTheta);

		vec4 attenuatedFactorLtoP = exp(-CalculateOpticalDepth(distLtoP));
		vec4 attenuatedFactorPtoC = exp(-CalculateOpticalDepth(distPtoC));

		vec4 inscatteringP = attenuatedFactorPtoC * (isotropicPhaseFunction * visible) * radiantIntensity * attenuatedFactorLtoP * distLtoP_Rcp*distLtoP_Rcp * stepSize;

		inscatteringIntergral += inscatteringP;
	}

	color = inscatteringIntergral * vec4(lightColor, 1.0f);

	return color;
}

vec4 CalculateInscatteringRayMarchingIntel2()//简化公式, 0.0002 
{
	vec4 color;

	float radiantIntensity = 12.0f;// 光强
	float visible = 1;// V(P)

	vec3 worldPos = getCurrentFsWorldPos();
	vec3 marchingRay = viewPos - worldPos ;
	float marchingRayLength = trunc( length(marchingRay) );
	float stepSize = marchingRayLength * NUM_SAMPLES_RCP;
	vec3 marchingRayDir = normalize(marchingRay);
	vec4 inscatteringIntergral;
	vec3 curWorldPos = worldPos;

	//                       Light
    //                        *                   -
    //                     .' |\                  |
    //                   .'   | \                 | object
    //                 .'     |  \                |
    //               .'       |   \               |
    //          Cam *--------------*--------->    -
    //              |<--------|     \
    //                  \
    //                  Sc
	float distCtoL = length(viewPos - lightPos);
	float cosCtoLWithEye = dot(normalize(lightPos - viewPos), -marchingRayDir);
	float Sc = distCtoL * cosCtoLWithEye;
	float h = distCtoL * sqrt(1 - cosCtoLWithEye*cosCtoLWithEye);//sqrt(distCtoL*distCtoL - Sc*Sc);

	for(float l = marchingRayLength; l > stepSize; l -= stepSize)
	{
		curWorldPos += stepSize * marchingRayDir;

		vec4 curPosLightSpace = lightSpaceMatrix * vec4(curWorldPos, 1.0);
		visible = 1.0f - CalculateShadow(curPosLightSpace);//阴影 
		vec3 rayDirection = normalize(curWorldPos - lightPos);
		float lightToSurfaceAngle = degrees(acos(dot(rayDirection, normalize(coneDirection - lightPos))));
		if(lightToSurfaceAngle > coneAngle)
			visible = 0.0;//是否在椎体内

		float distLtoP = length(curWorldPos - lightPos);
		float s = sqrt(distLtoP*distLtoP - h*h);
		float hsSquareRcp = 1 / (h*h + s*s);

		inscatteringIntergral += radiantIntensity * visible * exp(-rayleighBeta * (s+Sc+sqrt(h*h + s*s))) * hsSquareRcp * stepSize;

		//float cosTheta = dot(normalize(marchingRay), rayDirection);
		//vec4 isotropicPhaseFunction = vec4(1.0f, 1.0f, 1.0f, 1.0f);//CalculatePhasefunction(cosTheta);

		//vec4 attenuatedFactorLtoP = exp(-CalculateOpticalDepth(distLtoP));
		//vec4 attenuatedFactorPtoC = exp(-CalculateOpticalDepth(distPtoC));

		//vec4 inscatteringP = attenuatedFactorPtoC * (isotropicPhaseFunction * visible) * radiantIntensity * attenuatedFactorLtoP * distLtoP_Rcp*distLtoP_Rcp * stepSize;

		//inscatteringIntergral += inscatteringP;
	}

	color = inscatteringIntergral * vec4(lightColor, 1.0f);

	return color;
}

float GetPrecomputedPtLghtSrcTexU(vec3 f3Pos, vec3 f3EyeDir, vec3 f3ClosestPointToLight)
{
    return (dot(f3Pos - f3ClosestPointToLight, f3EyeDir) + m_fMaxTracingDistance) / (2*m_fMaxTracingDistance);
};

#define FLT_MAX 3.402823466e+38f
vec2 RayConeIntersect(in vec3 f3ConeApex, in vec3 f3ConeAxis, in float fCosAngle, in vec3 f3RayStart, in vec3 f3RayDir)
{
    f3RayStart -= f3ConeApex;
    float a = dot(f3RayDir, f3ConeAxis);
    float b = dot(f3RayDir, f3RayDir);
    float c = dot(f3RayStart, f3ConeAxis);
    float d = dot(f3RayStart, f3RayDir);
    float e = dot(f3RayStart, f3RayStart);
    fCosAngle *= fCosAngle;
    float A = a*a - b*fCosAngle;
    float B = 2 * ( c*a - d*fCosAngle );
    float C = c*c - e*fCosAngle;
    float D = B*B - 4*A*C;
    if( D > 0 )
    {
        D = sqrt(D);
        vec2 t = (-B + sign(A)*vec2(-D,+D)) / (2*A);
        bvec2 b2IsCorrect = bvec2(c + a * t.x > 0, c + a * t.y > 0);

		float tx = 0.0f;
		float ty = 0.0f;
		if(b2IsCorrect.x)
		{
			tx = 1.0f;
		}

		if(b2IsCorrect.y)
		{
			ty = 1.0f;
		}

		float ttx = 0.0f;
		float tty = 0.0f;
		if(!b2IsCorrect.x)
		{
			ttx = 1.0f;
		}

		if(!b2IsCorrect.y)
		{
			tty = 1.0f;
		}

        t = t * vec2(tx, ty) + vec2(ttx, tty) * (-FLT_MAX);
        return t;
    }
    else
        return vec2(-FLT_MAX, -FLT_MAX);
}

void TruncateEyeRayToLightCone(in vec3 f3EyeVector, 
                               inout vec3 f3RayStartPos, 
                               inout vec3 f3RayEndPos, 
                               inout float fTraceLength, 
                               out float fStartDistance,
                               bool bIsCamInsideCone)
{
    // Intersect view ray with the light cone
    vec2 f2ConeIsecs = 
    	RayConeIntersect(lightPos, f4SpotLightAxisAndCosAngle.xyz, f4SpotLightAxisAndCosAngle.w, viewPos, f3EyeVector);
        //RayConeIntersect(g_LightAttribs.f4LightWorldPos.xyz, g_LightAttribs.f4SpotLightAxisAndCosAngle.xyz, g_LightAttribs.f4SpotLightAxisAndCosAngle.w,
        //                 g_CameraAttribs.f4CameraPos.xyz, f3EyeVector);
    
    if( bIsCamInsideCone  )
    {
        f3RayStartPos = viewPos;//g_CameraAttribs.f4CameraPos.xyz;
        fStartDistance = 0;
        if( f2ConeIsecs.x > 0 )
        {
            // 
            //   '.       *     .' 
            //     '.      \  .'   
            //       '.     \'  x > 0
            //         '. .' \
            //           '    \ 
            //         '   '   \y = -FLT_MAX 
            //       '       ' 
            fTraceLength = min(f2ConeIsecs.x, fTraceLength);
        }
        else if( f2ConeIsecs.y > 0 )
        {
            // 
            //                '.             .' 
            //    x = -FLT_MAX  '.---*---->.' y > 0
            //                    '.     .'
            //                      '. .'  
            //                        '
            fTraceLength = min(f2ConeIsecs.y, fTraceLength);
        }
        f3RayEndPos = viewPos + fTraceLength * f3EyeVector;//g_CameraAttribs.f4CameraPos.xyz + fTraceLength * f3EyeVector;
    }
    else if( f2ConeIsecs.x > 0 && f2ConeIsecs.y > 0 )
    {
        // 
        //          '.             .' 
        //    *-------'.-------->.' y > 0
        //          x>0 '.     .'
        //                '. .'  
        //                  '
        fTraceLength = min(f2ConeIsecs.y,fTraceLength);
        f3RayEndPos   = viewPos + fTraceLength * f3EyeVector;
        f3RayStartPos = viewPos + f2ConeIsecs.x * f3EyeVector;
        fStartDistance = f2ConeIsecs.x;
        fTraceLength -= f2ConeIsecs.x;
    }
    else if( f2ConeIsecs.y > 0 )
    {
        // 
        //   '.       \     .'                '.         |   .' 
        //     '.      \  .'                    '.       | .'   
        //       '.     \'  y > 0                 '.     |'  y > 0
        //         '. .' \                          '. .'| 
        //           '    *                           '  |   
        //         '   '   \x = -FLT_MAX            '   '|   x = -FLT_MAX 
        //       '       '                        '      |' 
        //                                               *
        //
        f3RayEndPos   = viewPos + fTraceLength * f3EyeVector;
        f3RayStartPos = viewPos + f2ConeIsecs.y * f3EyeVector;
        fStartDistance = f2ConeIsecs.y;
        fTraceLength -= f2ConeIsecs.y;
    }
    else
    {
        fTraceLength = 0;
        fStartDistance = 0;
        f3RayStartPos = viewPos;
        f3RayEndPos   = viewPos;
    }
    fTraceLength = max(fTraceLength,0);
}

vec3 ToneMap(in vec3 f3Color)
{
	float m_fExposure = 1.0f;
    float fExposure = m_fExposure;//g_PPAttribs.m_fExposure;
    return 1.0 - exp(-fExposure * f3Color);
}

vec4 CalculateInscatteringRayMarchingIntel3()// LUT
{
	vec4 color;
	vec3 f3ReconstructedPos = getCurrentFsWorldPos();//ProjSpaceXYToWorldSpace(f2RayMarchingSampleLocation);

    vec3 f3RayStartPos = viewPos;//g_CameraAttribs.f4CameraPos.xyz;// C
    vec3 f3RayEndPos = f3ReconstructedPos;// O
    vec3 f3EyeVector = f3RayEndPos.xyz - f3RayStartPos;// C->O
    float fTraceLength = length(f3EyeVector);// CO长度
    f3EyeVector = normalize(f3EyeVector);///= fTraceLength;

    //                       Light
    //                        *                   -
    //                     .' |\                  |
    //                   .'   | \                 | fClosestDistToLight
    //                 .'     |  \                |
    //               .'       |   \               |
    //          Cam *--------------*--------->    -
    //              |<--------|     \
    //                  \
    //                  fStartDistFromProjection

    float fDistToLight = length(lightPos - viewPos);//( g_LightAttribs.f4LightWorldPos.xyz - g_CameraAttribs.f4CameraPos.xyz );// CL长度
    float fCosLV = dot(normalize(lightPos - viewPos), f3EyeVector);//(g_LightAttribs.f4DirOnLight.xyz, f3EyeVector);// CL CO夹角
    float fDistToClosestToLightPoint = fDistToLight * fCosLV;// P0到C的长度
    float fClosestDistToLight = fDistToLight * sqrt(1 - fCosLV*fCosLV);// L到P0的长度
    float fV = fClosestDistToLight / m_fMaxTracingDistance;// v坐标 = L到P0的长度 / max
    
    vec3 f3ClosestPointToLight = viewPos + f3EyeVector * fDistToClosestToLightPoint;// P0点的世界坐标

    float fCameraU = GetPrecomputedPtLghtSrcTexU(viewPos, f3EyeVector, f3ClosestPointToLight);// u坐标 = （max +- P0到C的长度的长度）/ 2max
    float fReconstrPointU = GetPrecomputedPtLghtSrcTexU(f3ReconstructedPos, f3EyeVector, f3ClosestPointToLight);

	vec3 f3CameraInsctrIntegral = vec3(0.0f,0.0f,0.0f);
    vec3 f3RayTerminationInsctrIntegral = vec3(0.0f,0.0f,0.0f);
    f3CameraInsctrIntegral = texture(preComputeTexture, vec2(fCameraU, fV)).rgb;//g_tex2DPrecomputedPointLightInsctr.SampleLevel(samLinearClamp, vec2(fCameraU, fV), 0);
    // OC的衰减？？？？
    f3RayTerminationInsctrIntegral = exp(-fTraceLength*summTotalBeta.rgb) * texture(preComputeTexture, vec2(fReconstrPointU, fV)).rgb;//g_tex2DPrecomputedPointLightInsctr.SampleLevel(samLinearClamp, vec2(fReconstrPointU, fV), 0);

	//float visible = 1;// V(P)
	//vec4 curPosLightSpace = lightSpaceMatrix * vec4(f3ReconstructedPos, 1.0);
	//	visible = 1.0f - CalculateShadow(curPosLightSpace); 
	//vec3 rayDirection = normalize(f3ReconstructedPos - lightPos);
	//float lightToSurfaceAngle = degrees(acos(dot(rayDirection, normalize(coneDirection - lightPos))));
	//if(lightToSurfaceAngle > coneAngle)
	//	visible = 0.0;

 	vec3 f3FullyLitInsctrIntegral = (f3CameraInsctrIntegral - f3RayTerminationInsctrIntegral) * lightColor.rgb * 5711.714f;//g_LightAttribs.f4LightColorAndIntensity.w;

 	bool bIsCamInsideCone = dot( -normalize(lightPos - viewPos), f4SpotLightAxisAndCosAngle.xyz) > f4SpotLightAxisAndCosAngle.w;

 	// Eye rays directed at exactly the light source requires special handling
    //if( fCosLV > 1 - 1e-6 )
    //{
    //    f3FullyLitInsctrIntegral = vec3(1e+8, 1e+8, 1e+8);
    //}

 	float fStartDistance;
    TruncateEyeRayToLightCone(f3EyeVector, f3RayStartPos, f3RayEndPos, fTraceLength, fStartDistance, bIsCamInsideCone);

 	if( fTraceLength < m_fMaxTracingDistance * 0.0001)
    {
    	color = vec4(0,0,0,0);
    	return color;
		//color = vec4(f3FullyLitInsctrIntegral, 1.0f);
        //return f3FullyLitInsctrIntegral;
    }

	 color = vec4(f3FullyLitInsctrIntegral, 1.0f);

	// vec4 f4StartUVAndDepthInLightSpace = lightSpaceMatrix * vec4(f3RayStartPos, 1.0);
	// vec4 f4EndUVAndDepthInLightSpace = lightSpaceMatrix * vec4(f3RayEndPos, 1.0);
	// vec3 f3ShadowMapTraceDir = f4EndUVAndDepthInLightSpace.xyz - f4StartUVAndDepthInLightSpace.xyz;
 //    float fTraceLenInShadowMapUVSpace = clamp( length( f3ShadowMapTraceDir.xy ), 1e-6, sqrt(2.f) );
 //    f3ShadowMapTraceDir /= fTraceLenInShadowMapUVSpace;

 //    float fShadowMapUVStepLen = 0;
 //    fShadowMapUVStepLen = (1.0f / 1024.0f) / max( abs(f3ShadowMapTraceDir.x), abs(f3ShadowMapTraceDir.y) );

	// float fTotalMarchedDistInUVSpace = 0;
	// while( fTotalMarchedDistInUVSpace < fTraceLenInShadowMapUVSpace )
 //    {
 //    	//	
 //    	fTotalMarchedDistInUVSpace += fShadowMapUVStepLen;
 //    }

 	// float stepCount = 3000.0f;
  //   vec3 dir = f3RayEndPos.xyz - f3RayStartPos;
  //   dir /= 3000.0f;
  //   vec3 curWorldPos = f3RayStartPos;
  //   for(int i = 0; i < 3000; i++)
  //   {
		// curWorldPos += dir * i;
		// float u = GetPrecomputedPtLghtSrcTexU(curWorldPos, f3EyeVector, f3ClosestPointToLight);
		// vec3 inscatter = texture(preComputeTexture, vec2(u, fV)).rgb;

		// vec4 curPosLightSpace = lightSpaceMatrix * vec4(curWorldPos, 1.0);
		// float visible = 1.0f - CalculateShadow(curPosLightSpace); 

		// f3FullyLitInsctrIntegral -= inscatter * visible;
  //   }
  //   color = vec4(f3FullyLitInsctrIntegral, 1.0f);

 //  	vec3 worldPos = getCurrentFsWorldPos();
	// vec3 marchingRay = viewPos - worldPos ;
	// float marchingRayLength = trunc( length(marchingRay) );
	// float stepSize = marchingRayLength * NUM_SAMPLES_RCP;
	// vec3 marchingRayDir = normalize(marchingRay);
 //  	vec3 curWorldPos = worldPos;
 //  	float inShadow = 1;// V(P)
	// for(float l = marchingRayLength; l > stepSize; l -= stepSize)
	// {
	// 	curWorldPos += stepSize * marchingRayDir;

	// 	vec4 curPosLightSpace = lightSpaceMatrix * vec4(curWorldPos, 1.0);
	// 	inShadow = CalculateShadow(curPosLightSpace); 

	// 	float u = GetPrecomputedPtLghtSrcTexU(curWorldPos, f3EyeVector, f3ClosestPointToLight);
	// 	vec3 inscatter = texture(preComputeTexture, vec2(u, fV)).rgb;

	// 	f3FullyLitInsctrIntegral -= inscatter * inShadow;
	// }
	// color = vec4(f3FullyLitInsctrIntegral, 1.0f);

	return color;
}

vec4 GetBackgroundColor()
{
	vec4 color;

	vec3 worldPos = getCurrentFsWorldPos();
	float distOtoC = length(worldPos - viewPos);
	vec4 attenuatedFactorOtoC = exp(-CalculateOpticalDepth(distOtoC));

	color = texture(screenTexture, TexCoords);//attenuatedFactorOtoC * texture(screenTexture, TexCoords);//

	return color;
}

const int NUM_SAMPLES = 100;
float Density = 1;
float Weight = 0.01;
float Decay = 1;
float Exposure = 1;

vec4 GpuGem3()
{
	vec3 color = vec3(0,0,0);
	vec4 ScreenLightPos = lightSpaceMatrix * vec4(lightPos, 1.0);
	// Calculate vector from pixel to light source in screen space.
  	vec2 deltaTexCoord = (TexCoords - ScreenLightPos.xy);
  	vec2 texCoord = TexCoords;

  	// Divide by number of samples and scale by control factor.
  	deltaTexCoord *= 1.0f / NUM_SAMPLES * Density;

  	// Store initial sample.
  	color = texture(screenTexture, TexCoords).rgb;//tex2D(frameSampler, texCoord);

  	// Set up illumination decay factor.
  	float illuminationDecay = 1.0f;

  // Evaluate summation from Equation 3 NUM_SAMPLES iterations.
  for (int i = 0; i < NUM_SAMPLES; i++)
  {
    // Step sample location along ray.
    texCoord -= deltaTexCoord;

    // Retrieve sample at new location.
    vec3 sample = texture(screenTexture, TexCoords).rgb;//tex2D(frameSampler, texCoord);

    // Apply sample attenuation scale/decay factors.
    sample *= illuminationDecay * Weight;

    // Accumulate combined color.
    color += sample;

    // Update exponential decay factor.
    illuminationDecay *= Decay;
   }

	// Output final color with a further scale control factor.
	return vec4( color * Exposure, 1);
}

vec3 godrays(
    float density,
    float weight,
    float decay,
    float exposure,
    int numSamples,
    sampler2D occlusionTexture,
    vec2 screenSpaceLightPos,
    vec2 uv
    ) {

    vec3 fragColor = vec3(0.0,0.0,0.0);

	vec2 deltaTextCoord = vec2( uv - screenSpaceLightPos.xy );

	vec2 textCoo = uv.xy ;
	deltaTextCoord *= (1.0 /  float(numSamples)) * density;
	float illuminationDecay = 1.0;


	for(int i=0; i < 100 ; i++){


        /*
        This makes sure that the loop only runs `numSamples` many times.
        We have to do it this way in WebGL, since you can't have a for loop
        that runs a variable number times in WebGL.
        This little hack gets around that.
        But the drawback of this is that we have to specify an upper bound to the
        number of iterations(but 100 is good enough for almost all cases.)
        */
	    if(numSamples < i) {
            break;
	    }

		textCoo -= deltaTextCoord;
		vec3 samp = texture2D(occlusionTexture, textCoo   ).xyz;
		samp *= illuminationDecay * weight;
		fragColor += samp;
		illuminationDecay *= decay;
	}

	fragColor *= exposure;

    return fragColor;


}

void main()
{
	finalColor = GetBackgroundColor() + CalculateInscatteringRayMarchingIntel3();//GetBackgroundColor() +  CalculateInscatteringRayMarchingIntel2();//GetBackgroundColor() + 
	//finalColor.rgb = ToneMap(finalColor.rgb);
	//finalColor = GpuGem3();
	// vec4 ScreenLightPos = screenSpaceMatrix * vec4(lightPos, 1.0);
	// ScreenLightPos.xyz /= ScreenLightPos.w;
	// ScreenLightPos.xyz = (ScreenLightPos.xyz + 1.0) / 2.0f;

	// vec3 fragColor = godrays(
 //    Density,
 //    Weight,
 //    Decay,
 //    Exposure,
 //    NUM_SAMPLES,
 //    screenTexture,
 //    ScreenLightPos.xy,
 //    TexCoords
 //    );

	// finalColor = vec4(fragColor , 1.0);
}