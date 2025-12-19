#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "constants.hpp"
#include "input.hpp"
#include "shader.hpp"
#include "world.hpp"
#include "player.hpp"

constexpr int START_SCR_WIDTH = 800, START_SCR_HEIGHT = 600;
int scr_width, scr_height;

void framebuffer_size_callback(GLFWwindow*, int width, int height);
float xoffset, yofset;
float deltaTime = 0.0f, lastFrame = 0.0f;

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(START_SCR_WIDTH, START_SCR_HEIGHT, "Voxel Engine", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialise GLAD." << std::endl;
        return -1;
    }
    glViewport(0, 0, scr_width, scr_height);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int fbw, fbh;
    glfwGetFramebufferSize(window, &fbw, &fbh);
    framebuffer_size_callback(window, fbw, fbh);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (CULL_BACK_FACES) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CCW);
    }
    if (WIREFRAMES) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Input::get().init(window);
    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    World world;
    Player player(glm::vec3(8.f, 200.f, 8.f), &world);

    while (!glfwWindowShouldClose(window)) {
        float time = glfwGetTime();
        deltaTime = time - lastFrame;
        lastFrame = time;

        std::cout << 1 / deltaTime << "\n";

        Input::get().update();
        auto& input = Input::get();
        if (input.key(GLFW_KEY_ESCAPE)) { glfwSetWindowShouldClose(window, 1); }
        player.update(window, input.getXOffset(), input.getYOffset(), deltaTime);

        glClearColor(0.1f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(50.0f), (float)scr_width / (float)scr_height, 0.1f, 1000.f);
        glm::mat4 view = player.getViewMatrix();

        shader.use();
        shader.setMat4("uView", view);
        shader.setMat4("uProjection", projection);
        
        world.update();
        world.render(&shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
    scr_width = width;
    scr_height = height;
}