#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "shader.h"


class Cube {
    public:
        glm::vec3 Translate;
        glm::vec3 Scale;
        glm::mat4 Rotate;
        Cube (glm::vec3, glm::vec3, glm::mat4);
        void Draw(Shader *, glm::mat4, glm::mat4, GLuint);
};


Cube::Cube(glm::vec3 newTranslate, glm::vec3 newScale, glm::mat4 newRotate) {
    Translate = newTranslate;
    Scale = newScale;
    Rotate = newRotate;
}

void Cube::Draw(Shader *shader_program, glm::mat4 Projection, glm::mat4 View, GLuint vertexCount) {
    glm::mat4 model = glm::translate(Translate) * Rotate * glm::scale(Scale);
    glm::mat4 mvp = Projection * View * model;

    shader_program->setMat4("MVP", mvp);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

    // shader_program->setMat4("ModelRotation", Rotate);
    // GLuint RotationID = glGetUniformLocation(programID, "ModelRotation");A
    // glUniformMatrix4fv(RotationID, 1, GL_FALSE, &Rotate[0][0]);

    // GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    // glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
    // shader_program->setVec3("Colors", *Colors);
    // GLuint ColorsID = glGetUniformLocation(programID, "Colors");
    // glUniform3fv(ColorsID, 3, &Colors[0][0]);

