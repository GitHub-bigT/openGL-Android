#version 330 core
out vec4 FragColor;

in vec3 textureDir;
uniform samplerCube skybox;

void main()
{    
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	FragColor = texture(skybox, textureDir);
	FragColor.rgb = pow(FragColor.rgb, vec3(1 / 2.2));
}