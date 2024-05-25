#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <cmath>
#include <string>

#define segments 12

float awayFromCircle[segments][2];
float nearToCircle[segments][2];

int endpointIndex = 0;

const int total_width = 800;
const int total_height = 800;

float skyBlue[] = {0.529f, 0.808f, 0.922f, 1.0f};
float yellow[] = {1.0f, 1.0f, 0.0f, 1.0f};
float black[] = {0.0f, 0.0f, 0.0f, 1.0f};
float red[] = {1.0f, 0.0f, 0.0f, 1.0f};
float lightPink[] = {1.0f, 0.714f, 0.757f, 1.0f};

void drawCircle(float cx, float cy, float r, int segment, float *color)
{
    glBegin(GL_TRIANGLE_FAN);
    glColor4fv(color);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segment; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(segment);
        float x = r * cosf(theta) + cx;
        float y = r * sinf(theta) + cy;
        glVertex2f(x, y);
    }
    glEnd();
}

void perpendicularlInes(float cx, float cy, float r, int num_segments)
{
    glLineWidth(10.0f);
    glBegin(GL_LINES);
    glColor4fv(black);
    float deltaTheta = 2.0f * 3.1415926f / float(num_segments);
    float theta = 0.0f;
    for (int i = 0; i < num_segments; i++)
    {
        float x1 = r * cosf(theta) + cx;
        float y1 = r * sinf(theta) + cy;
        float x2 = (r + 50) * cosf(theta) + cx;
        float y2 = (r + 50) * sinf(theta) + cy;
        nearToCircle[i][0] = x1;
        nearToCircle[i][1] = y1;
        awayFromCircle[i][0] = x2;
        awayFromCircle[i][1] = y2;
        theta += deltaTheta;
    }
    glEnd();
}

void redSection()
{
    glLineWidth(10.0f);
    glBegin(GL_POLYGON);
    glColor4fv(red);
    for (int i = 0; i < segments; i++)
    {
        float x1 = awayFromCircle[i][0];
        float y1 = awayFromCircle[i][1];
        float x2 = awayFromCircle[(i + 1) % segments][0];
        float y2 = awayFromCircle[(i + 1) % segments][1];

        float midX2 = x1 + (x2 - x1) * 0.5f;
        float midY2 = y1 + (y2 - y1) * 0.5f;

        float X2 = midX2;
        float Y2 = midY2;

        glVertex2f(x1, y1);

        glVertex2f(X2, Y2);
        glVertex2f(X2, Y2);

        glVertex2f(x2, y2);
    }
    glEnd();
}

void borderTriangle(float cx, float cy, float r_outer, float r_inner, float *color)
{
    glBegin(GL_TRIANGLES);
    glColor4fv(black);
    float x1_outer, y1_outer, x2_outer, y2_outer, x3_outer, y3_outer;
    x1_outer = cx;
    y1_outer = cy + r_outer;
    x2_outer = cx - r_outer * sqrt(3) / 2;
    y2_outer = cy - r_outer / 2;
    x3_outer = cx + r_outer * sqrt(3) / 2;
    y3_outer = cy - r_outer / 2;
    glVertex2f(x1_outer, y1_outer);
    glVertex2f(x2_outer, y2_outer);
    glVertex2f(x3_outer, y3_outer);
    glColor4fv(color);
    float x1_inner, y1_inner, x2_inner, y2_inner, x3_inner, y3_inner;
    x1_inner = cx;
    y1_inner = cy + r_inner;
    x2_inner = cx - r_inner * sqrt(3) / 2;
    y2_inner = cy - r_inner / 2;
    x3_inner = cx + r_inner * sqrt(3) / 2;
    y3_inner = cy - r_inner / 2;
    glVertex2f(x1_inner, y1_inner);
    glVertex2f(x2_inner, y2_inner);
    glVertex2f(x3_inner, y3_inner);
    glEnd();
}

int main()
{
    GLFWwindow *window;

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    window = glfwCreateWindow(total_width, total_height, "Examples", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, total_width, total_height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, total_width, total_height, 0, 0, 1);
        glColor3f(1.0f, 1.0f, 1.0f);

        perpendicularlInes(total_width / 2, total_height / 2, 300 + 10, segments);

        redSection();

        drawCircle(total_width / 2, total_height / 2, 310, 100, black);
        drawCircle(total_width / 2, total_height / 2, 300, 100, skyBlue);

        drawCircle(total_width / 2, total_height / 2, 260, 100, black);
        drawCircle(total_width / 2, total_height / 2, 250, 100, yellow);

        borderTriangle(total_width / 2, total_height / 2, 250, 240, lightPink);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
