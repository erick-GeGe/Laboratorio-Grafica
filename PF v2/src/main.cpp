#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "cube.h"
#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

void getCubesToRotate(std::vector<Cube *> *cube_to_rotate, std::vector<Cube> &cubes, int num_cubes);
vec3 getEjeToRotate();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float x_eye = 0.f;
float y_eye = 0.f;
float z_eye = 0.f;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 8.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw_v = -90.0f; // yaw_v is initialized to -90.0 degrees since a yaw_v of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch_v = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

bool rotating, rotating_u, rotating_i, rotating_o, rotating_j, rotating_k, rotating_l, rotating_y, rotating_h, rotating_p, rotating_coma, rotating_comilla, rotating_llave = false;

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
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rubik's Cube", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("src/shaders/vertexShader.glsl", "src/shaders/fragmentShader.glsl");

    glm::vec3 colors[6] = {
        glm::vec3(0.73, 0.02, 0.02), // 0 Red
        glm::vec3(0.85, 0.85, 0.85), // 1 White
        glm::vec3(0.96, 0.46, 0.14), // 2 Orange
        glm::vec3(0.02, 0.37, 0.68), // 3 Bkue
        glm::vec3(0.29, 0.68, 0.02), // 4 Green
        glm::vec3(0.89, 0.69, 0.03), // 5 Yellow
    };

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        // Caras mas pequenias
        // 0 FORWARD
        -0.45f, -0.45f,  0.5005f,
         0.45f, -0.45f,  0.5005f,
         0.45f,  0.45f,  0.5005f,
         0.45f,  0.45f,  0.5005f,
        -0.45f,  0.45f,  0.5005f,
        -0.45f, -0.45f,  0.5005f,
        // 1 BACKWARD
        -0.45f, -0.45f, -0.5005f,
         0.45f, -0.45f, -0.5005f,
         0.45f,  0.45f, -0.5005f,
         0.45f,  0.45f, -0.5005f,
        -0.45f,  0.45f, -0.5005f,
        -0.45f, -0.45f, -0.5005f,
        // 2 RIGHT
         0.5005f,  0.45f,  0.45f,
         0.5005f,  0.45f, -0.45f,
         0.5005f, -0.45f, -0.45f,
         0.5005f, -0.45f, -0.45f,
         0.5005f, -0.45f,  0.45f,
         0.5005f,  0.45f,  0.45f,
        // 3 LEFT
        -0.5005f,  0.45f,  0.45f,
        -0.5005f,  0.45f, -0.45f,
        -0.5005f, -0.45f, -0.45f,
        -0.5005f, -0.45f, -0.45f,
        -0.5005f, -0.45f,  0.45f,
        -0.5005f,  0.45f,  0.45f,
        // 4 TOP
        -0.45f,  0.5005f, -0.45f,
         0.45f,  0.5005f, -0.45f,
         0.45f,  0.5005f,  0.45f,
         0.45f,  0.5005f,  0.45f,
        -0.45f,  0.5005f,  0.45f,
        -0.45f,  0.5005f, -0.45f,
        // 5  BOT
        -0.45f, -0.5005f, -0.45f,
         0.45f, -0.5005f, -0.45f,
         0.45f, -0.5005f,  0.45f,
         0.45f, -0.5005f,  0.45f,
        -0.45f, -0.5005f,  0.45f,
        -0.45f, -0.5005f, -0.45f,

    };
    // world space positions of our cubes
    struct CubeData
    {
        glm::vec3 position_cube;
        int colors[3];
        int faces[3];
        int n_faces;
    };
    // glm::mat4 rotation_aux = rotate(glm::radians(90.f), vec3(0,0,-1));
    int num_cubes = 26;

    CubeData cube_data[26] = {
        // FORWARD-0 RED-0
        // LEFT-3 BLUE-3
        // TOP-4 GREEN-4
        // RIGHT-2 ORANGE-2
        // BOT-5 YELLOW-5
        // BACKWARD-1 WHITE-1

        //  POSITION            COLORS      FACES   NUM_FACES
        {glm::vec3(-1, 1, 1), {0, 4, 3}, {0, 4, 3}, 3},  // LEFT-TOP-FORWARD
        {glm::vec3(0, 1, 1), {0, 4, 0}, {0, 4, 0}, 2},   // TOP-FORWARD
        {glm::vec3(1, 1, 1), {0, 2, 4}, {0, 2, 4}, 3},   // RIGHT-TOP-FORWARD
        {glm::vec3(-1, 0, 1), {0, 3, 0}, {0, 3, 0}, 2},  // LEFT-FORWARD
        {glm::vec3(0, 0, 1), {0, 0, 0}, {0, 0, 0}, 1},   // FORWARD
        {glm::vec3(1, 0, 1), {0, 2, 0}, {0, 2, 0}, 2},   // RIGHT-FORWARD
        {glm::vec3(-1, -1, 1), {0, 3, 5}, {0, 3, 5}, 3}, // LEFT-BOT-FORWARD
        {glm::vec3(0, -1, 1), {0, 5, 0}, {0, 5, 0}, 2},  // BOT-FORWARD
        {glm::vec3(1, -1, 1), {0, 2, 5}, {0, 2, 5}, 3},  // RIGHT-BOT-FORWARD

        {glm::vec3(-1, 1, 0), {3, 4, 0}, {3, 4, 0}, 2},  // LEFT-TOP
        {glm::vec3(0, 1, 0), {4, 0, 0}, {4, 0, 0}, 1},   // TOP
        {glm::vec3(1, 1, 0), {2, 4, 0}, {2, 4, 0}, 2},   // RIGHT-TOP
        {glm::vec3(-1, 0, 0), {3, 0, 0}, {3, 0, 0}, 1},  // LEFTz
        {glm::vec3(1, 0, 0), {2, 0, 0}, {2, 0, 0}, 1},   // RIGHT
        {glm::vec3(-1, -1, 0), {3, 5, 0}, {3, 5, 0}, 2}, // LEFT-BOT
        {glm::vec3(0, -1, 0), {5, 0, 0}, {5, 0, 0}, 1},  // BOT
        {glm::vec3(1, -1, 0), {2, 5, 0}, {2, 5, 0}, 2},  // RIGHT-BOT

        {glm::vec3(-1, 1, -1), {3, 4, 1}, {3, 4, 1}, 3},  // LEFT-TOP-BACKWARD
        {glm::vec3(0, 1, -1), {4, 1, 0}, {4, 1, 0}, 2},   // TOP-BACKWARD
        {glm::vec3(1, 1, -1), {2, 4, 1}, {2, 4, 1}, 3},   // RIGHT-TOP-BACKWARD
        {glm::vec3(-1, 0, -1), {3, 1, 0}, {3, 1, 0}, 2},  // LEFT-BACKWARD
        {glm::vec3(0, 0, -1), {1, 0, 0}, {1, 0, 0}, 1},   // BACKWARD
        {glm::vec3(1, 0, -1), {2, 1, 0}, {2, 1, 0}, 2},   // RIGHT-BACKWARD
        {glm::vec3(-1, -1, -1), {3, 5, 1}, {3, 5, 1}, 3}, // LEFT-BOT-BACKWARD
        {glm::vec3(0, -1, -1), {5, 1, 0}, {5, 1, 0}, 2},  // BOT-BACKWARD
        {glm::vec3(1, -1, -1), {2, 5, 1}, {2, 5, 1}, 3},  // RIGHT-BOT-BACKWARD
    };

    glm::vec3 colors_face[3];
    std::vector<Cube> cubes;
    int faces[3] = {0, 0, 0};
    for (size_t i = 0; i < num_cubes; i++)
    {
        for (size_t j = 0; j < 3; j++)
            colors_face[j] = colors[cube_data[i].colors[j]];

        cubes.push_back(Cube(cube_data[i].position_cube, colors_face, cube_data[i].faces, cube_data[i].n_faces));
    }

    float angle = 0.f;
    float _PI = 3.14;
    float velocity = 0.3f;
    bool calculated = false;
    std::vector<Cube *> cubes_rotation;

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute

    ourShader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.46f, 0.46f, 0.53f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        ourShader.use();
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);
        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);

        // render boxes
        glBindVertexArray(VAO);

        if (rotating)
        {
            if (!calculated)
                getCubesToRotate(&cubes_rotation, cubes, num_cubes);
            glm::vec3 eje = getEjeToRotate();

            angle = fmin(angle + velocity, 90.f);
            mat4 newRotation = rotate(glm::radians(angle), eje);
            for (size_t i = 0; i < cubes_rotation.size(); i++)
                cubes_rotation[i]->Rotate = newRotation * cubes_rotation[i]->init_rotate;

            if (angle == 90.f)
            {
                rotating = rotating_u = rotating_i = rotating_o = rotating_j = rotating_k = rotating_l = rotating_y = rotating_h = rotating_p = rotating_coma = rotating_llave = rotating_comilla = false;
                angle = 0.f;
                for (size_t i = 0; i < cubes_rotation.size(); i++)
                {
                    cubes_rotation[i]->Rotate = newRotation * cubes_rotation[i]->init_rotate;
                    cubes_rotation[i]->init_pos = vec3(newRotation * vec4(cubes_rotation[i]->init_pos, 0));
                    cubes_rotation[i]->init_rotate = cubes_rotation[i]->Rotate;
                }
            }
        }

        for (size_t i = 0; i < num_cubes; i++)
            cubes[i].Draw(&ourShader, projection, view);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void getCubesToRotate(std::vector<Cube *> *cubes_to_rotate, std::vector<Cube> &cubes, int num_cubes)
{
    cubes_to_rotate->clear();
    for (size_t i = 0; i < num_cubes; i++)
    {
        if (rotating_y || rotating_h)
            if (cubes[i].init_pos.x < -0.9)
                cubes_to_rotate->push_back(&cubes[i]);

        if (rotating_comilla || rotating_llave)
            if (cubes[i].init_pos.x > 0.9)
                cubes_to_rotate->push_back(&cubes[i]);

        if (rotating_i || rotating_k)
            if (cubes[i].init_pos.y > 0.9)
                cubes_to_rotate->push_back(&cubes[i]);
        if (rotating_o || rotating_l)
            if (cubes[i].init_pos.y < -0.9)
                cubes_to_rotate->push_back(&cubes[i]);

        if (rotating_p || rotating_coma)
            if (cubes[i].init_pos.z > 0.9)
                cubes_to_rotate->push_back(&cubes[i]);
        if (rotating_u || rotating_j)
            if (cubes[i].init_pos.z < -0.9)
                cubes_to_rotate->push_back(&cubes[i]);
    }
}

