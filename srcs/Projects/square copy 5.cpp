#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>

const int WINDOW_WIDTH = 1600;
const int WINDOW_HEIGHT = 600;
float aa = 0;
float ggg = 0;
float bb = 0;
std::vector<std::pair<float, float>> outerPoints;

void precomputeCirclePoints(std::vector<std::pair<float, float>> &circlePoints, float centerX, float centerY, float radius)
{
    circlePoints.reserve(360);
    for (int j = 0; j < 360; ++j)
    {
        float angle = j * (3.14159f / 180.0f);
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);
        circlePoints.emplace_back(x, y);
    }
}

void drawCircle(const std::vector<std::pair<float, float>> &circlePoints, float speed, int loop, GLfloat size, float *i, float centerX, float centerY)
{
    int second_circle = static_cast<int>(*i) % 360;
    int starting_index = (*i - 90 < 0) ? 0 : static_cast<int>(*i) - 90;

    glPointSize(size);
    glBegin(GL_POINTS);
    for (int j = starting_index; j < static_cast<int>(*i); j++)
    {
        glVertex2f(circlePoints[j % 360].first, circlePoints[j % 360].second);
    }
    glEnd();

    *i += (1.0f / speed);

    glBegin(GL_LINES);
    glVertex2f(centerX, centerY);
    glVertex2f(circlePoints[second_circle].first, circlePoints[second_circle].second);
    glEnd();

    if (loop > 0)
    {
        std::vector<std::pair<float, float>> innerCirclePoints;
        precomputeCirclePoints(innerCirclePoints, circlePoints[second_circle].first, circlePoints[second_circle].second, 100.0f / 3);
        drawCircle(innerCirclePoints, circlePoints[second_circle].first / 3, loop - 1, 5, &bb, circlePoints[second_circle].first, circlePoints[second_circle].second);
    }
    else if (loop == 0)
    {
        outerPoints.emplace_back(1100 + ggg * WINDOW_WIDTH, circlePoints[second_circle].second);
        ggg += 1.0f / WINDOW_WIDTH;

        glPointSize(1);
        glBegin(GL_POINTS);
        for (const auto &coord : outerPoints)
        {
            glVertex2f(coord.first, coord.second);
        }
        glEnd();
    }
}

// Function to handle key events
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        aa = 0;
        bb = 0;
        ggg = 0;
        outerPoints.clear();
    }
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Circle Frame by Frame", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Set the key callback
    glfwSetKeyCallback(window, keyCallback);

    // Precompute outer circle points
    std::vector<std::pair<float, float>> circlePoints;
    precomputeCirclePoints(circlePoints, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f, 100.0f);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Clear the buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Set the viewport
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        // Set the projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT, -1.0, 1.0);

        // Set the modelview matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Draw circle frame by frame
        drawCircle(circlePoints, 100.0f, 1, 5, &aa, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
