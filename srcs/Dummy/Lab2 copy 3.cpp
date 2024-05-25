#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int centerX = WINDOW_WIDTH / 2;
const int centerY = WINDOW_HEIGHT / 2;
float black[] = {0.0f, 0.0f, 0.0f, 1.0f}; // RGB(0, 0, 0) with full opacity

const int RECTANGULAR_WIDTH = 50;
const int MAX_HEIGHT = WINDOW_HEIGHT * 0.8;

// Function to draw a line using DDA algorithm
void drawLineDDA(float x1, float y1, float x2, float y2)
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
    glColor4fv(black);
    for (auto &coord : lineCoords)
    {
        glVertex2f(coord.first, coord.second);
    }
    glEnd();
}

void drawLineBresenham(int x0, int y0, int x1, int y1)
{
    // Swap if starting coordinate is position above the second Coordinate
    if (x0 > x1 || y0 > y1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int x = x0;
    int y = y0;

    std::vector<std::pair<int, int>> lineCoords;
    if (abs(dy) < abs(dx))
    {
        int yi = 1;
        if (dy < 0)
        {
            yi = -1;
            dy = -dy;
        }
        int P = 2 * dy - dx;
        for (int xi = 0; xi <= dx; xi++)
        {
            lineCoords.push_back(std::make_pair(x, y));
            if (P > 0)
            {
                y += yi;
                P -= 2 * dx;
            }
            P += 2 * dy;
            x++;
        }
    }
    else
    {
        int xi = 1;
        if (dx < 0)
        {
            xi = -1;
            dx = -dx;
        }
        int P = 2 * dx - dy;
        for (int yi = 0; yi <= abs(dy); yi++)
        {
            lineCoords.push_back(std::make_pair(x, y));
            if (P > 0)
            {
                x += xi;
                P -= 2 * dy;
            }
            P += 2 * dx;
            y++;
        }
    }
    // // Print the coordinates
    // std::cout << "Line Coordinates:\n";
    // for (auto &coord : lineCoords)
    // {
    //     std::cout << "(" << coord.first << ", " << coord.second << ")\n";
    // }

    // Draw the line
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor4fv(black);
    for (auto &coord : lineCoords)
    {
        glVertex2i(coord.first, coord.second);
    }
    glEnd();
}

void drawRectangle(int x0, int y0, int height)
{
    // Width of the rectangle, you can adjust this as needed
    int width = RECTANGULAR_WIDTH;

    // Coordinates of the other three corners of the rectangle
    int x1 = x0;
    int y1 = y0 - height;
    int x2 = x0 + width;
    int y2 = y0;
    int x3 = x0 + width;
    int y3 = y0 - height;

    // Draw the rectangle
    drawLineDDA(x0, y0, x1, y1);
    drawLineDDA(x1, y1, x3, y3);
    drawLineDDA(x3, y3, x2, y2);
}

void drawHistogram(const std::vector<int> &frequencies)
{
    // X-axis
    drawLineDDA(50 - 10, 750, 50 + 700, 750);
    // Y Axis
    drawLineDDA(50 - 10, 750, 50 - 10, 750 - 700);

    int maxHeight = *std::max_element(frequencies.begin(), frequencies.end());

    for (int i = 0; i < frequencies.size(); ++i)
    {
        int barHeight = frequencies[i] * MAX_HEIGHT / maxHeight;
        drawRectangle(50 + RECTANGULAR_WIDTH * (i + 1), 750, barHeight);
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
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Lab 2", NULL, NULL);
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
        glClearColor(2.0f / 255.0f, 84.0f / 255.0f, 166.0f / 255.0f, 1.0f);
        // Clear the screen with white color
        glClear(GL_COLOR_BUFFER_BIT);

        // Set up the viewport
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 1);

        // // Draw the line using DDA algorithm
        // drawLineDDA(650, 70, 750, 20);

        // // slope is less than 1
        // drawLineBresenham(650, 100, 750, 110);
        // // slope is more than 1
        // drawLineBresenham(650, 200, 750, 600);
        std::vector<int> frequencies = {500, 300, 564, 237, 800, 450, 760};

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
