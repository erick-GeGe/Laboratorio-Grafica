#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "shader.h"

using namespace glm;

class Cube {
    public:
        vec3 Translate;
        mat4 Rotate;
        vec3 init_pos;
        mat4 init_rotate;
        Cube (vec3, mat4, vec3[3]);
        void Draw(Shader *, mat4, mat4);
    private:
        vec3 faces[3];
};


Cube::Cube(vec3 newTranslate, mat4 newRotate, vec3 newColors[3]) {
    Translate = newTranslate;
    Rotate = newRotate;
    for (size_t i = 0; i < 3; i++)
        faces[i] = newColors[i];
    init_pos = glm::vec3(0,0,0);
    init_pos = Translate;
    init_rotate = glm::mat4(1.f);
}




void Cube::Draw(Shader *shader_program, mat4 Projection, mat4 View) {
    
    mat4 model = Rotate * glm::translate(Translate) ;
    mat4 mvp = Projection * View * model;
    shader_program->setMat4("MVP", mvp);

    glDrawArrays(GL_TRIANGLES, 0, 12);
    glDrawArrays(GL_TRIANGLES, 12, 24);
    glDrawArrays(GL_TRIANGLES, 24, 36);
}

