#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>
#include <vector>

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 1000;

float black[] = {0.0f, 0.0f, 0.0f, 1.0f};

void drawLineDDA(float x1, float y1, float x2, float y2)
{

    float dx = x2 - x1;
    float dy = y2 - y1;

    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float x_inc = dx / (float)steps;
    float y_inc = dy / (float)steps;

    float x = x1;
    float y = y1;

    std::vector<std::pair<float, float>> lineCoords;
    for (int i = 0; i <= steps; i++)
    {
        lineCoords.push_back(std::make_pair(x, y));
        x += x_inc;
        y += y_inc;
    }

    glPointSize(5);
    glBegin(GL_POINTS);
    glColor4fv(black);
    for (auto &coord : lineCoords)
    {
        glVertex2f(coord.first, coord.second);
    }
    glEnd();
}

void drawLine(int x0, int y0, int x1, int y1)
{

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
        int D = 2 * dx - dy;
        for (int yi = 0; yi <= abs(dy); yi++)
        {
            lineCoords.push_back(std::make_pair(x, y));
            if (D > 0)
            {
                x += xi;
                D -= 2 * dy;
            }
            D += 2 * dx;
            y++;
        }
    }

    glPointSize(5);
    glBegin(GL_POINTS);
    glColor4fv(black);
    for (auto &coord : lineCoords)
    {
        glVertex2i(coord.first, coord.second);
    }
    glEnd();
}

void drawRectangle(int x, int y, int height)
{

    int width = 50;

    int x1 = x + width;
    int y1 = y;
    int x2 = x + width;
    int y2 = y - height;
    int x3 = x;
    int y3 = y - height;

    glPointSize(5);
    glBegin(GL_LINE_LOOP);
    glColor4fv(black);
    glVertex2i(x, y);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glVertex2i(x3, y3);
    glEnd();
}

void drawHistogram(const std::vector<int> &frequencies)
{

    int width = frequencies.size();
    int height = *std::max_element(frequencies.begin(), frequencies.end());
    for (int i = 0; i < width; ++i)
    {

        int barHeight = frequencies[i];
        drawRectangle(20 + RECTANGULAR_WIDTH * (i + 1), 800, barHeight);
    }
}

int main()
{
    GLFWwindow *window;

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "KU Logo", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, keyCallback);

    while (!glfwWindowShouldClose(window))
    {

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 1);

        float x1 = 200, y1 = 300, x2 = 200, y2 = 299;

        drawLineDDA(20, 70, 20, 20);

        drawLine(60, 20, 30, 20);
        std::vector<int> frequencies = {20, 50, 30, 70, 40, 90, 60};

        drawHistogram(frequencies);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
