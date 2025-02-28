#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Define colors for drawing
float black[] = {0.0f, 0.0f, 0.0f, 1.0f};
float red[] = {1.0f, 0.0f, 0.0f, 1.0f};

// Define window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

// Structure to hold a point in 3D space
struct Point
{
    float x, y, z;
};

// Define the vertices of a rectangle
Point rectangle[4] = {
    {0.1, 0.1, 1},
    {0.5, 0.1, 1},
    {0.5, 0.5, 1},
    {0.1, 0.5, 1}};

// Function to set a 3x3 matrix to the identity matrix
void setIdentityMatrix(float matrix[3][3])
{
    matrix[0][0] = 1;
    matrix[0][1] = 0;
    matrix[0][2] = 0;
    matrix[1][0] = 0;
    matrix[1][1] = 1;
    matrix[1][2] = 0;
    matrix[2][0] = 0;
    matrix[2][1] = 0;
    matrix[2][2] = 1;
}

// Function to set a 3x3 rotation matrix
void rotate(float angle, float matrix[3][3])
{
    setIdentityMatrix(matrix);
    float rad = angle * M_PI / 180.0; // Convert angle to radians
    matrix[0][0] = cos(rad);
    matrix[0][1] = -sin(rad);
    matrix[1][0] = sin(rad);
    matrix[1][1] = cos(rad);
}

// Function to set a 3x3 scaling matrix
void scale(float sx, float sy, float matrix[3][3])
{
    setIdentityMatrix(matrix);
    matrix[0][0] = sx;
    matrix[1][1] = sy;
}

// Function to set a 3x3 reflection matrix
void reflect(bool reflectX, bool reflectY, float matrix[3][3])
{
    setIdentityMatrix(matrix);
    if (reflectX)
        matrix[0][0] = -1;
    if (reflectY)
        matrix[1][1] = -1;
}

// Function to set a 3x3 shear matrix
void shear(float shx, float shy, float matrix[3][3])
{
    setIdentityMatrix(matrix);
    matrix[0][1] = shx;
    matrix[1][0] = shy;
}

// Function to set a 3x3 translation matrix
void translate(float tx, float ty, float matrix[3][3])
{
    setIdentityMatrix(matrix);
    matrix[0][2] = tx;
    matrix[1][2] = ty;
}

// Function to multiply a 3x3 matrix by a 3x1 vector
void multiplyMatricesPoints(float result[3][1], float matrixA[3][3], float matrixB[3][1])
{
    for (int i = 0; i < 3; i++)
    {
        result[i][0] = 0;
        for (int j = 0; j < 3; j++)
        {
            result[i][0] += matrixA[i][j] * matrixB[j][0];
        }
    }
}

// Function to multiply two 3x3 matrices
void multiplyMatrices(float a[3][3], float b[3][3], float result[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            result[i][j] = 0.0f;
            for (int k = 0; k < 3; k++)
            {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

// Function to apply a transformation matrix to a set of points
void applyTransformation(float matrix[3][3], Point *points, int numPoints)
{
    for (int i = 0; i < numPoints; i++)
    {
        float vector[3][1] = {{points[i].x}, {points[i].y}, {points[i].z}};
        float result[3][1];
        multiplyMatricesPoints(result, matrix, vector);
        points[i].x = result[0][0];
        points[i].y = result[1][0];
        points[i].z = result[2][0];
        // std::cout << "(" << points[i].x << ", " << points[i].y << ", " << points[i].z << ")\n";
    }
}

// Function to display a shape with a specified color
void display(Point *shape, float *color)
{
    glColor4fv(color);   // Set the current color
    glBegin(GL_POLYGON); // Start drawing a polygon
    for (int i = 0; i < 4; i++)
    {
        glVertex2f(shape[i].x, shape[i].y); // Specify each vertex
    }
    glEnd();   // End drawing the polygon
    glFlush(); // Ensure all OpenGL commands are executed
}

// Function to draw coordinate axes
void drawAxes()
{
    glColor3f(0.0, 0.0, 0.0); // Set color to black
    glBegin(GL_LINES);        // Start drawing lines
    glVertex2f(-1.0, 0.0);    // X-axis
    glVertex2f(1.0, 0.0);
    glVertex2f(0.0, -1.0); // Y-axis
    glVertex2f(0.0, 1.0);
    glEnd();   // End drawing lines
    glFlush(); // Ensure all OpenGL commands are executed
}

// GLFW key callback function to handle ESC key
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE); // Close the window if ESC is pressed
    }
}

