#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int main();
void processInput(GLFWwindow* window);
float* generateVerticesCircle(int n);
int* generateIndicesCircle(int n);

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;
const float M_PI = 3.14;
const float scale = 0.125;

// variables for shaders
float offsetx, offsety = 0.f;
float rValue, gValue, bValue = 0.f;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    Shader ourShader("C:/Users/erick/Documents/2022-A/grafica/template/src/laboratorio/3.3.shader.vs", "C:/Users/erick/Documents/2022-A/grafica/template/src/laboratorio/3.3.shader.fs");
    

    int num_triangles = 20;
    float* vertices = generateVerticesCircle(num_triangles);
    int* indices = generateIndicesCircle(num_triangles);


    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (num_triangles + 1) * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_triangles * 3 * sizeof(int), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        //glUseProgram(shaderProgram);
        ourShader.use();

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        ourShader.setVec4("offset", offsetx, offsety, 0.f, 0.f);
        //std::cout << offsetx << ' ' << offsety << "\n";
        rValue = ((-offsetx + 1) / 2) * ((offsety + 1) / 2);
        gValue = ((offsetx + 1) / 2) * ((offsety + 1) / 2);
        bValue = ((-offsetx + 1) / 2) * ((-offsety + 1) / 2);
        ourShader.setVec4("ourColor", rValue, gValue, bValue, 1.0f);


        glDrawElements(GL_TRIANGLES, num_triangles * 3, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time 

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    delete[] vertices;
    delete[] indices;
    
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        if (offsety < 1 - 1 * scale)
            offsety += 0.001f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        if (offsetx > -1 + 1 * scale)
            offsetx -= 0.001f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        if (offsety > -1 + 1 * scale)
            offsety -= 0.001f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        if (offsetx < 1 - 1 * scale)
            offsetx += 0.001f;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

float* generateVerticesCircle(int n)
{
    float division = (2 * M_PI) / n;
    float* vertices = new GLfloat[(n + 1) * 3]{ 0 };

    for (auto i = 0; i < n; i++) {
        vertices[(i + 1) * 3] = cos(i * division) * scale;
        vertices[(i + 1) * 3 + 1] = sin(i * division) * scale;
    }
    return vertices;
}

int* generateIndicesCircle(int n)
{
    int* indices = new int[n * 3]{ 0 };
    for (size_t i = 0; i < n; i++)
    {
        if (i == n - 1)
        {
            indices[i * 3] = n;
            indices[i * 3 + 1] = 1;
        }
        else
        {
            indices[i * 3] = i + 1;
            indices[i * 3 + 1] = i + 2;
        }
    }
    return indices;
}