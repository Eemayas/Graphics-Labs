#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <cmath>
#include <string>
#include <array>

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

float skyBlue[] = {169.0f / 255.0f, 198.0f / 255.0f, 228.0f / 255.0f, 1.0f}; // RGB(169, 198, 228) with full opacity
// Define an array to hold RGB values for the color (244, 232, 112)
float yellow[] = {244.0f / 255.0f, 232.0f / 255.0f, 112.0f / 255.0f, 1.0f}; // RGB(244, 232, 112) with full opacity
// Define an array to hold RGB values for the color (0, 0, 0)
float black[] = {0.0f, 0.0f, 0.0f, 1.0f};                                      // RGB(0, 0, 0) with full opacity
float white[] = {1.0f, 1.0f, 1.0f, 1.0f};                                      // RGB(1, 1, 1) with full opacity
float red[] = {1.0f, 0.0f, 0.0f, 1.0f};                                        // RGB(1, 0, 0) with full opacity
float lightPink[] = {253.0f / 255.0f, 173.0f / 255.0f, 172.0f / 255.0f, 1.0f}; // RGB(253, 173, 172) with full opacity
float transparent[] = {1.0f, 1.0f, 1.0f, 0.0f};                                // Transparent white

void drawCircle(float cx, float cy, float r, int num_segments, float *color)
{
    // Draw the filled circle
    glBegin(GL_TRIANGLE_FAN);
    glColor4fv(color);
    glVertex2f(cx, cy);
    for (int i = 0; i <= num_segments; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = r * cosf(theta) + cx;
        float y = r * sinf(theta) + cy;
        glVertex2f(x, y);
    }
    glEnd();
    // drawCircle(cx, cy, r + 10, num_segments, 0.0f, 0.0f, 0.0f);
}

void drawCirclePartly(float cx, float cy, float r, int start_angle, int end_angle, int num_segments, float *color)
{
    // Draw the circle border
    glLineWidth(10.0f); // Set the width of the border

    glBegin(GL_LINES);
    glColor4fv(color);

    // Convert angles to radians
    float start_rad = start_angle * 3.1415926f / 180.0f;
    float end_rad = end_angle * 3.1415926f / 180.0f;

    // Ensure that start_angle is less than end_angle for anticlockwise drawing
    if (start_angle >= end_angle)
    {
        // Add a full circle to the end angle
        end_angle += 360;
        end_rad = end_angle * 3.1415926f / 180.0f;
    }

    // Calculate the step size based on the number of segments
    float step = (end_rad - start_rad) / num_segments;

    // Draw line segments between consecutive points
    for (int i = 0; i < num_segments; i++)
    {
        float theta1 = start_rad + i * step;
        float theta2 = start_rad + (i + 1) * step;

        float x1 = r * cosf(theta1) + cx;
        float y1 = r * sinf(theta1) + cy;
        float x2 = r * cosf(theta2) + cx;
        float y2 = r * sinf(theta2) + cy;

        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    }

    glEnd();
}

std::array<float, 4> drawCirclePartlyPoints(float cx, float cy, float r, int start_angle, int end_angle, int num_segments, float *color)
{
    // Array to hold the coordinates of the starting and ending points
    std::array<float, 4> points;
    // Convert angles to radians
    float start_rad = start_angle * 3.1415926f / 180.0f;
    float end_rad = end_angle * 3.1415926f / 180.0f;

    // Ensure that start_angle is less than end_angle for anticlockwise drawing
    if (start_angle >= end_angle)
    {
        // Add a full circle to the end angle
        end_angle += 360;
        end_rad = end_angle * 3.1415926f / 180.0f;
    }

    // Calculate the step size based on the number of segments
    float step = (end_rad - start_rad) / num_segments;

    // Draw the circle border
    glLineWidth(7.0f); // Set the width of the border
    glBegin(GL_LINES);
    glColor4fv(black);

    // Draw line segments between consecutive points
    for (int i = 1; i < num_segments; i++)
    {
        float theta1 = start_rad + i * step;
        float theta2 = start_rad + (i + 1) * step;

        float x1 = r * cosf(theta1) + cx;
        float y1 = r * sinf(theta1) + cy;
        float x2 = r * cosf(theta2) + cx;
        float y2 = r * sinf(theta2) + cy;

        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    }

    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor4fv(color);
    r -= 3.0f;

    // Draw line segments between consecutive points
    for (int i = 0; i < num_segments; i++)
    {
        float theta1 = start_rad + i * step;
        float theta2 = start_rad + (i + 1) * step;

        float x1 = r * cosf(theta1) + cx;
        float y1 = r * sinf(theta1) + cy;
        float x2 = r * cosf(theta2) + cx;
        float y2 = r * sinf(theta2) + cy;

        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    }

    glEnd();

    // Calculate the coordinates of the starting point
    points[0] = r * cosf(start_rad) + cx;
    points[1] = r * sinf(start_rad) + cy;

    // Calculate the coordinates of the ending point
    points[2] = r * cosf(end_rad) + cx;
    points[3] = r * sinf(end_rad) + cy;

    // Return the array of coordinates
    return points;
}

