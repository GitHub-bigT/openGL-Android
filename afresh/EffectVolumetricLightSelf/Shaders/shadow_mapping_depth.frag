#version 330 core

in vec3 CurrentPosInScreenSpace;
in vec3 LightPosInScreenSpace;
in vec3 ConeViewPosInScreenSpace;

uniform float InConeAngle;

void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
	//vec3 rayDirectionNor = CurrentPosInScreenSpace - LightPosInScreenSpace;// 光照到片元的向量
	//vec3 coneDirectionNor = ConeViewPosInScreenSpace - LightPosInScreenSpace;// 光锥的向量
	//
	//vec2 rayDirectionNor_2 = normalize(vec2(rayDirectionNor.x, rayDirectionNor.y));
	//vec2 coneDirectionNor_2 = normalize(vec2(coneDirectionNor.x, coneDirectionNor.y));
	//float debugValue = acos(dot(rayDirectionNor_2, coneDirectionNor_2)) / 3.1415926;
	//gl_FragDepth = debugValue;
	//return;
	
	//float lightToSurfaceAngle = degrees(acos(dot(rayDirectionNor_2, coneDirectionNor_2)));// 算夹角
	//if(lightToSurfaceAngle > 12)
	//	gl_FragDepth = 1.0f;
	//else
	//	gl_FragDepth = 0.0f;
}