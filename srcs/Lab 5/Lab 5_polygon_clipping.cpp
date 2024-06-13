#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <limits>

using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

const int X_MAX = 300;
const int X_MIN = 150;
const int Y_MAX = 250;
const int Y_MIN = 150;

std::pair<float, float> clippedLineCoordinate[2];
bool isAccepted = false;
// Define the bit codes for different regions
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

// Point structure to represent 2D points
struct Point
{
    double x, y;

    // Constructor to initialize Point with specific values
    Point(double x, double y) : x(x), y(y) {}

    // Define the equality operator
    bool operator==(const Point &other) const
    {
        return (x == other.x && y == other.y);
    }
};

void push_if_not_duplicate(std::vector<Point> &vec, const Point &element)
{

    // Check if the element is already in the vector
    for (const auto &p : vec)
    {
        if (p == element)
        {
            // Element found, do nothing
            std::cout << "Element (" << element.x << ", " << element.y << ") is already in the vector.\n";
            return;
        }
    }
    // Element not found, so add it
    vec.push_back(element);
}

// Function to compute the region code for a point (x, y)
int computeRegionCode(float x, float y)
{
    int code = INSIDE;

    if (x < X_MIN) // to the left of clip window
        code |= LEFT;
    else if (x > X_MAX) // to the right of clip window
        code |= RIGHT;
    if (y < Y_MIN) // below the clip window
        code |= BOTTOM;
    else if (y > Y_MAX) // above the clip window
        code |= TOP;

    return code;
}
// Function to clip a line from (x1, y1) to (x2, y2)
void cohenSutherlandLineClipAndDraw(float x1, float y1, float x2, float y2, int direction)
{
    // Compute region codes for P1, P2
    int regionCode1 = computeRegionCode(x1, y1);
    int regionCode2 = computeRegionCode(x2, y2);

    // Initialize line as outside the clip window
    if (regionCode1 == direction || regionCode2 == direction)
    {
        int codeOut = regionCode1 == direction ? regionCode1 : regionCode2;
        float x, y;
        // Find intersection point
        if (codeOut & TOP)
        { // Point is above the clip window
            x = x1 + (x2 - x1) * (Y_MAX - y1) / (y2 - y1);
            y = Y_MAX;
        }
        else if (codeOut & BOTTOM)
        { // Point is below the clip window
            x = x1 + (x2 - x1) * (Y_MIN - y1) / (y2 - y1);
            y = Y_MIN;
        }
        else if (codeOut & RIGHT)
        { // Point is to the right of clip window
            y = y1 + (y2 - y1) * (X_MAX - x1) / (x2 - x1);
            x = X_MAX;
        }
        else if (codeOut & LEFT)
        { // Point is to the left of clip window
            y = y1 + (y2 - y1) * (X_MIN - x1) / (x2 - x1);
            x = X_MIN;
        } // Move outside point to intersection point to clip
        if (codeOut == regionCode1)
        {
            x1 = x;
            y1 = y;
            regionCode1 = computeRegionCode(x1, y1);
        }
        else
        {
            x2 = x;
            y2 = y;
            regionCode2 = computeRegionCode(x2, y2);
        }
    }

    clippedLineCoordinate[0].first = x1;
    clippedLineCoordinate[0].second = y1;
    clippedLineCoordinate[1].first = x2;
    clippedLineCoordinate[1].second = y2;
    //     std::cout << "The line is accepted and coordinate is: {" << x1 << "," << y1 << "} , {" << x2 << "," << y2 << "}" << std::endl;
}

bool checkCoordinateFallOnBoundary(const Point &element)
{
    if (
        (element.x <= X_MAX && element.x >= X_MIN) && (element.y <= Y_MAX && element.y >= Y_MIN))
    {
        return true;
    }
    else
    {
        std::cout << "Eror:Element (" << element.x << ", " << element.y << ")" << std::endl;
        return false;
    }
}