void drawLinesPerpendicularToCircle(float cx, float cy, float r, int num_segments)
{
    glLineWidth(10.0f); // Set the width of the lines to 10 units

    glBegin(GL_LINES);
    glColor4fv(black); // Set the color of the lines to black

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
    glColor4fv(red); // Set the color to light pink

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
    glColor4fv(black); // Set the color of the lines to black

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

void drawEquilateralTriangle(float cx, float cy, float r, float *color, bool inverted)
{
    glBegin(GL_TRIANGLES);

    // Set the color for the triangle
    glColor4fv(color);

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

void drawCircleAtPoint(float cx, float cy, float radius)
{
    int num_segments = 100;
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= num_segments; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = radius * cosf(theta) + cx;
        float y = radius * sinf(theta) + cy;
        glVertex2f(x, y);
    }
    glEnd();
}

void drawDoubleBorderTriangle2(float cx, float cy, float r_outer, float r_inner, float *color, bool inverted)
{

    glColor4fv(black);

    float x1_outer, y1_outer, x2_outer, y2_outer, x3_outer, y3_outer;

    x1_outer = cx;
    y1_outer = cy - r_outer;

    x2_outer = cx - r_outer * sqrt(3) / 2;
    y2_outer = cy + r_outer / 2;

    x3_outer = cx + r_outer * sqrt(3) / 2;
    y3_outer = cy + r_outer / 2;

    // Set the color for the inner triangle (border)
    glColor4fv(color);
    float smallCircleRadius = 20.0f;
    drawCircle(x1_outer, y1_outer, smallCircleRadius, 100, black);
    // Draw small circles at the vertices of the outer triangle
    // drawCircleAtPoint(x1_outer, y1_outer, smallCircleRadius);
    // drawCircleAtPoint(x2_outer, y2_outer, smallCircleRadius);
    // drawCircleAtPoint(x3_outer, y3_outer, smallCircleRadius);
}

void drawDoubleBorderTriangle1(float cx, float cy, float r_outer, float r_inner, float *color, bool inverted)
{
    glBegin(GL_TRIANGLES);

    // Set the color for the outer triangle
    glColor4fv(black);

    // Calculate the coordinates of the three vertices of the outer equilateral triangle
    float x1_outer, y1_outer, x2_outer, y2_outer, x3_outer, y3_outer;

    if (!inverted)
    {
        x1_outer = cx;
        y1_outer = cy - r_outer;

        x2_outer = cx - r_outer * sqrt(3) / 2;
        y2_outer = cy + r_outer / 2;

        x3_outer = cx + r_outer * sqrt(3) / 2;
        y3_outer = cy + r_outer / 2;
    }
    else
    {
        x1_outer = cx;
        y1_outer = cy + r_outer;

        x2_outer = cx - r_outer * sqrt(3) / 2;
        y2_outer = cy - r_outer / 2;

        x3_outer = cx + r_outer * sqrt(3) / 2;
        y3_outer = cy - r_outer / 2;
    }

    // Draw the outer triangle
    glVertex2f(x1_outer, y1_outer);
    glVertex2f(x2_outer, y2_outer);
    glVertex2f(x3_outer, y3_outer);

    // Set the color for the inner triangle (border)
    glColor4fv(color);

    // Calculate the coordinates of the three vertices of the inner equilateral triangle
    float x1_inner, y1_inner, x2_inner, y2_inner, x3_inner, y3_inner;

    if (!inverted)
    {
        x1_inner = cx;
        y1_inner = cy - r_inner;

        x2_inner = cx - r_inner * sqrt(3) / 2;
        y2_inner = cy + r_inner / 2;

        x3_inner = cx + r_inner * sqrt(3) / 2;
        y3_inner = cy + r_inner / 2;
    }
    else
    {
        x1_inner = cx;
        y1_inner = cy + r_inner;

        x2_inner = cx - r_inner * sqrt(3) / 2;
        y2_inner = cy - r_inner / 2;

        x3_inner = cx + r_inner * sqrt(3) / 2;
        y3_inner = cy - r_inner / 2;
    }

    // Draw the inner triangle
    glVertex2f(x1_inner, y1_inner);
    glVertex2f(x2_inner, y2_inner);
    glVertex2f(x3_inner, y3_inner);

    glEnd();
}

void drawDoubleBorderTriangle(float cx, float cy, float r_outer, float r_inner, bool inverted)
{
    glBegin(GL_TRIANGLES);

    // Calculate the coordinates of the three vertices of the outer equilateral triangle
    float x1_outer, y1_outer, x2_outer, y2_outer, x3_outer, y3_outer;

    if (!inverted)
    {
        x1_outer = cx;
        y1_outer = cy - r_outer;

        x2_outer = cx - r_outer * sqrt(3) / 2;
        y2_outer = cy + r_outer / 2;

        x3_outer = cx + r_outer * sqrt(3) / 2;
        y3_outer = cy + r_outer / 2;
    }
    else
    {
        x1_outer = cx;
        y1_outer = cy + r_outer;

        x2_outer = cx - r_outer * sqrt(3) / 2;
        y2_outer = cy - r_outer / 2;

        x3_outer = cx + r_outer * sqrt(3) / 2;
        y3_outer = cy - r_outer / 2;
    }

    // Draw the outer triangle
    glVertex2f(x1_outer, y1_outer);
    glVertex2f(x2_outer, y2_outer);
    glVertex2f(x3_outer, y3_outer);

    // Calculate the coordinates of the three vertices of the inner equilateral triangle
    float x1_inner, y1_inner, x2_inner, y2_inner, x3_inner, y3_inner;

    if (!inverted)
    {
        x1_inner = cx;
        y1_inner = cy - r_inner;

        x2_inner = cx - r_inner * sqrt(3) / 2;
        y2_inner = cy + r_inner / 2;

        x3_inner = cx + r_inner * sqrt(3) / 2;
        y3_inner = cy + r_inner / 2;
    }
    else
    {
        x1_inner = cx;
        y1_inner = cy + r_inner;

        x2_inner = cx - r_inner * sqrt(3) / 2;
        y2_inner = cy - r_inner / 2;

        x3_inner = cx + r_inner * sqrt(3) / 2;
        y3_inner = cy - r_inner / 2;
    }

    // Draw the inner triangle
    glVertex2f(x1_inner, y1_inner);
    glVertex2f(x2_inner, y2_inner);
    glVertex2f(x3_inner, y3_inner);

    glEnd();
}

void drawKUShape(float x, float y, float width, float height)
{

    glLineWidth(10.0f);
    glColor4fv(black);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);
    glVertex2f(x - width / 2 - 50, y - height); // Bottom-left
    glVertex2f(x + width / 2 - 50, y - height); // Bottom-right
    glVertex2f(x + width / 2 - 50, y + height); // Top-right
    glVertex2f(x - width / 2 - 50, y + height); // Top-left
    glEnd();

    GLfloat spaceBetweenTwoArm = 1.8 * width;
    glLineWidth(10.0f);
    glColor4fv(black);
    glPushMatrix();
    glTranslatef(x + 15, y - 40, 0);
    glRotatef(45, 0, 0, 1);
    glTranslatef(-width / 2, -height / 2, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(width, 0);
    glVertex2f(width, height);
    glVertex2f(0, height + width);
    // // glVertex2f(0, 0);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(width, height);
    glVertex2f(0, height + width);
    glVertex2f(width + spaceBetweenTwoArm + width, height + width);
    glVertex2f(spaceBetweenTwoArm + width, height);

    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(width + spaceBetweenTwoArm + width, height + width);
    glVertex2f(spaceBetweenTwoArm + width, height);
    glVertex2f(width + spaceBetweenTwoArm, 0);
    glVertex2f(width + spaceBetweenTwoArm + width, 0);

    glEnd();
    glPopMatrix();
}

// Function to calculate vertices and draw circles around them
std::array<std::array<float, 2>, 6> triangleCircle(float cx, float cy, float r)
{
    // Define a 2D array to store the coordinates of the vertices
    float vertices[3][2];

    // Calculate the coordinates of the vertices
    // Vertex 1
    vertices[0][0] = cx;     // x-coordinate of vertex 1
    vertices[0][1] = cy - r; // y-coordinate of vertex 1

    // Vertex 2
    vertices[1][0] = cx - r * sqrt(3) / 2; // x-coordinate of vertex 2
    vertices[1][1] = cy + r / 2;           // y-coordinate of vertex 2

    // Vertex 3
    vertices[2][0] = cx + r * sqrt(3) / 2; // x-coordinate of vertex 3
    vertices[2][1] = cy + r / 2;           // y-coordinate of vertex 3

    // Call the function
    std::array<std::array<float, 2>, 6> pointsArray; // 2D array to store starting and ending points

    // Create an array to hold the points for each circle
    std::array<std::array<float, 4>, 3> tempPointsArray;

    // Call drawCirclePartlyPoints for each circle and store the points
    float smallRadius = 30.0;
    tempPointsArray[0] = drawCirclePartlyPoints(vertices[0][0], vertices[0][1], smallRadius, 120, 60, 100, white);
    tempPointsArray[1] = drawCirclePartlyPoints(vertices[1][0], vertices[1][1], smallRadius, 0, 300, 100, white);
    tempPointsArray[2] = drawCirclePartlyPoints(vertices[2][0], vertices[2][1], smallRadius, 240, 180, 100, white);

    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < 2; ++i)
        {
            std::array<float, 2> tempArray;
            tempArray[0] = tempPointsArray[j][i * 2];
            tempArray[1] = tempPointsArray[j][i * 2 + 1];

            pointsArray[(j * 2) + i] = tempArray;
        }
    }
    // Draw lines connecting consecutive points
    glBegin(GL_LINES);
    glColor4fv(black);

    glVertex2f(pointsArray[0][0], pointsArray[0][1]);
    glVertex2f(pointsArray[3][0], pointsArray[3][1]);

    glVertex2f(pointsArray[2][0], pointsArray[2][1]);
    glVertex2f(pointsArray[5][0], pointsArray[5][1]);

    glVertex2f(pointsArray[4][0], pointsArray[4][1]);
    glVertex2f(pointsArray[1][0], pointsArray[1][1]);

    glVertex2f(vertices[1][0] + 30, vertices[1][1] - 20);
    glVertex2f(vertices[0][0], vertices[0][1] + 30);

    glVertex2f(vertices[2][0] - 25, vertices[2][1] - 20);
    glVertex2f(vertices[1][0] + 30, vertices[1][1] - 20);

    glVertex2f(vertices[0][0], vertices[0][1] + 30);
    glVertex2f(vertices[2][0] - 30, vertices[2][1] - 20);

    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(pointsArray[0][0], pointsArray[0][1]);
    glVertex2f(pointsArray[3][0], pointsArray[3][1]);
    glVertex2f(vertices[1][0] + 30, vertices[1][1] - 20);
    glVertex2f(vertices[0][0], vertices[0][1] + 30);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(pointsArray[2][0], pointsArray[2][1]);
    glVertex2f(pointsArray[5][0], pointsArray[5][1]);
    glVertex2f(vertices[2][0] - 25, vertices[2][1] - 20);
    glVertex2f(vertices[1][0] + 30, vertices[1][1] - 20);

    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(pointsArray[4][0], pointsArray[4][1]);
    glVertex2f(pointsArray[1][0], pointsArray[1][1]);
    glVertex2f(vertices[0][0], vertices[0][1] + 30);
    glVertex2f(vertices[2][0] - 30, vertices[2][1] - 20);
    glEnd();

    return pointsArray;
}

