#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

void generateCicle(int, float*&, float);

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Laboratorio 06", NULL, NULL);
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

    float scale = 0.8f;
    int numTraingles = 64;
    float *vertices;
    generateCicle(numTraingles, vertices, scale);
   
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
    unsigned int textureBackground[5];
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); 
    for (size_t i = 0; i < 5; i++)
    {
        glGenTextures(1, &textureBackground[i]);
        glBindTexture(GL_TEXTURE_2D, textureBackground[i]); 

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        std::string src_texture = "../resources/textures/wall0" + std::to_string(i+1) + ".jpg";
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
    }


    // texture Github
    unsigned int textureGithub;
    glGenTextures(1, &textureGithub);
    glBindTexture(GL_TEXTURE_2D, textureGithub);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    unsigned char *data = stbi_load("../resources/textures/github.png", &width, &height, &nrChannels, 0);
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
        ourShader.setFloat("mixture", mixture);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureBackground[background]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureGithub);

        glDrawArrays(GL_TRIANGLES, 0, numTraingles*3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
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

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        if (mixture < 1.0f)
            mixture += 0.001f; 
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        if (mixture > 0.0f)
            mixture -= 0.001f; 

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        background = 0;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        background = 1;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        background = 2;
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        background = 3;
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        background = 4;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void generateCicle(int n, float*& vertices, float scale) {
    int size_triangle = 3 * 5;
    vertices = new float[n * size_triangle]{ 0 };
    
    float division = (2 * PI_F) / n;

    for (size_t i = 0; i < n; i++)
    {
        // size point = 5
      
        // vertice 1
        float v_cos = cos(i * division);
        float v_sin = sin(i * division);
        vertices[i * size_triangle] = v_cos * scale;
        vertices[i * size_triangle + 1] = v_sin * scale;

        vertices[i * size_triangle + 3] = (v_cos + 1) / 2;
        vertices[i * size_triangle + 4] = (v_sin + 1) / 2;

        // vertice 2
        v_cos = cos((i + 1) * division);
        v_sin = sin((i + 1) * division);
        vertices[i * size_triangle + 5] = v_cos * scale;
        vertices[i * size_triangle + 6] = v_sin * scale;  

        vertices[i * size_triangle + 8] = (v_cos + 1) / 2;
        vertices[i * size_triangle + 9] = (v_sin + 1) / 2;  
        
        vertices[i * size_triangle + 13] = 0.5;
        vertices[i * size_triangle + 14] = 0.5; 
    }
}

