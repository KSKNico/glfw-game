#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>

#include "render.h"
#include "camera.h"
#include "input.h"
#include "world.h"
#include "player.h"


int main(int argc, char* argv[]) {
    std::cout << "STARTING" << std::endl;
    if (!glfwInit()) {
        std::cout << "glfw initialization failed" << std::endl;
    }
    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Game", NULL, NULL);
    if (!window) {
        std::cout << "Window or OpenGL context creation failed" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    else {
        std::cout << "raw mouse motion not supported, using normal mode" << std::endl;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE);


    Camera camera = Camera(glm::vec3(0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f));





    Player player = Player(glm::vec3(0.0f, 0.0f, 0.0f), camera);

    World world = World(5, player, camera);
    // world.populateWorld(30, 30, 30);

    Renderer renderer = Renderer(*window, camera, world);
    
    input::init(window, &camera, &renderer);

    double lastTime = glfwGetTime();
    int nbFrames = 0;

    // int timer = 0;
    renderer.init();
    while (!glfwWindowShouldClose(window)) {



        // Measure speed
        double currentTime = glfwGetTime();
        nbFrames++;
        if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
            // printf and reset timer
            printf("%f ms/frame\t%i FPS\n", 1000.0/double(nbFrames), nbFrames);
            nbFrames = 0;
            lastTime += 1.0;
            // printf("Player pos: %f %f %f\n", camera.position[0], camera.position[1], camera.position[2]);
        }


        world.update();

        // update other events like input handling 
        glfwPollEvents();

        glClearColor(0.1f, 0.2f, 0.3f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        renderer.render();
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}