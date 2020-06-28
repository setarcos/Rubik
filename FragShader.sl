#version 330 core

// Interpolated values from the vertex shaders
in vec3 fragmentColor;
in vec3 position;
// Ouput data
out vec3 color;

void main(){

	// Output color = color specified in the vertex shader,
	// interpolated between all 3 surrounding vertices
    float dist = length(vec3(3.0, 2.0, 2.0) - position);
	color = fragmentColor * 10 / dist / dist;
}
