#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
float black[] = {0.0f, 0.0f, 0.0f, 1.0f}; // RGB(0, 0, 0) with full opacity

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
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Lab 2 - Bresenham", NULL, NULL);
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

        // slope is less than 1
        drawLineBresenham(350, 100, 550, 110);
        // slope is more than 1
        drawLineBresenham(350, 200, 550, 600);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
