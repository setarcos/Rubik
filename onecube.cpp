#include "onecube.hpp"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>

OneCube::OneCube(float x, float y, float z): vbo(0), cbo(0),
    px(x), py(y), pz(z), angle(glm::mat4(1.0f))
{
}

OneCube::~OneCube(void)
{
    if (vbo) glDeleteBuffers(1, &vbo);
    if (cbo) glDeleteBuffers(1, &cbo);
}

void OneCube::Init(GLuint m)
{
    matrix = m;
	GLfloat vertices[] = { 
		-0.29f,-0.29f,-0.29f,
		-0.29f,-0.29f, 0.29f,
		-0.29f, 0.29f, 0.29f,
		-0.29f,-0.29f,-0.29f,
		-0.29f, 0.29f, 0.29f,
		-0.29f, 0.29f,-0.29f, // Left

		 0.29f, 0.29f, 0.29f,
		 0.29f,-0.29f,-0.29f,
		 0.29f, 0.29f,-0.29f,
		 0.29f,-0.29f,-0.29f,
		 0.29f, 0.29f, 0.29f,
		 0.29f,-0.29f, 0.29f, // Right

		 0.29f, 0.29f,-0.29f,
		-0.29f,-0.29f,-0.29f,
		-0.29f, 0.29f,-0.29f,
		 0.29f, 0.29f,-0.29f,
		 0.29f,-0.29f,-0.29f,
		-0.29f,-0.29f,-0.29f, // Back

		-0.29f, 0.29f, 0.29f,
		-0.29f,-0.29f, 0.29f,
		 0.29f,-0.29f, 0.29f,
		 0.29f, 0.29f, 0.29f,
		-0.29f, 0.29f, 0.29f,
		 0.29f,-0.29f, 0.29f, // Front

		 0.29f, 0.29f, 0.29f,
		 0.29f, 0.29f,-0.29f,
		-0.29f, 0.29f,-0.29f,
		 0.29f, 0.29f, 0.29f,
		-0.29f, 0.29f,-0.29f,
		-0.29f, 0.29f, 0.29f, // Top

		 0.29f,-0.29f, 0.29f,
		-0.29f,-0.29f,-0.29f,
		 0.29f,-0.29f,-0.29f,
		 0.29f,-0.29f, 0.29f,
		-0.29f,-0.29f, 0.29f,
		-0.29f,-0.29f,-0.29f, // Bottom
	};

    GLfloat colors[12 * 3 * 3];
    static const GLfloat basec[] =
    {
        0.0f, 1.0f, 0.0f, // Green
        0.0f, 0.0f, 1.0f, // Blue
        1.0f, 0.5f, 0.0f, // Oriange
        1.0f, 0.0f, 0.0f, // Red
        1.0f, 1.0f, 1.0f, // White
        1.0f, 1.0f, 0.0f, // Yellow
    };

    for (int i = 0; i < 12 * 3; ++i) {
        vertices[i * 3] += px;
        vertices[i * 3 + 1] += py;
        vertices[i * 3 + 2] += pz;
    }
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6 * 3; ++j)
            colors[i * 6 * 3 + j] = basec[i * 3 + (j % 3)];
    }

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &cbo);
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
}

void OneCube::Render(const glm::mat4& mvp)
{
    glm::mat4 tmpmvp = mvp * angle;
    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    glUniformMatrix4fv(matrix, 1, GL_FALSE, &tmpmvp[0][0]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(
        0,
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glVertexAttribPointer(
        1,
        3,                 // size
        GL_FLOAT,          // type
        GL_FALSE,          // normalized?
        0,                 // stride
        (void*)0           // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void OneCube::AddAngle(const glm::mat4& a)
{
    angle = a * angle;
}

void OneCube::FitAngle(void)
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
       //     printf("%10.3f, ", angle[i][j]);
            if (angle[i][j] > 0.99) angle[i][j] = 1;
            if (angle[i][j] < -0.99) angle[i][j] = -1;
            if (abs(angle[i][j]) < 0.1) angle[i][j] = 0;
        }
       // printf("\n");
    }
}
