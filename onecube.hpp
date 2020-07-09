#include <GL/glew.h>
#include <glm/glm.hpp>

class OneCube
{
    private:
        GLuint vbo;
        float px, py, pz;
        glm::mat4 angle;
        GLuint theta;

        float fit(float f);
    public:
        OneCube(float x, float y, float z);
        ~OneCube(void);

        void Init(GLuint m);
        void Render();
        void AddAngle(const glm::mat4& a);
        void FitAngle(void);
};
