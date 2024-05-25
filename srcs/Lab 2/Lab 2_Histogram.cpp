#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
float blueish[] = {2.0f / 255.0f, 84.0f / 255.0f, 166.0f / 255.0f, 1.0f}; // RGB(2,84,166) with full opacity
float black[] = {0.0f, 0.0f, 0.0f, 1.0f};                                 // RGB(0, 0, 0) with full opacity

const int RECTANGULAR_WIDTH = 50;
const int XAXIS_COORDINATE = 50;
const int YAXIS_COORDINATE = 750;
const int MAX_HEIGHT = WINDOW_HEIGHT * 0.8;

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

void drawRectangle(int xBottomLeft, int yBottomLeft, int height)
{
    // Width of the rectangle, you can adjust this as needed
    int width = RECTANGULAR_WIDTH;

    // Coordinates of the other three corners of the rectangle
    int xTopLeft = xBottomLeft;
    int yTopLeft = yBottomLeft - height;
    int xBottomRight = xBottomLeft + width;
    int yBottomRight = yBottomLeft;
    int xTopRight = xBottomLeft + width;
    int yTopRight = yBottomLeft - height;

    // Draw the rectangle
    drawLineDDA(xBottomLeft, yBottomLeft, xTopLeft, yTopLeft);
    drawLineDDA(xTopLeft, yTopLeft, xTopRight, yTopRight);
    drawLineDDA(xTopRight, yTopRight, xBottomRight, yBottomRight);
}

void drawHistogram(const std::vector<int> &frequencies)
{
    // X-axis
    drawLineDDA(XAXIS_COORDINATE - 10, YAXIS_COORDINATE, XAXIS_COORDINATE + 700, YAXIS_COORDINATE, blueish);
    // Y Axis
    drawLineDDA(XAXIS_COORDINATE - 10, YAXIS_COORDINATE, XAXIS_COORDINATE - 10, YAXIS_COORDINATE - 700, blueish);

    // X axis Arrow
    drawLineDDA(XAXIS_COORDINATE + 700, YAXIS_COORDINATE, XAXIS_COORDINATE + 680, YAXIS_COORDINATE + 10, blueish);
    drawLineDDA(XAXIS_COORDINATE + 700, YAXIS_COORDINATE, XAXIS_COORDINATE + 680, YAXIS_COORDINATE - 10, blueish);

    // Y axis Arrow
    drawLineDDA(XAXIS_COORDINATE - 10, YAXIS_COORDINATE - 700, XAXIS_COORDINATE, YAXIS_COORDINATE - 680, blueish);
    drawLineDDA(XAXIS_COORDINATE - 10, YAXIS_COORDINATE - 700, XAXIS_COORDINATE - 20, YAXIS_COORDINATE - 680, blueish);

    int maxHeight = *std::max_element(frequencies.begin(), frequencies.end());

    for (int i = 0; i < frequencies.size(); ++i)
    {
        int barHeight = frequencies[i] * MAX_HEIGHT / maxHeight;
        drawRectangle(XAXIS_COORDINATE + RECTANGULAR_WIDTH * (i + 1), YAXIS_COORDINATE, barHeight);
    }
}
// Function to handle key events
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
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Lab 2 - Histogram", NULL, NULL);
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

        std::vector<int> frequencies = {200, 500, 300, 700, 400, 900, 600};

        // Draw histogram
        drawHistogram(frequencies);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
