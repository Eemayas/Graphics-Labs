#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <cmath>
#define MAX_SEGMENTS 12 // Define the maximum number of segments

// Define an array to hold the endpoints of the lines
float lineEndpointsAwayFromCircle[MAX_SEGMENTS][2]; // Each row represents an endpoint [x, y]
float lineEndpointsNearToCircle[MAX_SEGMENTS][2];   // Each row represents an endpoint [x, y]

int endpointIndex = 0; // Track the index of the current endpoint

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 1000;
const int centerX = WINDOW_WIDTH / 2;
const int centerY = WINDOW_HEIGHT / 2;

const int border_width = 10;

float skyBlue[] = {169.0f / 255.0f, 198.0f / 255.0f, 228.0f / 255.0f}; // RGB(169, 198, 228)
// Define an array to hold RGB values for the color (244, 232, 112)
float yellow[] = {244.0f / 255.0f, 232.0f / 255.0f, 112.0f / 255.0f}; // RGB(244, 232, 112)
// Define an array to hold RGB values for the color (0, 0, 0)
float black[] = {0.0f, 0.0f, 0.0f}; // RGB(0, 0, 0)
float white[] = {1.0f, 1.0f, 1.0f}; // RGB(0, 0, 0)

float lightPink[] = {253.0f / 255.0f, 173.0f / 255.0f, 172.0f / 255.0f};

void drawCircle(float cx, float cy, float r, int num_segments, float red, float green, float blue)
{
    // Draw the filled circle
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(red, green, blue); // Set the color for the filled circle
    glVertex2f(cx, cy);          // Center of the circle
    for (int i = 0; i <= num_segments; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments); // get the current angle
        float x = r * cosf(theta) + cx;                                   // calculate the x component relative to center cx
        float y = r * sinf(theta) + cy;                                   // calculate the y component relative to center cy
        glVertex2f(x, y);                                                 // output vertex
    }
    glEnd();
    // drawCircle(cx, cy, r + 10, num_segments, 0.0f, 0.0f, 0.0f);
}

void drawLinesPerpendicularToCircle(float cx, float cy, float r, int num_segments)
{
    glLineWidth(10.0f); // Set the width of the lines to 10 units

    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f); // Set the color of the lines to black

    float deltaTheta = 2.0f * 3.1415926f / float(num_segments);
    float theta = 0.0f;

    for (int i = 0; i < num_segments; i++)
    {
        float x1 = r * cosf(theta) + cx;
        float y1 = r * sinf(theta) + cy;

        float x2 = (r + 50) * cosf(theta) + cx;
        float y2 = (r + 50) * sinf(theta) + cy;

        //     // Store the endpoints in the array
        lineEndpointsNearToCircle[i][0] = x1;
        lineEndpointsNearToCircle[i][1] = y1;
        lineEndpointsAwayFromCircle[i][0] = x2;
        lineEndpointsAwayFromCircle[i][1] = y2;

        // // Draw a line from (x1, y1) to (x2, y2)
        // glVertex2f(x1, y1);
        // glVertex2f(x2, y2);

        theta += deltaTheta;
    }

    glEnd();
}

