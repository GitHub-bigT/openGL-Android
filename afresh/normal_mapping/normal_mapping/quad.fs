#version 330 core
out vec4 FragColor;

in vec2 uv;
in vec3 worldPos;
in vec3 normal_vs;
in mat3 TBN;

uniform vec3 lightPos;
uniform vec3 lightLookAt;
uniform vec3 viewPos;
uniform int useNormalMapping;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

void main()
{   
	if(useNormalMapping == 0)
	{
		vec3 V = normalize(viewPos - worldPos);
		vec3 L = normalize(lightPos - lightLookAt);
		vec3 N = normalize(normal_vs);
		vec3 H = normalize(L + V);  

	    // get diffuse color
		vec3 color = texture(diffuseMap, uv).rgb;
		// ambient
		vec3 ambient = 0.1 * color;
		// diffuse
		float diff = max(dot(L, N), 0.0);
		vec3 diffuse = diff * color;
		// specular
		vec3 reflectDir = reflect(-L, N);
		float spec = pow(max(dot(N, H), 0.0), 32.0);
		vec3 specular = vec3(0.2) * spec;

		FragColor = vec4(ambient + diffuse + specular, 1.0);
	} 
	else
	{
		vec3 V = normalize(viewPos - worldPos);
		vec3 L = normalize(lightPos - lightLookAt);
		vec3 N = normalize(TBN * ((texture(normalMap, uv).rgb) * 2.0 - 1.0));
		vec3 H = normalize(L + V);  

	    // get diffuse color
		vec3 color = texture(diffuseMap, uv).rgb;
		// ambient
		vec3 ambient = 0.1 * color;
		// diffuse
		float diff = max(dot(L, N), 0.0);
		vec3 diffuse = diff * color;
		// specular
		vec3 reflectDir = reflect(-L, N);
		float spec = pow(max(dot(N, H), 0.0), 32.0);
		vec3 specular = vec3(0.2) * spec;

		FragColor = vec4(ambient + diffuse + specular, 1.0);
	}
}