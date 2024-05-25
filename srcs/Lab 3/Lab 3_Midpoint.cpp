#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

void drawCircleUsingMidpoint(int xc, int yc, int radius)
{
    int x = 0;
    int y = radius;
    int p = 1 - radius;

    std::vector<std::pair<int, int>> circlePoints;

    // Plot the initial point in each octant
    circlePoints.push_back(std::make_pair(xc + x, yc + y));
    circlePoints.push_back(std::make_pair(xc - x, yc + y));
    circlePoints.push_back(std::make_pair(xc + x, yc - y));
    circlePoints.push_back(std::make_pair(xc - x, yc - y));
    circlePoints.push_back(std::make_pair(xc + y, yc + x));
    circlePoints.push_back(std::make_pair(xc - y, yc + x));
    circlePoints.push_back(std::make_pair(xc + y, yc - x));
    circlePoints.push_back(std::make_pair(xc - y, yc - x));

    while (x <= y)
    {
        x++;

        if (p <= 0)
            p += 2 * x + 1;
        else
        {
            y--;
            p += 2 * (x - y) + 1;
        }

        // Plot the points in each octant
        circlePoints.push_back(std::make_pair(xc + x, yc + y));
        circlePoints.push_back(std::make_pair(xc - x, yc + y));
        circlePoints.push_back(std::make_pair(xc + x, yc - y));
        circlePoints.push_back(std::make_pair(xc - x, yc - y));
        circlePoints.push_back(std::make_pair(xc + y, yc + x));
        circlePoints.push_back(std::make_pair(xc - y, yc + x));
        circlePoints.push_back(std::make_pair(xc + y, yc - x));
        circlePoints.push_back(std::make_pair(xc - y, yc - x));
    }

    // Drawing the circle
    glPointSize(5);
    glBegin(GL_POINTS);
    glPointSize(5);
    glColor3f(0.0f, 0.0f, 0.0f); // Black color
    for (auto &point : circlePoints)
    {
        glVertex2i(point.first, point.second);
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
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Midpoint Circle Drawing Algorithm", NULL, NULL);
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
        drawCircleUsingMidpoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 200);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