void drawOuterRedSection()
{
    glLineWidth(10.0f); // Set the width of the lines to 10 units

    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.0f, 0.0f); // Set the color to red

    // Draw lines between consecutive points in the lineEndpoints array
    for (int i = 0; i < MAX_SEGMENTS; i++)
    {
        float x1 = lineEndpointsAwayFromCircle[i][0];
        float y1 = lineEndpointsAwayFromCircle[i][1];

        float x2 = lineEndpointsAwayFromCircle[(i + 1) % MAX_SEGMENTS][0];
        float y2 = lineEndpointsAwayFromCircle[(i + 1) % MAX_SEGMENTS][1];

        // Calculate the midpoints between x1 and x2, and y1 and y2
        float midX1 = x1 + (x2 - x1) * 0.25f;
        float midY1 = y1 + (y2 - y1) * 0.25f;
        float midX2 = x1 + (x2 - x1) * 0.5f;
        float midY2 = y1 + (y2 - y1) * 0.5f;
        float midX3 = x1 + (x2 - x1) * 0.75f;
        float midY3 = y1 + (y2 - y1) * 0.75f;

        // Calculate the control points for the curves
        float controlX1 = midX1 + (y2 - y1) * 0.1f;  // Offset control point in the direction of the outer curve
        float controlY1 = midY1 - (x2 - x1) * 0.1f;  // Offset control point in the direction of the outer curve
        float controlX2 = midX2 + (y2 - y1) * 0.15f; // Offset control point in the direction of the outer curve
        float controlY2 = midY2 - (x2 - x1) * 0.15f; // Offset control point in the direction of the outer curve
        float controlX3 = midX3 + (y2 - y1) * 0.1f;  // Offset control point in the direction of the outer curve
        float controlY3 = midY3 - (x2 - x1) * 0.1f;  // Offset control point in the direction of the outer curve

        // Draw quadratic Bézier curves for each part
        glVertex2f(x1, y1);
        glVertex2f(controlX1, controlY1);
        glVertex2f(controlX1, controlY1);
        glVertex2f(controlX2, controlY2);
        glVertex2f(controlX2, controlY2);
        glVertex2f(controlX3, controlY3);
        glVertex2f(controlX3, controlY3);
        glVertex2f(x2, y2);
    }
    glEnd();

    glLineWidth(10.0f); // Set the width of the lines to 10 units

    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f); // Set the color of the lines to black

    // Draw lines between consecutive points in the lineEndpoints array
    for (int i = 0; i < MAX_SEGMENTS; i++)
    {
        float x1 = lineEndpointsAwayFromCircle[i][0];
        float y1 = lineEndpointsAwayFromCircle[i][1];

        float x2 = lineEndpointsAwayFromCircle[(i + 1) % MAX_SEGMENTS][0];
        float y2 = lineEndpointsAwayFromCircle[(i + 1) % MAX_SEGMENTS][1];

        // Calculate the midpoints between x1 and x2, and y1 and y2
        float midX1 = x1 + (x2 - x1) * 0.25f;
        float midY1 = y1 + (y2 - y1) * 0.25f;
        float midX2 = x1 + (x2 - x1) * 0.5f;
        float midY2 = y1 + (y2 - y1) * 0.5f;
        float midX3 = x1 + (x2 - x1) * 0.75f;
        float midY3 = y1 + (y2 - y1) * 0.75f;

        // Calculate the control points for the curves
        float controlX1 = midX1 + (y2 - y1) * 0.1f;  // Offset control point in the direction of the outer curve
        float controlY1 = midY1 - (x2 - x1) * 0.1f;  // Offset control point in the direction of the outer curve
        float controlX2 = midX2 + (y2 - y1) * 0.15f; // Offset control point in the direction of the outer curve
        float controlY2 = midY2 - (x2 - x1) * 0.15f; // Offset control point in the direction of the outer curve
        float controlX3 = midX3 + (y2 - y1) * 0.1f;  // Offset control point in the direction of the outer curve
        float controlY3 = midY3 - (x2 - x1) * 0.1f;  // Offset control point in the direction of the outer curve

        // Draw quadratic Bézier curves for each part
        glVertex2f(x1, y1);
        glVertex2f(controlX1, controlY1);
        glVertex2f(controlX1, controlY1);
        glVertex2f(controlX2, controlY2);
        glVertex2f(controlX2, controlY2);
        glVertex2f(controlX3, controlY3);
        glVertex2f(controlX3, controlY3);
        glVertex2f(x2, y2);
    }

    for (int i = 0; i < MAX_SEGMENTS; i++)
    {
        float x1 = lineEndpointsNearToCircle[i][0];
        float y1 = lineEndpointsNearToCircle[i][1];

        float x2 = lineEndpointsAwayFromCircle[i][0];
        float y2 = lineEndpointsAwayFromCircle[i][1];

        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    }

    glEnd();
}

void drawEquilateralTriangle(float cx, float cy, float r, float red, float green, float blue, bool inverted)
{
    glBegin(GL_TRIANGLES);

    // Set the color for the triangle
    glColor3f(red, green, blue);

    // Calculate the coordinates of the three vertices of the equilateral triangle
    float x1, y1, x2, y2, x3, y3;

    if (!inverted)
    {
        x1 = cx;
        y1 = cy - r;

        x2 = cx - r * sqrt(3) / 2;
        y2 = cy + r / 2;

        x3 = cx + r * sqrt(3) / 2;
        y3 = cy + r / 2;
    }
    else
    {
        x1 = cx;
        y1 = cy + r;

        x2 = cx - r * sqrt(3) / 2;
        y2 = cy - r / 2;

        x3 = cx + r * sqrt(3) / 2;
        y3 = cy - r / 2;
    }

    // Draw the triangle
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);

    glEnd();
}

