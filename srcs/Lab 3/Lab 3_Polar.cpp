#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

void drawCircleUsingPolarCoordinate(float xc, float yc, float radius)
{
    std::vector<std::pair<float, float>> circlePoints;

    // Iterate through 0 to 360 degrees
    for (float theta = 0; theta <= 360; theta = theta + 1 / radius)
    {
        // Convert degrees to radians
        float radian = theta * (M_PI / 180.0);

        // Calculate x and y using polar coordinate system
        float x = xc + radius * cos(radian);
        float y = yc + radius * sin(radian);

        // Store the points
        circlePoints.push_back(std::make_pair(x, y));
    }

    // Drawing the circle
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 0.0f, 0.0f); // Black color
    for (auto &point : circlePoints)
    {
        glVertex2f(point.first, point.second);
    }
    glEnd();
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main()
{
    GLFWwindow *window;

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Polar Coordinate Circle Drawing Algorithm", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Set the key callback
    glfwSetKeyCallback(window, keyCallback);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Set the background color to white
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        // Clear the screen with white color
        glClear(GL_COLOR_BUFFER_BIT);

        // Set up the viewport
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 1);

        // Draw a circle at the center of the window with radius 200
        drawCircleUsingPolarCoordinate(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 200);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