void drawCurvedLine(float x1, float y1, float x2, float y2, bool inverse = false)
{
    // Define the width of the lines
    glLineWidth(10.0f);

    // Set the color of the lines
    glColor4fv(black);

    // Calculate the midpoints between x1 and x2, and y1 and y2
    float midX1 = inverse ? x1 - (x2 - x1) * 0.25f : x1 + (x2 - x1) * 0.25f;
    float midY1 = y1 + (y2 - y1) * 0.25f;
    float midX2 = inverse ? x1 - (x2 - x1) * 0.5f : x1 + (x2 - x1) * 0.5f;
    float midY2 = y1 + (y2 - y1) * 0.5f;
    float midX3 = inverse ? x1 - (x2 - x1) * 0.75f : x1 + (x2 - x1) * 0.75f;
    float midY3 = y1 + (y2 - y1) * 0.75f;

    // Calculate the control points for the curves
    float controlX1 = midX1 + (y2 - y1) * 0.1f;                                      // Offset control point in the direction of the outer curve
    float controlY1 = midY1 - (x2 - x1) * 0.1f;                                      // Offset control point in the direction of the outer curve
    float controlX2 = midX2 + (y2 - y1) * 0.15f;                                     // Offset control point in the direction of the outer curve
    float controlY2 = midY2 - (x2 - x1) * 0.15f;                                     // Offset control point in the direction of the outer curve
    float controlX3 = inverse ? midX3 - (y1 - y2) * 0.1f : midX3 + (y2 - y1) * 0.1f; // Offset control point in the direction of the outer curve
    float controlY3 = midY3 - (x2 - x1) * 0.1f;                                      // Offset control point in the direction of the outer curve

    // Draw quadratic Bézier curves for each part
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(controlX1, controlY1);
    glVertex2f(controlX1, controlY1);
    glVertex2f(controlX2, controlY2);
    glVertex2f(controlX2, controlY2);
    glVertex2f(controlX3, controlY3);
    glVertex2f(controlX3, controlY3);
    glVertex2f(x2, y2);
    glEnd();
}

