#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Define colors as arrays of floats
float black[] = {0.0f, 0.0f, 0.0f, 1.0f};
float red[] = {1.0f, 0.0f, 0.0f, 1.0f};
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

// Define a structure to represent a point in 3D space
struct Point
{
    float x, y, z;
};

// Define the vertices of a rectangle
Point rectangle[4] = {
    // {0.2, 0.3, 1},
    // {0.6, 0.3, 1},
    // {0.6, 0.7, 1},
    // {0.2, 0.7, 1}};
    {0.1, 0.1, 1},
    {0.5, 0.1, 1},
    {0.5, 0.5, 1},
    {0.1, 0.5, 1}};

// (0.2, 0.3)
// (0.6, 0.3)
// (0.6, 0.7)
// (0.2, 0.7)
// Function to generate the identity matrix
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

// Function to create a rotation matrix
void rotate(float angle, float matrix[3][3])
{
    setIdentityMatrix(matrix);
    float rad = angle * M_PI / 180.0; // Convert angle to radians
    matrix[0][0] = cos(rad);
    matrix[0][1] = -sin(rad);
    matrix[1][0] = sin(rad);
    matrix[1][1] = cos(rad);
}

// Function to create a scaling matrix
void scale(float sx, float sy, float matrix[3][3])
{
    setIdentityMatrix(matrix);
    matrix[0][0] = sx;
    matrix[1][1] = sy;
}

// Function to create a reflection matrix
void reflect(bool reflectX, bool reflectY, float matrix[3][3])
{
    setIdentityMatrix(matrix);
    if (reflectX)
        matrix[0][0] = -1; // Reflect across the x-axis
    if (reflectY)
        matrix[1][1] = -1; // Reflect across the y-axis
}

// Function to create a shearing matrix
void shear(float shx, float shy, float matrix[3][3])
{
    setIdentityMatrix(matrix);
    matrix[0][1] = shx; // Shear along the x-axis
    matrix[1][0] = shy; // Shear along the y-axis
}

// Function to create a translation matrix
void translate(float tx, float ty, float matrix[3][3])
{
    setIdentityMatrix(matrix);
    matrix[0][2] = tx;
    matrix[1][2] = ty;
}

// Function to multiply two matrices
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

