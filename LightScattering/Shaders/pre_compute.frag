#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform vec4 rayleighBeta;
uniform vec4 mieBeta;
uniform vec4 summTotalBeta;
uniform float m_fMaxTracingDistance;

#define PI 3.14159265358979

vec3 EvaluatePhaseFunction(float fCosTheta)
{
//#if ANISOTROPIC_PHASE_FUNCTION
//    float3 f3RlghInsctr =  g_MediaParams.f4AngularRayleighBeta.rgb * (1.0 + fCosTheta*fCosTheta);
//    float HGTemp = rsqrt( dot(g_MediaParams.f4HG_g.yz, float2(1.f, fCosTheta)) );
//    float3 f3MieInsctr = g_MediaParams.f4AngularMieBeta.rgb * g_MediaParams.f4HG_g.x * (HGTemp*HGTemp*HGTemp);
//#else
    vec3 f3RlghInsctr = rayleighBeta.rgb / (4.0*PI);
    vec3 f3MieInsctr = mieBeta.rgb / (4.0*PI);
//#endif

    return f3RlghInsctr + f3MieInsctr;
}

vec2 ProjToUV(vec2 f2ProjSpaceXY)
{
    return vec2(0.5, 0.5) + vec2(0.5, -0.5) * f2ProjSpaceXY;
}

// LUT的思路：
// 化简积分公式
// 以light为0，0点
// y为光投影到C->O上的点的距离，范围为0 ~ +∞
// x为投影点离camera的距离，范围为-∞ ~ +∞
void main()
{ 
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
    //
    //vec2 f2UV = ProjToUV(TexCoords.xy);
    //float fStartDistFromProjection = TexCoords.x * m_fMaxTracingDistance;
    //float fClosestDistToLight = f2UV.y * m_fMaxTracingDistance;

    vec2 projPos = TexCoords * 2.0f - 1.0f;
    float fStartDistFromProjection = projPos.x * m_fMaxTracingDistance;
    float fClosestDistToLight = TexCoords.y * m_fMaxTracingDistance;

    vec3 f3InsctrRadinance = vec3(0.f, 0.f, 0.f);
    
    // There is a very important property: pre-computed scattering must be monotonical with respect
    // to u coordinate. However, if we simply subdivide the tracing distance onto the equal number of steps
    // as in the following code, we cannot guarantee this
    // 
    // float fStepWorldLen = length(f2StartPos-f2EndPos) / fNumSteps;
    // for( float fRelativePos=0; fRelativePos < 1; fRelativePos += 1.f/fNumSteps )
    // {
    //      vec2 f2CurrPos = lerp(f2StartPos, f2EndPos, fRelativePos);    
    //      ...
    //
    // To assure that the scattering is monotonically increasing, we must go through
    // exactly the same taps for all pre-computations. The simple method to achieve this
    // is to make the world step the same as the difference between two neighboring texels:
    // The step can also be integral part of it, but not greater! So /2 will work, but *2 won't!

    float fStepWorldLen = dFdx(fStartDistFromProjection);
    for(float fDistFromProj = fStartDistFromProjection; fDistFromProj < m_fMaxTracingDistance; fDistFromProj += fStepWorldLen)
    {
        vec2 f2CurrPos = vec2(fDistFromProj, fClosestDistToLight);
        float fDistToLightSqr = dot(f2CurrPos, f2CurrPos);
        float fDistToLight = sqrt(fDistToLightSqr);
        float fDistToCam = f2CurrPos.x - fStartDistFromProjection;
        vec3 f3Extinction = exp( -(fDistToCam + fDistToLight) * summTotalBeta.rgb ); //LP PC
        vec2 f2LightDir = normalize(f2CurrPos);
		//    sun
		//      \
		//       \
		//    ----\------eye
		//         \theta 
		//          \
		// θ = pi - β
		// ------  cosθ = cos(pi - β) = cosΠcosβ-sinΠsinβ = -cosβ  ---- //
		// dot(pos-L(0,0), (1,0)) = pos.x*1 + pos.y*0 = pos.x = f2LightDir.x;
		// 此时的theta是 L->P 和 P->O 的夹角，和ppt给出的角不一致
        float fCosTheta = f2LightDir.x;
	
        vec3 f3dLInsctr = f3Extinction * EvaluatePhaseFunction(fCosTheta) * fStepWorldLen / max(fDistToLightSqr,m_fMaxTracingDistance*m_fMaxTracingDistance*1e-8);
        f3InsctrRadinance += f3dLInsctr;
    }

	color = vec4(f3InsctrRadinance,1);//vec4(1.5,0.5,0.5,1);
}