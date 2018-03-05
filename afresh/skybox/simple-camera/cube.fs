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
	//reflect
	//vec3 R = reflect(I, normalize(Normal));
	//refract
	//����	1.00
	//ˮ	1.33
	//��	1.309
	//����	1.52
	//��ʯ	2.42
	float ratio = 1.00f / 1.52f;
	vec3 R = refract(I, normalize(Normal), ratio);
	FragColor = vec4(texture(skybox, R).rgb, 1.0f);
    //FragColor = texture(sam, TexCoords);
	//if(gl_FragCoord.x < 640)//gl_FragCoord:only read
	//{
	//	FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	//}
	//else
	//{
	//	FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	//}
}