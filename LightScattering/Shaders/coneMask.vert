#version 330 core // cone
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;

out vec3 CurrentPosInScreenSpace;
out vec3 LightPosInScreenSpace;
out vec3 ConeViewPosInScreenSpace;
out vec4 FragPosLightSpace;

uniform vec3 light_position;
uniform vec3 cone_view_position;
uniform mat4 cameraSpaceMatrix;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = vec4(position, 1.0f);
    TexCoords = texCoords;

	vec4 CurrentPosInClipSpace = vec4(position, 1.0f);
	CurrentPosInClipSpace = CurrentPosInClipSpace / CurrentPosInClipSpace.w;// 透视变换
	CurrentPosInClipSpace = CurrentPosInClipSpace * 0.5 + 0.5;// NDC -> [0,1]
	CurrentPosInScreenSpace = vec3(CurrentPosInClipSpace.x, CurrentPosInClipSpace.y, CurrentPosInClipSpace.z);

	vec4 LightPosInClipSpace = cameraSpaceMatrix * vec4(light_position, 1.0f);
	LightPosInClipSpace = LightPosInClipSpace / LightPosInClipSpace.w;// 透视变换
	LightPosInClipSpace = LightPosInClipSpace * 0.5 + 0.5;// NDC -> [0,1]
	LightPosInScreenSpace = vec3(LightPosInClipSpace.x, LightPosInClipSpace.y, LightPosInClipSpace.z);
	
	vec4 ConeViewPosInClipSpace = cameraSpaceMatrix * vec4(cone_view_position, 1.0f);
	ConeViewPosInClipSpace = ConeViewPosInClipSpace / ConeViewPosInClipSpace.w;
	ConeViewPosInClipSpace = ConeViewPosInClipSpace * 0.5 + 0.5;
	ConeViewPosInScreenSpace = vec3(ConeViewPosInClipSpace.x, ConeViewPosInClipSpace.y, ConeViewPosInClipSpace.z);

	FragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0);
} 