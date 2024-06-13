#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>
#include <vector>

// Screen dimensions
constexpr unsigned int SCREEN_WIDTH = 800;
constexpr unsigned int SCREEN_HEIGHT = 800;

// Colors in RGB format
constexpr float BACKGROUND_COLOR[4] = {1.0, 1.0, 1.0, 1.0};
constexpr float COLOR_YELLOW[3] = {244 / 255.0, 232 / 255.0, 112 / 255.0};
constexpr float blackColor[3] = {0.0, 0.0, 0.0};
constexpr float COLOR_WHITE[3] = {1.0, 1.0, 1.0};
constexpr float COLOR_RED[3] = {1.0, 0.0, 0.0};
constexpr float COLOR_GREEN[3] = {0.0, 1.0, 0.0};
constexpr float COLOR_BLUE[3] = {0.0, 0.0, 1.0};

// Structure to hold 3D point coordinates
typedef struct Point3D
{
    double x, y, z;
} Point3D;

// Callback function for framebuffer resizing
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// Function to set perspective projection
void setPerspectiveProjection(int width, int height);

// Function to render a 3D cube
void render3DCube(const std::vector<Point3D> &vertices, const GLfloat color[3], GLfloat lineColor[3]);

// Overloaded operator to print glm::mat4 matrices
std::ostream &operator<<(std::ostream &os, const glm::mat4 &matrix)
{
    const float *data = glm::value_ptr(matrix);
    for (int i = 0; i < 4; ++i)
    {
        os << "| ";
        for (int j = 0; j < 4; ++j)
        {
            os << data[j * 4 + i] << " ";
        }
        os << "|\n";
    }
    return os;
}

// Function to perform matrix multiplication
static bool matrixMultiplication(GLfloat *matrixA, GLfloat *matrixB, size_t rowsA, size_t colsA, size_t rowsB, size_t colsB, GLfloat *result)
{
    if (colsA != rowsB)
    {
        return false;
    }

    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < colsB; j++)
        {
            GLfloat sum = 0;
            for (int k = 0; k < rowsB; k++)
            {
                sum += matrixA[i * colsA + k] * matrixB[k * colsB + j];
            }
            result[i * colsB + j] = sum;
        }
    }

    return true;
}

// Function to perform parallel projection
bool performParallelProjection(double alpha, double phi, GLfloat points[3], GLfloat result[3])
{
    double L1 = 1 / tan(glm::radians(alpha));
    double l1CosPhi = L1 * cos(glm::radians(phi));
    double l1SinPhi = L1 * sin(glm::radians(phi));

    // Transformation matrix for parallel projection
    GLfloat matrixA[16] = {
        1, 0, l1CosPhi, 0,
        0, 1, l1SinPhi, 0,
        0, 0, 0, 0,
        0, 0, 0, 1};

    GLfloat matrixB[4] = {points[0], points[1], points[2], 1};
    GLfloat output[4];

    bool multiplySuccess = matrixMultiplication(matrixA, matrixB, 4, 4, 4, 1, output);

    result[0] = output[0];
    result[1] = output[1];
    result[2] = output[2];

    return multiplySuccess;
}

int main(int argc, char **argv)
{
    // GLFW initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    // Creating GLFW window
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Lab", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD initialization
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set perspective projection
    setPerspectiveProjection(SCREEN_WIDTH, SCREEN_HEIGHT);

    glm::mat4 transformationMatrix = glm::mat4(1.0f);

    // Rendering loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // View matrix setup
        glm::mat4 viewMatrix = glm::lookAt(glm::vec3(3.0f, 2.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.0f, 0.0f));

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(glm::value_ptr(viewMatrix * modelMatrix));

        // Define cube vertices in 3D
        std::vector<Point3D> cubeVertices = {
            {-0.25, -1.25, 0.25},
            {-1.25, -0.25, 0.25},
            {-1.25, -0.25, 0.25},
            {-0.25, -0.25, 0.25},
            {-0.25, -1.25, 1.25},
            {-1.25, -1.25, 1.25},
            {-1.25, -0.25, 1.25},
            {-0.25, -0.25, 1.25},
        };

        // Render the 3D cube
        render3DCube(cubeVertices, COLOR_RED, (GLfloat *)blackColor);

        transformationMatrix = glm::mat4(1.0f);

        std::vector<Point3D> projectedVertices;
        double alpha = 90;
        double phi = 0;

        // Perform parallel projection for each vertex of the cube
        for (int i = 0; i < cubeVertices.size(); i++)
        {
            GLfloat vertex[3] = {static_cast<GLfloat>(cubeVertices[i].x), static_cast<GLfloat>(cubeVertices[i].y), static_cast<GLfloat>(cubeVertices[i].z)};
            GLfloat outVertex[3];
            performParallelProjection(alpha, phi, vertex, outVertex);

            projectedVertices.push_back({outVertex[0], outVertex[1], outVertex[2]});
        }

        // Render the projected vertices
        glColor3fv(blackColor);
        glPointSize(20);
        glBegin(GL_QUADS);
        for (Point3D point : projectedVertices)
        {
            GLfloat vertex[3] = {static_cast<GLfloat>(point.x), static_cast<GLfloat>(point.y), static_cast<GLfloat>(point.z)};
            glVertex3fv(vertex);
        }
        glEnd();

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
    return 0;
}

// Function definition for rendering a 3D cube
void render3DCube(const std::vector<Point3D> &vertices, const GLfloat color[3], GLfloat lineColor[3])
{
    // Render each face of the cube
    glColor3fv(color);

    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++)
    {
        glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
    }
    glEnd();

    glBegin(GL_QUADS);
    for (int i = 4; i < 8; i++)
    {
        glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
    }
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
    glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
    glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
    glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
    glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
    glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
    glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
    glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
    glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
    glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
    glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
    glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
    glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
    glEnd();

    // Render edges of the cube
    glColor3fv(lineColor);
    glBegin(GL_LINE_STRIP);
    for (const Point3D &p : vertices)
    {
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
    glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
    glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
    glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
    glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
    glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
    glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
    glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
    glEnd();
}

// Callback function called when the framebuffer size changes
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // Adjust the viewport when the window is resized
    glViewport(0, 0, width, height);
}

// Function to set up perspective projection
void setPerspectiveProjection(int width, int height)
{
    // Set the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    glLoadMatrixf(glm::value_ptr(projectionMatrix));

    // Switch back to modelview matrix mode
    glMatrixMode(GL_MODELVIEW);
}
