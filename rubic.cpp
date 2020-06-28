// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "onecube.hpp"
#include "shader.hpp"

void rotate_cube(unsigned char* idx, const unsigned char *face)
{
    char tmpcube[2];
    for (int i = 0; i < 2; ++i)
        tmpcube[i] = idx[face[i]];
    for (int i = 0; i < 6; ++i)
        idx[face[i]] = idx[face[i + 2]];
    for (int i = 0; i < 2; ++i)
        idx[face[6 + i]] = tmpcube[i];
}

int main( void )
{
    OneCube* cube[26];
    const char cubepos[] ={
        -1, 1, -1, 0, 1, -1, 1, 1, -1,
        -1, 1, 0, 0, 1, 0, 1, 1, 0,
        -1, 1, 1, 0, 1, 1, 1, 1, 1,
        -1, 0, -1, 0, 0, -1, 1, 0, -1,
        -1, 0, 0,          1, 0, 0,
        -1, 0, 1, 0, 0, 1, 1, 0, 1,
        -1, -1, -1, 0, -1, -1, 1, -1, -1,
        -1, -1, 0, 0, -1, 0, 1, -1, 0,
        -1, -1, 1, 0, -1, 1, 1, -1, 1,
    };
    unsigned char cubeidx[26] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
    const unsigned char cubeface[][9] = {
        {0, 1, 2, 5, 8, 7, 6, 3, 4}, // Top
        {23, 24, 25, 22, 19, 18, 17, 20, 21}, // Bottom
        {0, 3, 6, 14, 23, 20, 17, 9, 12}, // Left
        {8, 5, 2, 11, 19, 22, 25, 16, 13}, // Right
        {6, 7, 8, 16, 25, 24, 23, 14, 15}, // Front
        {17, 18, 19, 11, 2, 1, 0, 9, 10}, // Back
    };
    const float speed = glm::radians(2.0f);
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Tutorial 04 - Colored Cube", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "VertShader.sl", "FragShader.sl" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45? Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
                    glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
                    glm::vec3(0,0,0), // and looks at the origin
                    glm::vec3(0,1,0)  // Head is up
						   );
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model      = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

    for (int i = 0; i < 26; ++i) {
        cube[i] = new OneCube(cubepos[i * 3] * 0.6,
                cubepos[i * 3 + 1] * 0.6, cubepos[i * 3 + 2] * 0.6);
        cube[i]->Init(MatrixID);
    }
    GLfloat angle = 0;
    int step = 0;
    glm::vec3 dir[] = {
        glm::vec3(0, 1, 0),
        glm::vec3(0, -1, 0),
        glm::vec3(-1, 0, 0),
        glm::vec3(1, 0, 0),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, -1),
    };
    bool rotating = false;

    // Use our shader
    glUseProgram(programID);

	do{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (rotating) {
            if (angle < glm::radians(90.0f)) {
                angle += speed;
                glm::mat4 rotate_speed = glm::rotate(glm::mat4(1.0f),
                        speed, dir[step]);
                for (int i = 0; i < 9; ++i)
                    cube[cubeidx[cubeface[step][i]]]->AddAngle(rotate_speed);
            }
            else {
                rotating = false;
                angle = 0;
                for (int i = 0; i < 9; ++i)
                    cube[cubeidx[cubeface[step][i]]]->FitAngle();
                rotate_cube(cubeidx, cubeface[step]);
            }
        } else {
            if (glfwGetKey( window, GLFW_KEY_U ) == GLFW_PRESS){
                step = 0;
                rotating = true;
            }
            if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
                step = 1;
                rotating = true;
            }
            if (glfwGetKey( window, GLFW_KEY_L ) == GLFW_PRESS){
                step = 2;
                rotating = true;
            }
            if (glfwGetKey( window, GLFW_KEY_R ) == GLFW_PRESS){
                step = 3;
                rotating = true;
            }
            if (glfwGetKey( window, GLFW_KEY_F ) == GLFW_PRESS){
                step = 4;
                rotating = true;
            }
            if (glfwGetKey( window, GLFW_KEY_B ) == GLFW_PRESS){
                step = 5;
                rotating = true;
            }
        }

        for (int i = 0; i < 26; ++i)
            cube[i]->Render(MVP);
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );
    for (int i = 0; i < 26; ++i)
        delete cube[i];
	// Cleanup VBO and shader
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

