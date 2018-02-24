#version 330 core
out vec4 FragColor;

//in vec2 TexCoords;
in vec3 Position;
in vec3 Normal;

uniform vec3 cameraPos;
//uniform sampler2D sam;
uniform samplerCube skybox;

void main()
{    
	vec3 I = normalize(Position - cameraPos);
	vec3 R = reflect(I, normalize(Normal));
	FragColor = vec4(texture(skybox, R).rgb, 1.0f);
    //FragColor = texture(sam, TexCoords);
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}