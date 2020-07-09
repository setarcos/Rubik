#include "onecube.hpp"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>

OneCube::OneCube(float x, float y, float z): vbo(0),
    px(x), py(y), pz(z), angle(glm::mat4(1.0f)), theta(0)
{
}

OneCube::~OneCube(void)
{
    if (vbo) glDeleteBuffers(1, &vbo);
}

void OneCube::Init(GLuint m)
{
    theta = m;
	static const GLfloat vertices[] = {
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

    GLfloat vbodata[12 * 3 * 6];
    static const GLfloat basec[] =
    {
        0.0f, 1.0f, 0.0f, // Green
        0.0f, 0.0f, 1.0f, // Blue
        1.0f, 0.5f, 0.0f, // Oriange
        1.0f, 0.0f, 0.0f, // Red
        1.0f, 1.0f, 1.0f, // White
        1.0f, 1.0f, 0.0f, // Yellow
    };

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            vbodata[i * 36 + j * 6] = vertices[i * 18 + j * 3] + px;
            vbodata[i * 36 + j * 6 + 1] = vertices[i * 18 + j * 3 + 1] + py;
            vbodata[i * 36 + j * 6 + 2] = vertices[i * 18 + j * 3 + 2] + pz;
            for (int k = 0; k < 3; ++ k)
                vbodata[i * 36 + j * 6 + 3 + k] = basec[i * 3 + k];
        }
    }

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbodata), vbodata, GL_STATIC_DRAW);
}

void OneCube::Render()
{
    glUniformMatrix4fv(theta, 1, GL_FALSE, &angle[0][0]);

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        6 * sizeof(GLfloat), // stride
        (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,                 // size
        GL_FLOAT,          // type
        GL_FALSE,          // normalized?
        6 * sizeof(GLfloat), // stride
        (void*)(3 * sizeof(GLfloat)) // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OneCube::AddAngle(const glm::mat4& a)
{
    angle = a * angle;
}

void OneCube::FitAngle(void)
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (angle[i][j] > 0.99) angle[i][j] = 1;
            if (angle[i][j] < -0.99) angle[i][j] = -1;
            if (abs(angle[i][j]) < 0.1) angle[i][j] = 0;
        }
    }
}
