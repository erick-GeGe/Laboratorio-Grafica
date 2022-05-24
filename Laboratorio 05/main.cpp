#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Generate forms
void generateCicle(int, float*&, float*&);
void generateLeaf(int, float*&, float*&);

// settings
const unsigned int SCR_WIDTH = 700;
const unsigned int SCR_HEIGHT = 700;
const float M_PI = 3.14;




int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Vitral", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader ourShader("C:/Users/erick/Documents/2022-A/grafica/template/src/Testing/3.3.shader.vs", "C:/Users/erick/Documents/2022-A/grafica/template/src/Testing/3.3.shader.fs");


    int num_triangles = 64;
    // --------------------- Circle -----------------
    float* verticesCircle = nullptr;
    float* borderCircle = nullptr;
    generateCicle(num_triangles, verticesCircle, borderCircle);

    // --------------------- Leaf -----------------
    float* verticesLeaf = nullptr;
    float* borderLeaf = nullptr;
    generateLeaf(num_triangles, verticesLeaf, borderLeaf);


    // --------------------- Circle -----------------
    unsigned int circleVBO, circleVAO;
    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);

    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    glBufferData(GL_ARRAY_BUFFER, num_triangles * 3 * 3 * sizeof(float), verticesCircle, GL_STATIC_DRAW);

    glBindVertexArray(circleVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // --------------- Border
    unsigned int borderVBO, borderVAO;
    glGenVertexArrays(1, &borderVAO);
    glGenBuffers(1, &borderVBO);

    glBindBuffer(GL_ARRAY_BUFFER, borderVBO);
    glBufferData(GL_ARRAY_BUFFER, (num_triangles + 8) * 2 * 3 * sizeof(float), borderCircle, GL_STATIC_DRAW);

    glBindVertexArray(borderVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // --------------------- Leaf -----------------
    unsigned int leafVBO, leafVAO;
    glGenVertexArrays(1, &leafVAO);
    glGenBuffers(1, &leafVBO);

    glBindBuffer(GL_ARRAY_BUFFER, leafVBO);
    glBufferData(GL_ARRAY_BUFFER, num_triangles * 3 * 3 * sizeof(float), verticesLeaf, GL_STATIC_DRAW);

    glBindVertexArray(leafVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // --------------- Border
    unsigned int borderLeafVBO, borderLeafVAO;
    glGenVertexArrays(1, &borderLeafVAO);
    glGenBuffers(1, &borderLeafVBO);

    glBindBuffer(GL_ARRAY_BUFFER, borderLeafVBO);
    glBufferData(GL_ARRAY_BUFFER, num_triangles * 2 * 3 * sizeof(float), borderLeaf, GL_STATIC_DRAW);

    glBindVertexArray(borderLeafVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glLineWidth(8);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ourShader.use();

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::scale(trans, glm::vec3(.9, 0.9, 0.9));
        ourShader.setMat4("transform", trans);

        //------------Circle-----------------
        glBindVertexArray(circleVAO);
        ourShader.setVec4("ourColor", 0.08f, 0.1f, 0.48f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3 * num_triangles);
        ourShader.setVec4("ourColor", 0.f, 0.f, 0.f, 1.0f);
        glBindVertexArray(borderVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 2 * (num_triangles + 8));

        //------------Leafs green-----------------
        trans = glm::scale(trans, glm::vec3(.6, 0.6, 0.6));
        for (size_t i = 0; i < 4; i++)
        {
            glBindVertexArray(leafVAO);
            ourShader.setVec4("ourColor", 0.19f, 0.44f, 0.07f, 1.0f);
            trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            ourShader.setMat4("transform", trans);
            glDrawArrays(GL_TRIANGLES, 0, 3 * num_triangles);
            ourShader.setVec4("ourColor", 0.f, 0.f, 0.f, 1.0f);
            glBindVertexArray(borderLeafVAO);
            glDrawArrays(GL_LINE_STRIP, 0, 2 * num_triangles);

        }
        glBindVertexArray(circleVAO);
        ourShader.setVec4("ourColor", 0.19f, 0.44f, 0.07f, 1.0f);
        trans = glm::scale(trans, glm::vec3(1.15f, 1.15f, 1.15f));
        ourShader.setMat4("transform", trans);
        glDrawArrays(GL_TRIANGLES, 0, 3 * num_triangles);

        //------------Leafs yellow-----------------
        trans = glm::scale(trans, glm::vec3(0.72, 0.72, 0.72));
        trans = glm::rotate(trans, glm::radians(22.5f), glm::vec3(0.0f, 0.0f, 1.0f));
        for (size_t i = 0; i < 4; i++)
        {
            glBindVertexArray(leafVAO);
            ourShader.setVec4("ourColor", 0.77f, 0.5f, 0.03f, 1.0f);
            trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            ourShader.setMat4("transform", trans);
            glDrawArrays(GL_TRIANGLES, 0, 3 * num_triangles);
            ourShader.setVec4("ourColor", 0.f, 0.f, 0.f, 1.0f);
            glBindVertexArray(borderLeafVAO);
            glDrawArrays(GL_LINE_STRIP, 0, 2 * num_triangles);
        }

        glBindVertexArray(circleVAO);
        ourShader.setVec4("ourColor", 0.77f, 0.5f, 0.03f, 1.0f);
        trans = glm::scale(trans, glm::vec3(1.15, 1.15, 1.15));
        ourShader.setMat4("transform", trans);
        glDrawArrays(GL_TRIANGLES, 0, 3 * num_triangles);

        //------------Leafs red-----------------
        trans = glm::scale(trans, glm::vec3(0.36, 0.36, 0.36));

        // red 1
        trans = glm::rotate(trans, glm::radians(337.5f), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::translate(trans, glm::vec3(0, 1.4, 0.0f));
        trans = glm::rotate(trans, glm::radians(-45.f), glm::vec3(0.0f, 0.0f, 1.0f));
        glBindVertexArray(leafVAO);
        ourShader.setVec4("ourColor", 0.5f, 0.02f, 0.015f, 1.0f);
        ourShader.setMat4("transform", trans);
        glDrawArrays(GL_TRIANGLES, 0, 3 * num_triangles);
        ourShader.setVec4("ourColor", 0.f, 0.f, 0.f, 1.0f);
        glBindVertexArray(borderLeafVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 2 * num_triangles);

        // red 2
        trans = glm::rotate(trans, glm::radians(45.f), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::translate(trans, glm::vec3(0, -2.8, 0.0f));
        trans = glm::rotate(trans, glm::radians(-45.f), glm::vec3(0.0f, 0.0f, 1.0f));
        glBindVertexArray(leafVAO);
        ourShader.setVec4("ourColor", 0.5f, 0.02f, 0.015f, 1.0f);
        ourShader.setMat4("transform", trans);
        glDrawArrays(GL_TRIANGLES, 0, 3 * num_triangles);
        ourShader.setVec4("ourColor", 0.f, 0.f, 0.f, 1.0f);
        glBindVertexArray(borderLeafVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 2 * num_triangles);

        // red 3
        trans = glm::rotate(trans, glm::radians(45.f), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::translate(trans, glm::vec3(1.4, 1.4, 0.0f));
        trans = glm::rotate(trans, glm::radians(-135.f), glm::vec3(0.0f, 0.0f, 1.0f));
        glBindVertexArray(leafVAO);
        ourShader.setVec4("ourColor", 0.5f, 0.02f, 0.015f, 1.0f);
        ourShader.setMat4("transform", trans);
        glDrawArrays(GL_TRIANGLES, 0, 3 * num_triangles);
        ourShader.setVec4("ourColor", 0.f, 0.f, 0.f, 1.0f);
        glBindVertexArray(borderLeafVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 2 * num_triangles);

        // red 4
        trans = glm::rotate(trans, glm::radians(135.f), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::translate(trans, glm::vec3(-2.8, 0, 0.0f));
        trans = glm::rotate(trans, glm::radians(-135.f), glm::vec3(0.0f, 0.0f, 1.0f));
        glBindVertexArray(leafVAO);
        ourShader.setVec4("ourColor", 0.5f, 0.02f, 0.015f, 1.0f);
        ourShader.setMat4("transform", trans);
        glDrawArrays(GL_TRIANGLES, 0, 3 * num_triangles);
        ourShader.setVec4("ourColor", 0.f, 0.f, 0.f, 1.0f);
        glBindVertexArray(borderLeafVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 2 * num_triangles);

        // red 5
        trans = glm::rotate(trans, glm::radians(135.f), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::translate(trans, glm::vec3(2.4, 1, 0.0f));
        trans = glm::rotate(trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
        glBindVertexArray(leafVAO);
        ourShader.setVec4("ourColor", 0.5f, 0.02f, 0.015f, 1.0f);
        ourShader.setMat4("transform", trans);
        glDrawArrays(GL_TRIANGLES, 0, 3 * num_triangles);
        ourShader.setVec4("ourColor", 0.f, 0.f, 0.f, 1.0f);
        glBindVertexArray(borderLeafVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 2 * num_triangles);

        // red 6
        trans = glm::rotate(trans, glm::radians(-90.f), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::translate(trans, glm::vec3(-2, -2, 0.0f));
        trans = glm::rotate(trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
        glBindVertexArray(leafVAO);
        ourShader.setVec4("ourColor", 0.5f, 0.02f, 0.015f, 1.0f);
        ourShader.setMat4("transform", trans);
        glDrawArrays(GL_TRIANGLES, 0, 3 * num_triangles);
        ourShader.setVec4("ourColor", 0.f, 0.f, 0.f, 1.0f);
        glBindVertexArray(borderLeafVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 2 * num_triangles);

        // red 7
        trans = glm::rotate(trans, glm::radians(-90.f), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::translate(trans, glm::vec3(0, 2, 0.0f));
        glBindVertexArray(leafVAO);
        ourShader.setVec4("ourColor", 0.5f, 0.02f, 0.015f, 1.0f);
        ourShader.setMat4("transform", trans);
        glDrawArrays(GL_TRIANGLES, 0, 3 * num_triangles);
        ourShader.setVec4("ourColor", 0.f, 0.f, 0.f, 1.0f);
        glBindVertexArray(borderLeafVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 2 * num_triangles);

        // red 8
        trans = glm::translate(trans, glm::vec3(2, -2, 0.0f));
        glBindVertexArray(leafVAO);
        ourShader.setVec4("ourColor", 0.5f, 0.02f, 0.015f, 1.0f);
        ourShader.setMat4("transform", trans);
        glDrawArrays(GL_TRIANGLES, 0, 3 * num_triangles);
        ourShader.setVec4("ourColor", 0.f, 0.f, 0.f, 1.0f);
        glBindVertexArray(borderLeafVAO);
        glDrawArrays(GL_LINE_STRIP, 0, 2 * num_triangles);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &circleVAO);
    glDeleteBuffers(1, &circleVBO);
    glDeleteVertexArrays(1, &borderVAO);
    glDeleteBuffers(1, &borderVBO);

    glDeleteVertexArrays(1, &leafVAO);
    glDeleteBuffers(1, &leafVBO);
    glDeleteVertexArrays(1, &borderLeafVAO);
    glDeleteBuffers(1, &borderLeafVBO);

    delete[] borderLeaf;
    delete[] verticesLeaf;
    delete[] borderCircle;
    delete[] verticesCircle;

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void generateCicle(int n, float*& vertices, float*& border) {
    vertices = new float[n * 3 * 3]{ 0 };
    border = new float[(n + 8) * 2 * 3]{ 0 };
    float division = (2 * M_PI) / n;

    int div = n / 8;
    int k = 0;
    for (size_t i = 0; i < n; i++)
    {
        float aux_c = cos(i * division);
        float aux_s = sin(i * division);
        if (i == div * k)
        {
            ++k;
            border[(n + i / div) * 6] = aux_c;
            border[(n + i / div) * 6 + 1] = aux_s;
            border[(n + i / div) * 6 + 3] = 0.0f;
            border[(n + i / div) * 6 + 4] = 0.0f;
        }

        // vertice 1
        vertices[i * 9] = aux_c;
        vertices[i * 9 + 1] = aux_s;
        border[i * 6] = aux_c;
        border[i * 6 + 1] = aux_s;

        // vertice 2
        if (i == n - 1)
        {
            aux_c = cos(0);
            aux_s = sin(0);
            vertices[i * 9 + 3] = aux_c;
            vertices[i * 9 + 4] = aux_s;
            border[i * 6 + 3] = aux_c;
            border[i * 6 + 4] = aux_s;
        }
        else
        {
            aux_c = cos((i + 1) * division);
            aux_s = sin((i + 1) * division);
            vertices[i * 9 + 3] = aux_c;
            vertices[i * 9 + 4] = aux_s;
            border[i * 6 + 3] = aux_c;
            border[i * 6 + 4] = aux_s;
        }
        // vertice 3
    }
}

void generateLeaf(int n, float*& vertices, float*& border)
{
    vertices = new float[n * 3 * 3]{ 0 };
    border = new float[n * 2 * 3]{ 0 };
    float division = (2.75 * M_PI) / (2 * n);
    float begin = 0;

    for (size_t i = 0; i < n / 2; i++)
    {
        // setting left side
        begin = M_PI / 2;
        glm::vec4 vec(0.0f, 0.0f, 0.0f, 1.0f);
        glm::mat4 trans = glm::mat4(1.0f);

        trans = glm::translate(trans, glm::vec3(.535, .535, 0.0f));
        trans = glm::rotate(trans, glm::radians(73.3f), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::scale(trans, glm::vec3(1.6, 1.6, 1));

        //// vertice 1
        float aux_c = cos(i * division + begin);
        float aux_s = sin(i * division + begin);

        vec.x = aux_c;
        vec.y = aux_s;
        vec = trans * vec;
        vertices[i * 9] = vec.x;
        vertices[i * 9 + 1] = vec.y;
        border[i * 6] = vec.x;
        border[i * 6 + 1] = vec.y;

        // vertice 2
        if (i == n - 1)
        {
            aux_c = cos(0 + begin);
            aux_s = sin(0 + begin);
            vec.x = aux_c;
            vec.y = aux_s;

        }
        else
        {
            aux_c = cos((i + 1) * division + begin);
            aux_s = sin((i + 1) * division + begin);
            vec.x = aux_c;
            vec.y = aux_s;
            vec = trans * vec;
        }
        vertices[i * 9 + 3] = vec.x;
        vertices[i * 9 + 4] = vec.y;
        border[i * 6 + 3] = vec.x;
        border[i * 6 + 4] = vec.y;

        // setting right side
        begin = 3 * M_PI / 2;
        trans = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        trans = glm::translate(trans, glm::vec3(-.535, -.535, 0.0f));
        trans = glm::rotate(trans, glm::radians(73.3f), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::scale(trans, glm::vec3(1.6, 1.6, 1));

        // vertice 4
        aux_c = cos(i * division + begin);
        aux_s = sin(i * division + begin);

        vec.x = aux_c;
        vec.y = aux_s;
        vec = trans * vec;
        vertices[(i + n / 2) * 9] = vec.x;
        vertices[(i + n / 2) * 9 + 1] = vec.y;
        border[(i + n / 2) * 6] = vec.x;
        border[(i + n / 2) * 6 + 1] = vec.y;

        // vertice 5
        if (i == n - 1)
        {
            aux_c = cos(0 + begin);
            aux_s = sin(0 + begin);
            vec.x = aux_c;
            vec.y = aux_s;
        }
        else
        {
            aux_c = cos((i + 1) * division + begin);
            aux_s = sin((i + 1) * division + begin);
            vec.x = aux_c;
            vec.y = aux_s;
            vec = trans * vec;
        }
        vertices[(i + n / 2) * 9 + 3] = vec.x;
        vertices[(i + n / 2) * 9 + 4] = vec.y;
        border[(i + n / 2) * 6 + 3] = vec.x;
        border[(i + n / 2) * 6 + 4] = vec.y;
    }
}