// Function to copy a matrix
void copyMatrix(float source[3][3], float destination[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            destination[i][j] = source[i][j];
        }
    }
}

int main()
{
    int choice;
    float tx, ty, angle, sx, sy, shx, shy;
    bool continueLoop = true;
    bool reflectX;

    // Copy original rectangle vertices to a new array for transformations
    Point transformedRectangle[4];
    for (int i = 0; i < 4; i++)
    {
        transformedRectangle[i] = rectangle[i];
    }

    // User input for choosing a transformation
    std::cout << "Choose a transformation to apply to the rectangle:\n"
              << "1. Translate\n"
              << "2. Rotate\n"
              << "3. Scale\n"
              << "4. Reflect\n"
              << "5. Shear\n"
              << "6. Composite Transformation\n"
              << "Enter your choice: ";
    std::cin >> choice;

    // Get specific transformation parameters based on the user's choice
    switch (choice)
    {
    case 1:
        std::cout << "Enter translation values in range(0-1) (tx ty): ";
        std::cin >> tx >> ty;
        break;
    case 2:
        std::cout << "Enter rotation angle: ";
        std::cin >> angle;
        break;
    case 3:
        std::cout << "Enter scaling factors (sx sy): ";
        std::cin >> sx >> sy;
        break;
    case 4:
        std::cout << "Reflect across ......? (0 for x-axis, 1 for y-axis): ";
        std::cin >> reflectX;
        break;
    case 5:
        std::cout << "Enter shear values (shx shy): ";
        std::cin >> shx >> shy;
        break;
    case 6:
        std::cout << "Composite Transformation ---------------\n ";
        break;
    default:
        std::cout << "Invalid choice" << std::endl;
        return 0;
    }

    // Initialize transformation matrices
    float matrix[3][3];
    setIdentityMatrix(matrix);

    float tempMatrix[3][3];
    setIdentityMatrix(tempMatrix);

    float tempMatrix2[3][3];
    setIdentityMatrix(tempMatrix2);

    // Apply the selected transformation
    switch (choice)
    {
    case 1:
        // Apply translation transformation
        translate(tx, ty, matrix);                            // Set translation matrix with tx, ty values
        applyTransformation(matrix, transformedRectangle, 4); // Apply translation to the rectangle vertices
        break;
    case 2:
        // Apply rotation transformation
        rotate(angle, matrix);                                // Set rotation matrix with the given angle
        applyTransformation(matrix, transformedRectangle, 4); // Apply rotation to the rectangle vertices
        break;
    case 3:
        // Apply scaling transformation
        scale(sx, sy, matrix);                                // Set scaling matrix with sx, sy values
        applyTransformation(matrix, transformedRectangle, 4); // Apply scaling to the rectangle vertices
        break;
    case 4:
        // Apply reflection transformation
        reflect(reflectX, !reflectX, matrix);                 // Set reflection matrix based on reflectX
        applyTransformation(matrix, transformedRectangle, 4); // Apply reflection to the rectangle vertices
        break;
    case 5:
        // Apply shear transformation
        shear(shx, shy, matrix);                              // Set shear matrix with shx, shy values
        applyTransformation(matrix, transformedRectangle, 4); // Apply shear to the rectangle vertices
        break;
    case 6:
        // Apply composite transformation
        // Copy the current transformation matrix to tempMatrix for further composite transformations
        copyMatrix(matrix, tempMatrix);

        // Loop to apply multiple transformations
        while (continueLoop)
        {
            // User input for choosing a transformation
            std::cout << "Choose a transformation to apply to the rectangle:\n"
                      << "1. Translate\n"
                      << "2. Rotate\n"
                      << "3. Scale\n"
                      << "4. Reflect\n"
                      << "5. Shear\n"
                      << "6. Apply and Exit\n"
                      << "Enter your choice: ";
            std::cin >> choice;

            switch (choice)
            {
            case 1:
                // Apply translation transformation
                std::cout << "Enter translation values (tx ty): ";
                std::cin >> tx >> ty;
                translate(tx, ty, matrix);                         // Set translation matrix
                copyMatrix(tempMatrix, tempMatrix2);               // Copy current composite matrix
                multiplyMatrices(matrix, tempMatrix2, tempMatrix); // Update composite matrix with translation
                break;
            case 2:
                // Apply rotation transformation
                std::cout << "Enter rotation angle: ";
                std::cin >> angle;
                rotate(angle, matrix);                             // Set rotation matrix
                copyMatrix(tempMatrix, tempMatrix2);               // Copy current composite matrix
                multiplyMatrices(matrix, tempMatrix2, tempMatrix); // Update composite matrix with rotation
                break;
            case 3:
                // Apply scaling transformation
                std::cout << "Enter scaling factors (sx sy): ";
                std::cin >> sx >> sy;
                scale(sx, sy, matrix);                             // Set scaling matrix
                copyMatrix(tempMatrix, tempMatrix2);               // Copy current composite matrix
                multiplyMatrices(matrix, tempMatrix2, tempMatrix); // Update composite matrix with scaling
                break;
            case 4:
                // Apply reflection transformation
                std::cout << "Reflect across ......? (0 for x-axis, 1 for y-axis): ";
                std::cin >> reflectX;
                reflect(reflectX, !reflectX, matrix);              // Set reflection matrix
                copyMatrix(tempMatrix, tempMatrix2);               // Copy current composite matrix
                multiplyMatrices(matrix, tempMatrix2, tempMatrix); // Update composite matrix with reflection
                break;
            case 5:
                // Apply shear transformation
                std::cout << "Enter shear values (shx shy): ";
                std::cin >> shx >> shy;
                shear(shx, shy, matrix);                           // Set shear matrix
                copyMatrix(tempMatrix, tempMatrix2);               // Copy current composite matrix
                multiplyMatrices(matrix, tempMatrix2, tempMatrix); // Update composite matrix with shear
                break;
            case 6:
                // Exit loop to apply composite transformation
                continueLoop = false;
                break;
            default:
                std::cout << "Invalid choice" << std::endl;
                return 0;
            }
        }

        // Apply the final composite transformation
        applyTransformation(tempMatrix, transformedRectangle, 4);
        break;
    default:
        // Handle invalid transformation choice
        std::cout << "Invalid choice" << std::endl;
    }

    // Initialize GLFW and create a window
    GLFWwindow *window;
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Transformation", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    // Main loop to render the rectangle and apply transformations
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set the background color to white
        glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // Set the viewport to cover the window
        glMatrixMode(GL_PROJECTION);                   // Switch to projection matrix
        glLoadIdentity();                              // Reset the projection matrix
        glOrtho(-1.0, 1.0, -1.0, 1.0, 0, 1);           // Set orthographic projection

        drawAxes(); // Draw coordinate axes

        display(rectangle, black);          // Draw the original rectangle in black
        display(transformedRectangle, red); // Draw the transformed rectangle in red

        glfwSwapBuffers(window); // Swap the front and back buffers
        glfwPollEvents();        // Poll for and process events
    }

    glfwTerminate(); // Terminate GLFW
    return 0;
}
