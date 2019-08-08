#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

out vec3 CurrentPosInScreenSpace;
out vec3 LightPosInScreenSpace;
out vec3 ConeViewPosInScreenSpace;

uniform vec3 lightPosInWorldSpace;
uniform vec3 coneViewPosInWorldSpace;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(position, 1.0f);

	//vec4 CurrentPosInClipSpace = gl_Position;
	//CurrentPosInClipSpace = CurrentPosInClipSpace / CurrentPosInClipSpace.w;// 透视变换
	//CurrentPosInClipSpace = CurrentPosInClipSpace * 0.5 + 0.5;// NDC -> [0,1]
	//CurrentPosInScreenSpace = vec3(CurrentPosInClipSpace.x, CurrentPosInClipSpace.y, CurrentPosInClipSpace.z);
	//
	//vec4 LightPosInClipSpace = lightSpaceMatrix * vec4(lightPosInWorldSpace, 1.0f);
	//LightPosInClipSpace = LightPosInClipSpace / LightPosInClipSpace.w;// 透视变换
	//LightPosInClipSpace = LightPosInClipSpace * 0.5 + 0.5;// NDC -> [0,1]
	//LightPosInScreenSpace = vec3(LightPosInClipSpace.x, LightPosInClipSpace.y, LightPosInClipSpace.z);
	//
	//vec4 ConeViewPosInClipSpace = lightSpaceMatrix * vec4(ConeViewPosInScreenSpace, 1.0f);
	//ConeViewPosInClipSpace = ConeViewPosInClipSpace / ConeViewPosInClipSpace.w;
	//ConeViewPosInClipSpace = ConeViewPosInClipSpace * 0.5 + 0.5;
	//ConeViewPosInScreenSpace = vec3(ConeViewPosInClipSpace.x, ConeViewPosInClipSpace.y, ConeViewPosInClipSpace.z);
}