#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

// Output data ; will be interpolated for each fragment.
out vec3 fragmentColor;
out vec3 position;
// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 R;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * R * vec4(vertexPosition_modelspace,1);
    position = (R * vec4(vertexPosition_modelspace,1)).xyz;
	// The color of each vertex will be interpolated
	// to produce the color of each fragment
	fragmentColor = vertexColor;
}

