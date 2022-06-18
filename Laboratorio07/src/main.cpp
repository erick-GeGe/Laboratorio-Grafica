#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

void generateCicle(int, float*&);

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

const float PI_F = 3.14;

float mixture = 0.f;
int background = 0;


int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Laboratorio 07", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader zprogram
    Shader ourShader("src/shaders/vertexShader.glsl", "src/shaders/fragmentShader.glsl");

    int numTraingles = 64;
    float *vertices;
    generateCicle(numTraingles, vertices);
   
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, numTraingles * 5 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // load and create textures
    unsigned int textureBackground;
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); 
    glGenTextures(1, &textureBackground);
    glBindTexture(GL_TEXTURE_2D, textureBackground); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::string src_texture = "../resources/textures/wall02.jpg";
    unsigned char *data = stbi_load(src_texture.c_str() , &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    // texture Github
    unsigned int textureGithub;
    glGenTextures(1, &textureGithub);
    glBindTexture(GL_TEXTURE_2D, textureGithub);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    data = stbi_load("../resources/textures/github.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    ourShader.use(); 
    // ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureBackground);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureGithub);

        // first transformation
        // ---------------------
        
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::scale(transform, glm::vec3(0.4f, 0.4f, 0.4f));
        transform = glm::translate(transform, glm::vec3(-1.25f, 1.25f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        ourShader.setMat4("transform", transform);

        glDrawArrays(GL_TRIANGLES, 0, numTraingles*3);


        // second transformation
        // ---------------------
        transform = glm::mat4(1.0f); 
        transform = glm::scale(transform, glm::vec3(0.4f, 0.4f, 0.4f));
        transform = glm::translate(transform, glm::vec3(1.25f, 1.25f, 0.0f));
        float scaleAmount = static_cast<float>(sin(glfwGetTime()));
        if(scaleAmount < 0.f)
            scaleAmount *= -1;
        transform = glm::scale(transform, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
        ourShader.setMat4("transform", transform);

        glDrawArrays(GL_TRIANGLES, 0, numTraingles*3);


        // third transformation
        // ---------------------
        transform = glm::mat4(1.0f); 
        transform = glm::scale(transform, glm::vec3(0.25f, 0.25f, 0.4f));
        transform = glm::translate(transform, glm::vec3(-2.f, -2.f, 0.0f));
        float scaleAmountx = static_cast<float>(sin(glfwGetTime()));
        float scaleAmounty = static_cast<float>(cos(glfwGetTime()));
        transform = glm::translate(transform, glm::vec3(scaleAmountx, scaleAmounty, 0.f));
        ourShader.setMat4("transform", transform);

        glDrawArrays(GL_TRIANGLES, 0, numTraingles*3);

        // fourth transformation
        // ---------------------
        transform = glm::mat4(1.0f); 
        transform = glm::scale(transform, glm::vec3(0.25f, 0.25f, 0.25f));
        transform = glm::translate(transform, glm::vec3(2.f, -2.f, 0.0f));
        scaleAmount = static_cast<float>(sin(2.f *glfwGetTime()));
        if(scaleAmount < 0.f)
            scaleAmount *= -1;
        transform = glm::translate(transform, glm::vec3(scaleAmountx, scaleAmounty, 0.f));
        transform = glm::rotate(transform, 2.f * (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
        ourShader.setMat4("transform", transform);

        glDrawArrays(GL_TRIANGLES, 0, numTraingles*3);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void generateCicle(int n, float*& vertices) {
    int size_triangle = 3 * 5;
    vertices = new float[n * size_triangle]{ 0 };
    
    float division = (2 * PI_F) / n;

    for (size_t i = 0; i < n; i++)
    {
        // size point = 5
      
        // vertice 1
        float v_cos = cos(i * division);
        float v_sin = sin(i * division);
        vertices[i * size_triangle] = v_cos;
        vertices[i * size_triangle + 1] = v_sin;

        vertices[i * size_triangle + 3] = (v_cos + 1) / 2;
        vertices[i * size_triangle + 4] = (v_sin + 1) / 2;

        // vertice 2
        v_cos = cos((i + 1) * division);
        v_sin = sin((i + 1) * division);
        vertices[i * size_triangle + 5] = v_cos;
        vertices[i * size_triangle + 6] = v_sin;

        vertices[i * size_triangle + 8] = (v_cos + 1) / 2;
        vertices[i * size_triangle + 9] = (v_sin + 1) / 2;  
        
        vertices[i * size_triangle + 13] = 0.5;
        vertices[i * size_triangle + 14] = 0.5; 
    }
}

