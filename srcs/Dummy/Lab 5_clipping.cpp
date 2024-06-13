#include <GLFW/glfw3.h>
#include <vector>

// Define a Point structure
struct Point
{
    float x, y;
};

// Function to handle window resizing
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Function to process input
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Sutherland-Hodgman clipping algorithm
std::vector<Point> clipPolygon(const std::vector<Point> &polygon, const Point &min, const Point &max)
{
    auto clipEdge = [&](const std::vector<Point> &input, const Point &edgeStart, const Point &edgeEnd)
    {
        std::vector<Point> output;
        for (size_t i = 0; i < input.size(); ++i)
        {
            Point current = input[i];
            Point previous = input[(i + input.size() - 1) % input.size()];
            auto isInside = [&](const Point &p)
            {
                return (edgeEnd.x - edgeStart.x) * (p.y - edgeStart.y) - (edgeEnd.y - edgeStart.y) * (p.x - edgeStart.x) >= 0;
            };
            if (isInside(current))
            {
                if (!isInside(previous))
                {
                    float t = ((edgeStart.y - previous.y) * (current.x - previous.x) - (edgeStart.x - previous.x) * (current.y - previous.y)) /
                              ((current.x - previous.x) * (edgeEnd.y - edgeStart.y) - (current.y - previous.y) * (edgeEnd.x - edgeStart.x));
                    output.push_back({previous.x + t * (current.x - previous.x), previous.y + t * (current.y - previous.y)});
                }
                output.push_back(current);
            }
            else if (isInside(previous))
            {
                float t = ((edgeStart.y - previous.y) * (current.x - previous.x) - (edgeStart.x - previous.x) * (current.y - previous.y)) /
                          ((current.x - previous.x) * (edgeEnd.y - edgeStart.y) - (current.y - previous.y) * (edgeEnd.x - edgeStart.x));
                output.push_back({previous.x + t * (current.x - previous.x), previous.y + t * (current.y - previous.y)});
            }
        }
        return output;
    };

    std::vector<Point> output = polygon;
    output = clipEdge(output, {min.x, min.y}, {max.x, min.y}); // bottom
    output = clipEdge(output, {max.x, min.y}, {max.x, max.y}); // right
    output = clipEdge(output, {max.x, max.y}, {min.x, max.y}); // top
    output = clipEdge(output, {min.x, max.y}, {min.x, min.y}); // left

    return output;
}

// Function to draw a polygon
void drawPolygon(const std::vector<Point> &polygon, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    for (const auto &point : polygon)
    {
        glVertex2f(point.x, point.y);
    }
    glEnd();
}

// Function to draw the clipping window
void drawClippingWindow(const Point &min, const Point &max)
{
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(min.x, min.y);
    glVertex2f(max.x, min.y);
    glVertex2f(max.x, max.y);
    glVertex2f(min.x, max.y);
    glEnd();
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        return -1;
    }

    // Set GLFW options
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(800, 600, "Sutherland-Hodgman Clipping", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Set callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Define the original polygon and the clipping window
    std::vector<Point> polygon = {{100, 150}, {200, 250}, {300, 200}, {350, 100}, {250, 50}};
    Point min = {150, 150};
    Point max = {300, 300};

    // Compute the clipped polygon
    std::vector<Point> clippedPolygon = clipPolygon(polygon, min, max);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);

        // Rendering commands here
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw original polygon in red
        drawPolygon(polygon, 1.0f, 0.0f, 0.0f);

        // Draw clipped polygon in green
        drawPolygon(clippedPolygon, 0.0f, 1.0f, 0.0f);

        // Draw clipping window border in black
        drawClippingWindow(min, max);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
