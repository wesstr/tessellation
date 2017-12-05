// Name: Wesley Strong
// NetID: wfs51
// Date: 12/1/17
// Tessilation fragment shader
#version 410

// The output. Always a color
out vec4 fragColor;

in vec4 teColors;
in vec2 fragTexcoord;
in vec3 fragPosition;
in vec3 teNormals;
in vec2 teTexcords;
in vec3 tePosition;

uniform sampler2D miniTexture;
uniform bool drawDefault;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float ambietStrength;
uniform mat4 V;
uniform float specularStrength;

void main()
{  
	vec3 camPosition = vec3(V);
	vec3 viewDir = normalize(camPosition - tePosition);
	vec3 ambient = ambietStrength * lightColor;
	vec3 lightDir = normalize(lightPos - tePosition);
	float diff = max(dot(teNormals, lightDir), 0.0);
	vec3 diffues = diff * lightColor; 
	vec3 reflectDir = reflect(-lightDir, teNormals);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular = specularStrength * spec * lightColor;

	if (drawDefault){
		fragColor = teColors;
		return;
	}

	fragColor = vec4(ambient + diffues + specular, 1.0) * texture(miniTexture, teTexcords);
}
//