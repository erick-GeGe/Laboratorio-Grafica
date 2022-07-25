#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "shader.h"

using namespace glm;

class Cube
{
public:
    int num_faces;
    vec3 Translate;
    mat4 Rotate;
    vec3 real_pos;
    mat4 init_rotate;
    Cube(vec3, vec3[5], int[5], int);
    void Draw(Shader *);

private:
    vec3 faces_colors[5];
    int faces[5];
};

Cube::Cube(vec3 newTranslate, vec3 _face_colors[], int _faces[], int _num_faces)
{
    Translate = newTranslate;
    Rotate = mat4(1.f);
    num_faces = _num_faces;

    for (size_t i = 0; i < num_faces; i++)
    {
        faces_colors[i] = _face_colors[i];
        faces[i] = _faces[i];
    }
    
    real_pos = glm::vec3(0, 0, 0);
    real_pos = Translate;
    init_rotate = glm::mat4(1.f);
}

void Cube::Draw(Shader *shader_program)
{
    // glm::mat4 model = glm::mat4(1.0f);
    // model = glm::translate(model, Translate);
    // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

    mat4 model = glm::translate(Translate) * Rotate;
    // mat4 mvp = Projection * View * model;
    shader_program->setMat4("model", model);

    // Cube 
    // shader_program->setVec3("color", glm::vec3(1, 1, 1));
    // glDrawArrays(GL_LINE_STRIP, 0, 36);
    shader_program->setVec3("color", glm::vec3(0, 0, 0));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Cube faces
    for (size_t i = 0; i < num_faces; i++)
    {
        shader_program->setVec3("color", faces_colors[i]);
        // glDrawArrays(GL_TRIANGLES, 36 + faces[i]*6, 6);
        glDrawArrays(GL_TRIANGLES, 36 + faces[i]*6, 6);
    }

    // glm::mat4 trans = glm::mat4(1.0f);
    // trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));


    // glm::mat4 matriz = glm::mat4(1.0f);
    // glm::vec3 posicion = vec3(0.5f, -0.5f, 0.0f);
    // matriz = matriz * glm::translate(posicion);
    // glm::mat4 rotacion = rotate((float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    // matriz = matriz * rotacion;

    // matriz = matriz * glm::translate(posicion) * rotacion;
}
