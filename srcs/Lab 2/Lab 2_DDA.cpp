#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
float black[] = {0.0f, 0.0f, 0.0f, 1.0f};                                 // RGB(0, 0, 0) with full opacity

// Function to draw a line using DDA algorithm
void drawLineDDA(float x1, float y1, float x2, float y2, float *color = black)
{
    // Calculate dx and dy
    float dx = x2 - x1;
    float dy = y2 - y1;

    // Calculate steps needed to reach from (x1, y1) to (x2, y2)
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    // Calculate increment in x and y for each step
    float xIncrement = dx / (float)steps;
    float yIncrement = dy / (float)steps;

    // Starting position
    float x = x1;
    float y = y1;

    // Store coordinates in a vector
    std::vector<std::pair<float, float>> lineCoords;
    for (int i = 0; i <= steps; i++)
    {
        lineCoords.push_back(std::make_pair(x, y));
        x += xIncrement;
        y += yIncrement;
    }
    // Draw the line
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor4fv(color);
    for (auto &coord : lineCoords)
    {
        glVertex2f(coord.first, coord.second);
    }
    glEnd();
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
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Lab 2 - DDA", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

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

        // Draw the line using DDA algorithm
        drawLineDDA(500, 200, 650, 600);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