// Function to clip a polygon against a clipping window edge
vector<Point> clipPolygon(vector<Point> polygon)
{
    vector<Point> outputList;
    vector<Point> temp;

    for (int i = 0; i < polygon.size(); i++)
    {
        int polygonLength = polygon.size();
        int direction = LEFT;
        cohenSutherlandLineClipAndDraw(polygon[i % polygonLength].x, polygon[i % polygonLength].y, polygon[(i + 1) % polygonLength].x, polygon[(i + 1) % polygonLength].y, direction);

        int regionCode1 = computeRegionCode(clippedLineCoordinate[0].first, clippedLineCoordinate[0].second);
        int regionCode2 = computeRegionCode(clippedLineCoordinate[1].first, clippedLineCoordinate[1].second);
        if (regionCode1 != direction)
        {
            push_if_not_duplicate(outputList, Point(clippedLineCoordinate[0].first, clippedLineCoordinate[0].second));
        }
        if (regionCode2 != direction)
        {
            push_if_not_duplicate(outputList, Point(clippedLineCoordinate[1].first, clippedLineCoordinate[1].second));
        }
    }
    for (const auto &point : outputList)
    {
        std::cout << "{" << point.x - 100 << "," << point.y - 100 << "}" << std::endl;
    }
    polygon = outputList;
    outputList.clear();
    for (int i = 0; i < polygon.size(); i++)
    {
        int polygonLength = polygon.size();
        int direction = TOP;
        cohenSutherlandLineClipAndDraw(polygon[i % polygonLength].x, polygon[i % polygonLength].y, polygon[(i + 1) % polygonLength].x, polygon[(i + 1) % polygonLength].y, direction);

        int regionCode1 = computeRegionCode(clippedLineCoordinate[0].first, clippedLineCoordinate[0].second);
        int regionCode2 = computeRegionCode(clippedLineCoordinate[1].first, clippedLineCoordinate[1].second);
        if (regionCode1 != direction)
        {
            push_if_not_duplicate(outputList, Point(clippedLineCoordinate[0].first, clippedLineCoordinate[0].second));
        }
        if (regionCode2 != direction)
        {
            push_if_not_duplicate(outputList, Point(clippedLineCoordinate[1].first, clippedLineCoordinate[1].second));
        }
    }
    polygon = outputList;
    outputList.clear();
    for (int i = 0; i < polygon.size(); i++)
    {
        int polygonLength = polygon.size();
        int direction = RIGHT;
        cohenSutherlandLineClipAndDraw(polygon[i % polygonLength].x, polygon[i % polygonLength].y, polygon[(i + 1) % polygonLength].x, polygon[(i + 1) % polygonLength].y, direction);

        int regionCode1 = computeRegionCode(clippedLineCoordinate[0].first, clippedLineCoordinate[0].second);
        int regionCode2 = computeRegionCode(clippedLineCoordinate[1].first, clippedLineCoordinate[1].second);
        if (regionCode1 != direction)
        {
            push_if_not_duplicate(outputList, Point(clippedLineCoordinate[0].first, clippedLineCoordinate[0].second));
        }
        if (regionCode2 != direction)
        {
            push_if_not_duplicate(outputList, Point(clippedLineCoordinate[1].first, clippedLineCoordinate[1].second));
        }
    }
    polygon = outputList;
    outputList.clear();
    for (int i = 0; i < polygon.size(); i++)
    {
        int polygonLength = polygon.size();
        int direction = BOTTOM;
        cohenSutherlandLineClipAndDraw(polygon[i % polygonLength].x, polygon[i % polygonLength].y, polygon[(i + 1) % polygonLength].x, polygon[(i + 1) % polygonLength].y, direction);

        int regionCode1 = computeRegionCode(clippedLineCoordinate[0].first, clippedLineCoordinate[0].second);
        int regionCode2 = computeRegionCode(clippedLineCoordinate[1].first, clippedLineCoordinate[1].second);

        if (regionCode1 != direction)
        {
            push_if_not_duplicate(outputList, Point(clippedLineCoordinate[0].first, clippedLineCoordinate[0].second));
        }
        if (regionCode2 != direction)
        {
            push_if_not_duplicate(outputList, Point(clippedLineCoordinate[1].first, clippedLineCoordinate[1].second));
        }
    }
    temp = outputList;
    outputList.clear();

    for (const auto &point : temp)
    {
        if (checkCoordinateFallOnBoundary(Point(point.x, point.y)))
        {
            push_if_not_duplicate(outputList, Point(point.x, point.y));
            std::cout << "{" << point.x - 100 << "," << point.y - 100 << "}" << std::endl;
        }
    }
    return outputList;
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main()
{
    // Define the polygon vertices
    vector<Point> polygon = {
        {180, 300},
        {320, 220},
        {250, 200},
        {200, 130},
        {110, 220}};

    // Perform polygon clipping
    vector<Point> clippedPolygon = clipPolygon(polygon);
    GLFWwindow *window;

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Polygon Clipping", NULL, NULL);
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
        glOrtho(0, WINDOW_WIDTH / 2, 0, WINDOW_HEIGHT / 2, 0, 1);

        // Draw the rectangular clipping window
        glLineWidth(5);
        glBegin(GL_LINE_LOOP);
        glColor3f(0.0, 1.0, 0.0);
        glVertex2i(X_MIN, Y_MIN);
        glVertex2i(X_MAX, Y_MIN);
        glVertex2i(X_MAX, Y_MAX);
        glVertex2i(X_MIN, Y_MAX);
        glEnd();

        // Draw the original polygon
        glColor3f(1.0, 0.0, 0.0); // Set color to red
        glBegin(GL_LINE_LOOP);
        for (const auto &point : polygon)
        {
            glVertex2d(point.x, point.y);
        }
        glEnd();

        // Draw the clipped polygon
        glColor3f(0.0, 0.0, 0.0); // Set color to black
        glBegin(GL_LINE_LOOP);
        for (const auto &point : clippedPolygon)
        {
            glVertex2d(point.x, point.y);
        }
        glEnd();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
