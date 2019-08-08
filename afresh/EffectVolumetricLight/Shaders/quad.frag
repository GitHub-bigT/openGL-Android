#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform vec2 screenSpaceLightPos;
uniform sampler2D screenTexture;

float near = 0.1; 
float far  = 100.0;
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

void main()
{ 
	//color = vec4(0,0,0,1);
	// 深度可视化
	float depth = LinearizeDepth(texture(screenTexture, TexCoords).r) / far;
	color = vec4(vec3(depth), 1.0);
	//color = vec4(vec3(texture(screenTexture, TexCoords).r), 1.0);

	//color = texture(screenTexture, TexCoords);
	return;
	float density = 1.0;
	float weight = 0.01;
	float decay = 1.0;
	float exposure = 1.0;
	int numSamples = 100;
	float illuminationDecay = 1.0;

	vec3 fragColor = vec3(0.0,0.0,0.0);
	vec2 deltaTextCoord = vec2( TexCoords - screenSpaceLightPos.xy );
	vec2 textCoo = TexCoords.xy ;
	deltaTextCoord *= (1.0 /  float(numSamples)) * density;

	for(int i=0; i < 100 ; i++){

		textCoo -= deltaTextCoord;
		vec3 samp = texture(screenTexture, textCoo).xyz;
		samp *= illuminationDecay * weight;
		fragColor += samp;
		illuminationDecay *= decay;

	}
	fragColor *= exposure;
	color = vec4(fragColor,1.0);
    //vec4 colora = texture(screenTexture, TexCoords);
	//color = colora;
}