// Define the angle of rotation
float angle = 90.0f;
void drawLine(float x1, float y1, float x2, float y2)
{
    glColor4fv(black);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}
void drawParallelLines(float x, float y)
{
    // Define the length of the lines
    float length = 60.0f;
    float distance = 20.0f;
    float increment = 20.0f;
    glLoadIdentity();

    // Translate to the center of the window
    // glTranslatef(0.5f, 0.5f, 0.0f);

    // Rotate the line around its midpoint
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    // Draw the line
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    // drawLine(-0.5f, 0.0f, 0.5f, 0.0f); // Line from (-0.5, 0) to (0.5, 0)

    glLineWidth(10.f);
    glColor4fv(black);

    // Draw the second line
    glBegin(GL_LINES);
    glVertex2f(x - length / 2, y);
    glVertex2f(x + length / 2, y);
    // glEnd();

    // // Draw the first line parallel to the second and 10.0f above
    // glBegin(GL_LINES);
    glVertex2f(x - length / 2 + increment / 2, y - distance); // Starting point of the second line
    glVertex2f(x + length / 2 - increment / 2, y - distance); // Ending point of the second line
    glEnd();

    // // Draw the third line parallel to the first line and 10.0f below
    // glBegin(GL_LINES);
    // glVertex2f(x - length / 2, y + distance); // Starting point of the third line
    // glVertex2f(x + length / 2, y + distance); // Ending point of the third line
    // glEnd();

    // drawCurvedLine(x - length / 2 + increment / 2, y, x - length / 2 + increment / 2, y - distance);
    // drawCurvedLine(x + length / 2 - increment / 2, y - distance, x + length / 2 - increment / 2, y, true);

    // drawCurvedLine(x - length / 2, y + distance, x - length / 2, y);
    // drawCurvedLine(x + length / 2, y, x + length / 2, y + distance, true);

    // glLineWidth(10.0f); // Set the width of the lines to 10 units
    // glColor4fv(black);

    // glBegin(GL_TRIANGLES);
    // // Specify the vertices of the triangle
    // glVertex2f(x - length / 4, y - distance);
    // glVertex2f(x + length / 4, y - distance);
    // glVertex2f(x, y - distance * 2);
    // glEnd();
    glFlush();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Translate to the center of the window
    glTranslatef(0.5f, 0.5f, 0.0f);

    // Rotate the line around its midpoint
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    // Draw the line
    glColor3f(1.0f, 1.0f, 1.0f);       // White color
    drawLine(-0.5f, 0.0f, 0.5f, 0.0f); // Line from (-0.5, 0) to (0.5, 0)

    glFlush();
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
        glColor4fv(white); // white color

        // // Call drawLinesPerpendicularToCircle function to draw lines perpendicular to the circle
        // drawLinesPerpendicularToCircle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 300 + border_width, MAX_SEGMENTS);

        // // Call drawJoinedLines function to draw the joined lines
        // drawOuterRedSection();

        // // Call drawCircle function to draw and fill a circle
        // drawCircle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 300 + border_width, 100, black);
        // drawCircle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 300, 100, skyBlue);

        // drawCircle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 250 + border_width, 100, black);
        // drawCircle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 250, 100, yellow);

        // drawDoubleBorderTriangle1(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 250, 240, lightPink, true);

        // drawKUShape(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 30,
        //             70);

        // triangleCircle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 250.f);

        display();
        drawParallelLines(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
