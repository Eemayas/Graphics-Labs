#include <GLFW/glfw3.h>
#include <iostream>

// Define the bit codes for different regions
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

const int X_MAX = WINDOW_WIDTH / 2 + 100;
const int X_MIN = WINDOW_WIDTH / 2 - 100;
const int Y_MAX = WINDOW_HEIGHT / 2 + 100;
const int Y_MIN = WINDOW_HEIGHT / 2 - 100;

bool isAccepted = false;
std::pair<float, float> clippedLineCoordinate[2];

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
void cohenSutherlandLineClipAndDraw(float x1, float y1, float x2, float y2)
{
    // Compute region codes for P1, P2
    int regionCode1 = computeRegionCode(x1, y1);
    int regionCode2 = computeRegionCode(x2, y2);

    // Initialize line as outside the clip window
    bool accept = false;

    while (true)
    {
        if (!(regionCode1 | regionCode2))
        { // Both endpoints lie within the clip window
            accept = true;
            break;
        }
        else if (regionCode1 & regionCode2)
        { // Both endpoints are outside the clip window, in same region
            break;
        }
        else
        {
            // Line needs clipping
            // At least one of the endpoints is outside the clip window, pick it
            int codeOut = regionCode1 ? regionCode1 : regionCode2;

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
            }

            // Print the coordinates of the clipped segment
            std::cout << "(" << x1 << ", " << y1 << ") -> (" << x << ", " << y << ")" << std::endl;

            // Move outside point to intersection point to clip
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
    }

    if (accept)
    {
        clippedLineCoordinate[0].first = x1;
        clippedLineCoordinate[0].second = y1;
        clippedLineCoordinate[1].first = x2;
        clippedLineCoordinate[1].second = y2;
        isAccepted = true;
        std::cout << "The line is accepted and coordinate is: {" << x1 << "," << y1 << "} , {" << x2 << "," << y2 << "}" << std::endl;
    }
    else
    {
        std::cout << "The line is not accepted " << std::endl;
    }
}

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
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Cohen-Sutherland Line Clipping", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Set the key callback
    glfwSetKeyCallback(window, keyCallback);

    std::pair<float, float> lineCoordinate[2];

    // Ask the user for line coordinates
    std::cout << "Enter the coordinates of the line endpoints (x1, y1) and (x2, y2):" << std::endl;
    std::cout << "x1: ";
    std::cin >> lineCoordinate[0].first;
    std::cout << "y1: ";
    std::cin >> lineCoordinate[0].second;
    std::cout << "x2: ";
    std::cin >> lineCoordinate[1].first;
    std::cout << "y2: ";
    std::cin >> lineCoordinate[1].second;

    cohenSutherlandLineClipAndDraw(lineCoordinate[0].first, lineCoordinate[0].second, lineCoordinate[1].first, lineCoordinate[1].second);

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

        // Draw the rectangular clipping window
        glLineWidth(5);
        glBegin(GL_LINE_LOOP);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2i(X_MIN, Y_MIN);
        glVertex2i(X_MAX, Y_MIN);
        glVertex2i(X_MAX, Y_MAX);
        glVertex2i(X_MIN, Y_MAX);
        glEnd();

        // Draw the line
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex2d(lineCoordinate[0].first, lineCoordinate[0].second);
        glVertex2d(lineCoordinate[1].first, lineCoordinate[1].second);
        glEnd();

        if (isAccepted)
        {
            // std::cout << "The line is accepted and coordinate is: {" << clippedLineCoordinate[0].first << "," << clippedLineCoordinate[0].second << "} , {" << clippedLineCoordinate[1].first << "," << clippedLineCoordinate[1].second << "}" << std::endl;

            // Draw the line to be clipped
            glColor3f(0.0, 0.0, 0.0);
            // Draw the line
            glBegin(GL_LINES);
            glVertex2f(clippedLineCoordinate[0].first, clippedLineCoordinate[0].second);
            glVertex2f(clippedLineCoordinate[1].first, clippedLineCoordinate[1].second);
            glEnd();
        }

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
