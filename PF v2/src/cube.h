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
    vec3 init_pos;
    mat4 init_rotate;
    Cube(vec3, vec3[3], int[3], int);
    void Draw(Shader *, mat4, mat4);

private:
    vec3 faces_colors[3];
    int faces[3];
};

Cube::Cube(vec3 newTranslate, vec3 _face_colors[3], int _faces[3], int _num_faces)
{
    Translate = newTranslate;
    Rotate = mat4(1.f);
    num_faces = _num_faces;

    for (size_t i = 0; i < num_faces; i++)
    {
        faces_colors[i] = _face_colors[i];
        faces[i] = _faces[i];
    }
    
    init_pos = glm::vec3(0, 0, 0);
    init_pos = Translate;
    init_rotate = glm::mat4(1.f);
}

void Cube::Draw(Shader *shader_program, mat4 Projection, mat4 View)
{

    mat4 model = Rotate * glm::translate(Translate) ;
    mat4 mvp = Projection * View * model;
    shader_program->setMat4("MVP", mvp);
    shader_program->setVec3("color", glm::vec3(0, 0, 0));

    // Cube white
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Cube faces of cube
    for (size_t i = 0; i < num_faces; i++)
    {
        shader_program->setVec3("color", faces_colors[i]);
        glDrawArrays(GL_TRIANGLES, 36 + faces[i]*6, 6);
    }
}