// Define a function to multiply two 3x3 matrices
void multiplyMatricesPoints(float a[3][3], float b[3][3], float result[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            result[i][j] = 0;
            for (int k = 0; k < 3; k++)
            {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

// Define a function to multiply two 3x3 matrices
void multiplyMatrices(float a[3][3], float b[3][3], float result[3][3])
{
    // // Initialize the result matrix to zero
    // for (int i = 0; i < 3; i++)
    // {
    //     for (int j = 0; j < 3; j++)
    //     {
    //         result[i][j] = 0.0f;
    //     }
    // }

    // Perform the matrix multiplication
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            result[i][j] = 0;
            for (int k = 0; k < 3; k++)
            {
                result[i][j] += a[i][k] * b[k][j];
            }
            std::cout << result[i][j] << std::endl;
        }
    }
}

// Function to apply a transformation matrix to an array of points
void applyTransformation(float matrix[3][3], Point *points, int numPoints)
{
    for (int i = 0; i < numPoints; i++)
    {
        float vector[3][1] = {{points[i].x}, {points[i].y}, {points[i].z}};
        float result[3][1];

        // Multiply the transformation matrix with the point vector
        multiplyMatricesPoints(result, matrix, vector);

        // Update the point coordinates with the result
        points[i].x = result[0][0];
        points[i].y = result[1][0];
        points[i].z = result[2][0];
    }
}

// Function to display a shape on the screen
void display(Point *shape, float *color)
{
    glColor4fv(color);
    glBegin(GL_POLYGON);
    std::cout << "--------------------------------------------------------------------------------\n";

    // Loop through the points and draw them
    for (int i = 0; i < 4; i++)
    {
        std::cout << "(" << shape[i].x << ", " << shape[i].y << ")\n";
        glVertex2f(shape[i].x, shape[i].y);
    }

    glEnd();
    glFlush();
}

// Function to draw the x and y axes
void drawAxes()
{
    glColor3f(0.0, 0.0, 0.0); // Set color to black
    glBegin(GL_LINES);

    // Draw the x-axis
    glVertex2f(-1.0, 0.0);
    glVertex2f(1.0, 0.0);

    // Draw the y-axis
    glVertex2f(0.0, -1.0);
    glVertex2f(0.0, 1.0);

    glEnd();
    glFlush();
}

// Callback function to handle key events
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// Function to copy a 3x3 matrix
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

    // Copy the original rectangle vertices to a new array for transformation
    Point transformedRectangle[4];
    for (int i = 0; i < 4; i++)
    {
        transformedRectangle[i] = rectangle[i];
    }

    // Ask the user to choose a transformation
    std::cout << "Choose a transformation to apply to the rectangle:\n"
              << "1. Translate\n"
              << "2. Rotate\n"
              << "3. Scale\n"
              << "4. Reflect\n"
              << "5. Shear\n"
              << "6. Composite Transformation\n"
              << "Enter your choice: ";
    std::cin >> choice;

    // Get the appropriate parameters based on the user's choice
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
        break;
    }

    // Create an identity matrix
    float matrix[3][3];
    setIdentityMatrix(matrix);

    // Create an identity matrix
    float tempMatrix[3][3];
    setIdentityMatrix(tempMatrix);

    // Create an identity matrix
    float tempMatrix2[3][3];
    setIdentityMatrix(tempMatrix2);

    switch (choice)
    {
    case 1:
        translate(tx, ty, matrix);
        applyTransformation(matrix, transformedRectangle, 4);
        break;
    case 2:
        rotate(angle, matrix);
        applyTransformation(matrix, transformedRectangle, 4);
        break;
    case 3:
        scale(sx, sy, matrix);
        applyTransformation(matrix, transformedRectangle, 4);
        break;
    case 4:
        reflect(reflectX, !reflectX, matrix);
        applyTransformation(matrix, transformedRectangle, 4);
        break;
    case 5:
        shear(shx, shy, matrix);
        applyTransformation(matrix, transformedRectangle, 4);
        break;
    case 6:
        while (continueLoop)
        {
            // Ask the user to choose a transformation
            std::cout << "Choose a transformation to apply to the rectangle:\n"
                      << "1. Translate\n"
                      << "2. Rotate\n"
                      << "3. Scale\n"
                      << "4. Reflect\n"
                      << "5. Shear\n"
                      << "6. Exit\n"
                      << "Enter your choice: ";
            std::cin >> choice;

            switch (choice)
            {
            case 1:
                std::cout << "Enter translation values in range(0-1) (tx ty): ";
                std::cin >> tx >> ty;
                translate(tx, ty, matrix);
                copyMatrix(tempMatrix, tempMatrix2);
                multiplyMatrices(tempMatrix2, matrix, tempMatrix);
                break;
            case 2:
                std::cout << "Enter rotation angle: ";
                std::cin >> angle;
                rotate(angle, matrix);
                copyMatrix(tempMatrix, tempMatrix2);
                multiplyMatrices(tempMatrix2, matrix, tempMatrix);
                // applyTransformation(matrix, transformedRectangle, 4);
                break;
            case 3:
                std::cout << "Enter scaling factors (sx sy): ";
                std::cin >> sx >> sy;
                scale(sx, sy, matrix);
                copyMatrix(tempMatrix, tempMatrix2);
                multiplyMatrices(tempMatrix2, matrix, tempMatrix);
                // applyTransformation(matrix, transformedRectangle, 4);
                break;
            case 4:
                std::cout << "Reflect across ......? (0 for x-axis, 1 for y-axis): ";
                std::cin >> reflectX;
                reflect(reflectX, !reflectX, matrix);
                copyMatrix(tempMatrix, tempMatrix2);
                multiplyMatrices(tempMatrix2, matrix, tempMatrix);
                // applyTransformation(matrix, transformedRectangle, 4);
                break;
            case 5:
                std::cout << "Enter shear values (shx shy): ";
                std::cin >> shx >> shy;
                shear(shx, shy, matrix);
                copyMatrix(tempMatrix, tempMatrix2);
                multiplyMatrices(tempMatrix2, matrix, tempMatrix);
                // applyTransformation(matrix, transformedRectangle, 4);
                break;
            case 6:
                continueLoop = false;
                break;
            default:
                std::cout << "Invalid choice" << std::endl;
                return 0;
                break;
            }
        }

        applyTransformation(tempMatrix, transformedRectangle, 4);
        break;
    default:
        std::cout << "Invalid choice" << std::endl;
        break;
    }

    GLFWwindow *window;

    // Initialize GLFW library
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Transformation", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Set the key callback function
    glfwSetKeyCallback(window, keyCallback);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Clear the window with a white background
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Set the viewport and projection matrix
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, 0, 1); // Set orthographic projection for a normalized coordinate system

        // // Create an identity matrix
        // float matrix[3][3];
        // setIdentityMatrix(matrix);

        // // Create an identity matrix
        // float tempMatrix[3][3];
        // setIdentityMatrix(tempMatrix);

        // // Create an identity matrix
        // float tempMatrix2[3][3];
        // setIdentityMatrix(tempMatrix2);

        // Draw the x and y axes
        drawAxes();

        // Display the original rectangle in black
        display(rectangle, black);

        // // Apply the chosen transformation to the rectangle
        // switch (choice)
        // {
        // case 1:
        //     translate(tx, ty, matrix);
        //     applyTransformation(matrix, transformedRectangle, 4);
        //     break;
        // case 2:
        //     rotate(angle, matrix);
        //     applyTransformation(matrix, transformedRectangle, 4);
        //     break;
        // case 3:
        //     scale(sx, sy, matrix);
        //     applyTransformation(matrix, transformedRectangle, 4);
        //     break;
        // case 4:
        //     reflect(reflectX, !reflectX, matrix);
        //     applyTransformation(matrix, transformedRectangle, 4);
        //     break;
        // case 5:
        //     shear(shx, shy, matrix);
        //     applyTransformation(matrix, transformedRectangle, 4);
        //     break;
        // case 6:
        //     while (continueLoop)
        //     {
        //         // Ask the user to choose a transformation
        //         std::cout << "Choose a transformation to apply to the rectangle:\n"
        //                   << "1. Translate\n"
        //                   << "2. Rotate\n"
        //                   << "3. Scale\n"
        //                   << "4. Reflect\n"
        //                   << "5. Shear\n"
        //                   << "6. Exit\n"
        //                   << "Enter your choice: ";
        //         std::cin >> choice;

        //         switch (choice)
        //         {
        //         case 1:
        //             std::cout << "Enter translation values in range(0-1) (tx ty): ";
        //             std::cin >> tx >> ty;
        //             translate(tx, ty, matrix);
        //             copyMatrix(tempMatrix, tempMatrix2);
        //             multiplyMatrices(tempMatrix2, matrix, tempMatrix);
        //             break;
        //         case 2:
        //             std::cout << "Enter rotation angle: ";
        //             std::cin >> angle;
        //             rotate(angle, matrix);
        //             copyMatrix(tempMatrix, tempMatrix2);
        //             multiplyMatrices(tempMatrix2, matrix, tempMatrix);
        //             // applyTransformation(matrix, transformedRectangle, 4);
        //             break;
        //         case 3:
        //             std::cout << "Enter scaling factors (sx sy): ";
        //             std::cin >> sx >> sy;
        //             scale(sx, sy, matrix);
        //             multiplyMatrices(tempMatrix, matrix, tempMatrix);
        //             // applyTransformation(matrix, transformedRectangle, 4);
        //             break;
        //         case 4:
        //             std::cout << "Reflect across ......? (0 for x-axis, 1 for y-axis): ";
        //             std::cin >> reflectX;
        //             reflect(reflectX, !reflectX, matrix);
        //             multiplyMatrices(tempMatrix, matrix, tempMatrix);
        //             // applyTransformation(matrix, transformedRectangle, 4);
        //             break;
        //         case 5:
        //             std::cout << "Enter shear values (shx shy): ";
        //             std::cin >> shx >> shy;
        //             shear(shx, shy, matrix);
        //             multiplyMatrices(tempMatrix, matrix, tempMatrix);
        //             // applyTransformation(matrix, transformedRectangle, 4);
        //             break;
        //         case 6:
        //             continueLoop = false;
        //             break;
        //         default:
        //             std::cout << "Invalid choice" << std::endl;
        //             return 0;
        //             break;
        //         }
        //     }

        //     applyTransformation(tempMatrix, transformedRectangle, 4);
        //     break;
        // default:
        //     std::cout << "Invalid choice" << std::endl;
        //     break;
        // }

        // Display the transformed rectangle in red
        display(transformedRectangle, red);

        // Swap the front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up and exit
    glfwTerminate();
    return 0;
}
