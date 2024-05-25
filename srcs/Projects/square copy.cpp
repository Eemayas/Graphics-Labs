#include <GLFW/glfw3.h>
#include <iostream>`
#include <cmath>
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
float aa = 0;
float bb = 0;
// void drawCircle(float centerX, float centerY, float radius)
// {
//     glPointSize(10);
//     glBegin(GL_POINTS);
//     glColor3f(1.0f, 1.0f, 1.0f);

//     for (int j = 0; j < 20; j++)
//     {
//         float angle = (i + j) * (3.14159f / 180.0f);
//         float x = centerX + radius * cos(angle);
//         float y = centerY + radius * sin(angle);
//         glVertex2f(x, y);
//     }
//     i = i + 1 / radius;
//     glEnd();
// }
void drawCircle(float centerX, float centerY, float radius, float speed, bool loop, GLfloat size, float i)
{
    std::vector<std::pair<int, int>> circlePoints;
    glPointSize(size);
    for (int i = 0; i < 360; ++i)
    {
        float angle = i * (3.14159f / 180.0f);
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);
        circlePoints.push_back(std::make_pair(x, y));
        // glVertex2f(x, y);
    }

    int second_circle = (int)i % 360;

    // int starting_index = i - 180 < 0 ? 0 : i - 180;
    int starting_index = 0;
    glBegin(GL_POINTS);
    for (int j = starting_index; j < i; j++)
    {
        glVertex2f(circlePoints[j % 360].first, circlePoints[j % 360].second);
    }

    aa += (1 / speed) / 2;
    bb += (1 / speed);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(centerX, centerY);
    glVertex2f(circlePoints[second_circle].first, circlePoints[second_circle].second);
    glEnd();
    if (loop)
    {
        std::cout << "(" << circlePoints[second_circle % 360].first << ", " << circlePoints[second_circle % 360].second << ")\n";
        drawCircle(circlePoints[second_circle].first, circlePoints[second_circle].second, radius / 2, 5, false, 2, bb);
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
        float radius = 100.0f;
        float centerX = WINDOW_WIDTH / 2.0f;
        float centerY = WINDOW_HEIGHT / 2.0f;
        drawCircle(centerX, centerY, radius, 20, true, 5, aa);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
