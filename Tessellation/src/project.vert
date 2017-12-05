/// Name: Wesley Strong
// NetID: wfs51
// Date: 12/1/17
// Tessilation vertex shader
// Transform position into clip coordinates
#version 410

in vec3 position;
in vec3 normals;
in vec2 texcoord;

out vec3 vPosition;
out vec2 vTexcords;
out vec3 fragPosition;
out vec3 vNormals;
out vec4 outColor;
out vec2 windowPosition;

// Model View Projection uniform
uniform mat4 MVP;
// Model matrix
uniform mat4 M;
// View matrix
uniform mat4 V;
// For some reason my GPU doest support uniform arrays so i just used a 3x3 matrix
uniform mat3 color;
// Light position in world space

// For A3, the only out varying we need is the gl_Position. We could generate
// the color here if desired. Out variables go here if any.


void main(void)
{
	// Transform points to the MVP
	// Remember, lemons live here...
	gl_Position = vec4(position, 1);
	// UV data for texture
	vTexcords = texcoord;
    // The out color is RGBA, so add the A to our already RGB values
	outColor = vec4(color[0][0], color[1][1], color[2][2], 1);
	fragPosition = vec3( M * vec4(position, 1.0));
	vNormals = mat3(M) * normals;

}
//