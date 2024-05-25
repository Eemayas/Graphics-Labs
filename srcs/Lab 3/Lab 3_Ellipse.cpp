#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

void drawEllipse(int xc, int yc, int rx, int ry)
{
    int x = 0;
    int y = ry;
    int rxSquare = rx * rx;
    int rySquare = ry * ry;
    int rx2 = 2 * rxSquare;
    int ry2 = 2 * rySquare;
    int p;

    std::vector<std::pair<int, int>> ellipsePoints;

    // Region 1
    p = rySquare - rxSquare * ry + 0.25 * rxSquare;
    while (rySquare * x < rxSquare * y)
    {
        x++;
        if (p < 0)
            p += rySquare * (2 * x + 1);
        else
        {
            y--;
            p += rySquare * (2 * x + 1) - rxSquare * 2 * y;
        }
        ellipsePoints.push_back(std::make_pair(xc + x, yc + y));
        ellipsePoints.push_back(std::make_pair(xc - x, yc + y));
        ellipsePoints.push_back(std::make_pair(xc + x, yc - y));
        ellipsePoints.push_back(std::make_pair(xc - x, yc - y));
    }

    // Region 2
    p = rySquare * (x + 0.5) * (x + 0.5) + rxSquare * (y - 1) * (y - 1) - rxSquare * rySquare;
    while (y > 0)
    {
        y--;
        if (p > 0)
            p += rxSquare * (1 - 2 * y);
        else
        {
            x++;
            p += rxSquare * (1 - 2 * y) + rySquare * 2 * x;
        }
        ellipsePoints.push_back(std::make_pair(xc + x, yc + y));
        ellipsePoints.push_back(std::make_pair(xc - x, yc + y));
        ellipsePoints.push_back(std::make_pair(xc + x, yc - y));
        ellipsePoints.push_back(std::make_pair(xc - x, yc - y));
    }

    // Drawing the ellipse
    glPointSize(1);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 0.0f, 0.0f); // Black color
    for (auto &point : ellipsePoints)
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
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Midpoint Ellipse Drawing Algorithm", NULL, NULL);
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

        // Draw an ellipse at the center of the window with semi-major axis 250 and semi-minor axis 150
        drawEllipse(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 250, 150);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
