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
        vec3 Scale;
        mat4 Rotate;
        Cube (vec3, vec3, mat4, vec3[3]);
        void Draw(Shader *, mat4, mat4, GLuint);
    private:
        vec3* Colors;
};


Cube::Cube(vec3 newTranslate, vec3 newScale, mat4 newRotate, vec3 newColors[3]) {
    Translate = newTranslate;
    Scale = newScale;
    Rotate = newRotate;
    Colors = newColors;
}

void Cube::Draw(Shader *shader_program, mat4 Projection, mat4 View, GLuint vertexCount) {
    // mat4 model = glm::mat4(1.0f);
    mat4 model = glm::translate(Translate) * Rotate * glm::scale(Scale);
    mat4 mvp = Projection * View * model;

    // shader_program->setMat4("ModelRotation", Rotate);
    // GLuint RotationID = glGetUniformLocation(programID, "ModelRotation");
    // glUniformMatrix4fv(RotationID, 1, GL_FALSE, &Rotate[0][0]);
    shader_program->setMat4("MVP", mvp);

    // GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    // glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
    // shader_program->setVec3("Colors", *Colors);
    // GLuint ColorsID = glGetUniformLocation(programID, "Colors");
    // glUniform3fv(ColorsID, 3, &Colors[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