vec3 getEjeToRotate()
{
    glm::vec3 eje = glm::vec3(0, 0, 0);
    if (rotating_y || rotating_llave)
        eje = glm::vec3(1, 0, 0);
    if (rotating_h || rotating_comilla)
        eje = glm::vec3(-1, 0, 0);
    if (rotating_i || rotating_o)
        eje = glm::vec3(0, 1, 0);
    if (rotating_k || rotating_l)
        eje = glm::vec3(0, -1, 0);
    if (rotating_u || rotating_p)
        eje = glm::vec3(0, 0, 1);
    if (rotating_j || rotating_coma)
        eje = glm::vec3(0, 0, -1);
    return eje;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(5 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos += cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPos -= cameraUp * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
        if (!rotating)
            rotating = rotating_llave = true;
    if (glfwGetKey(window, GLFW_KEY_APOSTROPHE) == GLFW_PRESS)
        if (!rotating)
            rotating = rotating_comilla = true;
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        if (!rotating)
            rotating = rotating_y = true;
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        if (!rotating)
            rotating = rotating_h = true;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        if (!rotating)
            rotating = rotating_p = true;
    if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS)
        if (!rotating)
            rotating = rotating_coma = true;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        if (!rotating)
            rotating = rotating_u = true;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        if (!rotating)
            rotating = rotating_i = true;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        if (!rotating)
            rotating = rotating_o = true;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        if (!rotating)
            rotating = rotating_j = true;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        if (!rotating)
            rotating = rotating_k = true;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        if (!rotating)
            rotating = rotating_l = true;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw_v += xoffset;
    pitch_v += yoffset;

    // make sure that when pitch_v is out of bounds, screen doesn't get flipped
    if (pitch_v > 89.0f)
        pitch_v = 89.0f;
    if (pitch_v < -89.0f)
        pitch_v = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw_v)) * cos(glm::radians(pitch_v));
    front.y = sin(glm::radians(pitch_v));
    front.z = sin(glm::radians(yaw_v)) * cos(glm::radians(pitch_v));
    cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}