int main()
{
    GLFWwindow *window;

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Circle Example", NULL, NULL);
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

        // Set up drawing color
        glColor3f(1.0f, 1.0f, 1.0f); // white color

        // Call drawLinesPerpendicularToCircle function to draw lines perpendicular to the circle
        drawLinesPerpendicularToCircle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 300 + border_width, MAX_SEGMENTS);

        // Call drawJoinedLines function to draw the joined lines
        drawOuterRedSection();

        // Call drawCircle function to draw and fill a circle
        drawCircle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 300 + border_width, 100, black[0], black[1], black[2]);
        drawCircle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 300, 100, skyBlue[0], skyBlue[1], skyBlue[2]);

        drawCircle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 250 + border_width, 100, black[0], black[1], black[2]);
        drawCircle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 250, 100, yellow[0], yellow[1], yellow[2]);

        drawEquilateralTriangle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 250 + border_width, black[0], black[1], black[2], true);
        drawEquilateralTriangle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 250, lightPink[0], lightPink[1], lightPink[2], true);

        drawEquilateralTriangle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 250 + border_width, black[0], black[1], black[2], false);
        drawEquilateralTriangle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 250, lightPink[0], lightPink[1], lightPink[2], false);

        drawEquilateralTriangle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 250 + border_width, black[0], black[1], black[2], false);
        drawEquilateralTriangle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 250, lightPink[0], lightPink[1], lightPink[2], false);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
// void drawJoinedLines()
// {
//     glLineWidth(10.0f); // Set the width of the lines to 10 units
//     glBegin(GL_LINES);
//     glColor3f(0.0f, 0.0f, 0.0f); // Set the color of the lines to black

//     // Draw lines between consecutive points in the lineEndpoints array
//     for (int i = 0; i < MAX_SEGMENTS; i++)
//     {
//         float x1 = lineEndpoints[i][0];
//         float y1 = lineEndpoints[i][1];

//         float x2 = lineEndpoints[(i + 1) % MAX_SEGMENTS][0];
//         float y2 = lineEndpoints[(i + 1) % MAX_SEGMENTS][1];

//         // Calculate the midpoints between x1 and x2, and y1 and y2
//         float midX1 = x1 + (x2 - x1) * 0.25f;
//         float midY1 = y1 + (y2 - y1) * 0.25f;
//         float midX2 = x1 + (x2 - x1) * 0.5f;
//         float midY2 = y1 + (y2 - y1) * 0.5f;
//         float midX3 = x1 + (x2 - x1) * 0.75f;
//         float midY3 = y1 + (y2 - y1) * 0.75f;

//         // Calculate the control points for the curves
//         float controlX1 = midX1 + (y2 - y1) * 0.1f;  // Offset control point in the direction of the outer curve
//         float controlY1 = midY1 - (x2 - x1) * 0.1f;  // Offset control point in the direction of the outer curve
//         float controlX2 = midX2 + (y2 - y1) * 0.15f; // Offset control point in the direction of the outer curve
//         float controlY2 = midY2 - (x2 - x1) * 0.15f; // Offset control point in the direction of the outer curve
//         float controlX3 = midX3 + (y2 - y1) * 0.1f;  // Offset control point in the direction of the outer curve
//         float controlY3 = midY3 - (x2 - x1) * 0.1f;  // Offset control point in the direction of the outer curve

//         // Draw quadratic Bézier curves for each part
//         glVertex2f(x1, y1);
//         glVertex2f(controlX1, controlY1);
//         glVertex2f(controlX1, controlY1);
//         glVertex2f(controlX2, controlY2);
//         glVertex2f(controlX2, controlY2);
//         glVertex2f(controlX3, controlY3);
//         glVertex2f(controlX3, controlY3);
//         glVertex2f(x2, y2);
//     }

//     glEnd();
// }