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

// Point structure to represent 2D points
struct Point
{
    double x, y;

    // Constructor to initialize Point with specific values
    Point(double x, double y) : x(x), y(y) {}
};

// Function to check if a point is inside the clipping area
bool inside(Point p, Point clip_min, Point clip_max)
{
    return (p.x >= clip_min.x && p.x <= clip_max.x &&
            p.y >= clip_min.y && p.y <= clip_max.y);
}

// Function to find the intersection point of two lines
Point intersect(Point p1, Point p2, Point clip_min, Point clip_max)
{
    Point intersection(0, 0); // Initialize intersection point
    double m = (p2.y - p1.y) / (p2.x - p1.x);

    if (p1.x != p2.x)
    {
        // Non-horizontal line
        if (p1.x < clip_min.x)
        {
            intersection.x = clip_min.x;
            intersection.y = p1.y + m * (clip_min.x - p1.x);
        }
        else if (p1.x > clip_max.x)
        {
            intersection.x = clip_max.x;
            intersection.y = p1.y + m * (clip_max.x - p1.x);
        }
    }
    if (p1.y != p2.y)
    {
        // Horizontal line
        if (p1.y < clip_min.y)
        {
            intersection.y = clip_min.y;
            intersection.x = p1.x + m * (clip_min.y - p1.y);
        }
        else if (p1.y > clip_max.y)
        {
            intersection.y = clip_max.y;
            intersection.x = p1.x + m * (clip_max.y - p1.y);
        }
    }

    return intersection;
}

// Function to calculate Euclidean distance between two points
double euclideanDistance(Point p1, Point p2)
{
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}

// Function to find the nearest boundary corner to a point
Point findNearestBoundaryCorner(Point point, Point clip_min, Point clip_max)
{
    double minDistance = std::numeric_limits<double>::max(); // Initialize with maximum possible value
    Point nearestCorner(0, 0);

    // Calculate distances to each corner
    double distanceTopLeft = euclideanDistance(point, clip_min);
    double distanceTopRight = euclideanDistance(point, Point(clip_max.x, clip_min.y));
    double distanceBottomLeft = euclideanDistance(point, Point(clip_min.x, clip_max.y));
    double distanceBottomRight = euclideanDistance(point, clip_max);

    // Find the minimum distance among all corners
    if (distanceTopLeft < minDistance)
    {
        minDistance = distanceTopLeft;
        nearestCorner = clip_min;
    }
    if (distanceTopRight < minDistance)
    {
        minDistance = distanceTopRight;
        nearestCorner = Point(clip_max.x, clip_min.y);
    }
    if (distanceBottomLeft < minDistance)
    {
        minDistance = distanceBottomLeft;
        nearestCorner = Point(clip_min.x, clip_max.y);
    }
    if (distanceBottomRight < minDistance)
    {
        minDistance = distanceBottomRight;
        nearestCorner = clip_max;
    }

    return nearestCorner;
}

// Function to find the nearest point in a vector of points given an input point
Point findNearestPoint(const vector<Point> &points, Point input)
{
    double minDistance = numeric_limits<double>::max(); // Initialize with maximum possible value
    Point nearestPoint(0, 0);

    for (const auto &point : points)
    {
        double distance = euclideanDistance(input, point);
        if (distance < minDistance)
        {
            minDistance = distance;
            nearestPoint = point;
        }
    }

    return nearestPoint;
}

// Function to perform Sutherland-Hodgman polygon clipping
vector<Point> sutherlandHodgman(vector<Point> polygon, Point clip_min, Point clip_max)
{
    vector<Point> clippedPolygon;

    for (int i = 0; i < polygon.size(); i++)
    {
        int next = (i + 1) % polygon.size();

        // Check if both points are outside the clip window
        if (!inside(polygon[i], clip_min, clip_max) && !inside(polygon[next], clip_min, clip_max))
        {
            // Check if the line intersects with any boundary line
            vector<Point> intersection;
            bool hasIntersection = false;

            // Check intersection with left boundary
            if (polygon[i].x < clip_min.x && polygon[next].x >= clip_min.x ||
                polygon[next].x < clip_min.x && polygon[i].x >= clip_min.x)
            {
                intersection.push_back(intersect(polygon[i], polygon[next], Point(clip_min.x, std::numeric_limits<double>::lowest()), Point(clip_min.x, std::numeric_limits<double>::max())));
                hasIntersection = true;
            }
            // Check intersection with right boundary
            if (polygon[i].x > clip_max.x && polygon[next].x <= clip_max.x ||
                polygon[next].x > clip_max.x && polygon[i].x <= clip_max.x)
            {
                intersection.push_back(intersect(polygon[i], polygon[next], Point(clip_max.x, std::numeric_limits<double>::lowest()), Point(clip_max.x, std::numeric_limits<double>::max())));
                hasIntersection = true;
            }
            // Check intersection with bottom boundary
            if (polygon[i].y < clip_min.y && polygon[next].y >= clip_min.y ||
                polygon[next].y < clip_min.y && polygon[i].y >= clip_min.y)
            {
                intersection.push_back(intersect(polygon[i], polygon[next], Point(std::numeric_limits<double>::lowest(), clip_min.y), Point(std::numeric_limits<double>::max(), clip_min.y)));
                hasIntersection = true;
            }
            // Check intersection with top boundary
            if (polygon[i].y > clip_max.y && polygon[next].y <= clip_max.y ||
                polygon[next].y > clip_max.y && polygon[i].y <= clip_max.y)
            {
                intersection.push_back(intersect(polygon[i], polygon[next], Point(std::numeric_limits<double>::lowest(), clip_max.y), Point(std::numeric_limits<double>::max(), clip_max.y)));
                hasIntersection = true;
            }

            // If there is an intersection, add the intersection point if it is near the first point
            if (hasIntersection)
            {
                clippedPolygon.push_back(findNearestPoint(intersection, polygon[i]));
            }

            // Continue to the next edge
            continue;
        }

        // If both points are inside, keep the second point
        if (inside(polygon[next], clip_min, clip_max))
        {
            if (!inside(polygon[i], clip_min, clip_max))
                clippedPolygon.push_back(intersect(polygon[i], polygon[next], clip_min, clip_max));
            clippedPolygon.push_back(polygon[next]);
        }
        // If only the second point is inside, find intersection and keep it
        else if (inside(polygon[i], clip_min, clip_max))
        {
            clippedPolygon.push_back(intersect(polygon[i], polygon[next], clip_min, clip_max));
        }
        // If neither points are inside, ignore
    }

    return clippedPolygon;
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

    // Define the clipping window (rectangle)
    Point clip_min = {X_MIN, Y_MIN};
    Point clip_max = {X_MAX, Y_MAX};

    // Perform polygon clipping
    vector<Point> clippedPolygon = sutherlandHodgman(polygon, clip_min, clip_max);

    // Print the clipped polygon vertices
    cout << "Clipped Polygon Vertices:" << endl;
    for (const auto &point : clippedPolygon)
    {
        cout << "(" << point.x << ", " << point.y << ")" << endl;
    }

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
        glColor3f(1.0, 0.0, 0.0);
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
