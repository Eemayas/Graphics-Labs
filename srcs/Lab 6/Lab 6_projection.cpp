#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>
#include <vector>

constexpr unsigned int SCREEN_WIDTH = 800;
constexpr unsigned int SCREEN_HEIGHT = 800;
constexpr float blackColor[3] = {0.0, 0.0, 0.0};
constexpr float redColor[3] = {1.0, 0.0, 0.0};

typedef struct Point3D
{
    double x, y, z;
} Point3D;

// Function prototypes
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void setPerspectiveProjection(int width, int height);
void render3DCube(const std::vector<Point3D> &vertices, const GLfloat color[3], GLfloat line_color[3]);
bool computeParallelProjection(double alpha, double phi, GLfloat points[3], GLfloat ret_out[3]);

// Overloaded operator for printing glm::mat4
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

// Matrix multiplication function
static bool matrix_multiplication(GLfloat *A, GLfloat *B, size_t row_A, int column_A, int row_B, int column_B, GLfloat *result_matrix)
{
    if (column_A != row_B)
    {
        return false; // Dimensions mismatch
    }

    // Matrix multiplication
    for (int i = 0; i < row_A; i++)
    {
        for (int j = 0; j < column_B; j++)
        {
            GLfloat sum = 0;
            for (int k = 0; k < row_B; k++)
            {
                sum += A[i * column_A + k] * B[k * column_B + j];
            }
            result_matrix[i * column_B + j] = sum;
        }
    }

    return true;
}

// Function to compute parallel projection
bool computeParallelProjection(double alpha, double phi, GLfloat points[3], GLfloat ret_out[3])
{
    double L1 = 1 / tan(glm::radians(alpha));
    double l1_cos_phi = L1 * cos(glm::radians(phi));
    double l1_sin_phi = L1 * sin(glm::radians(phi));

    GLfloat matrix_a[16] = {
        1, 0, static_cast<GLfloat>(l1_cos_phi), 0,
        0, 1, static_cast<GLfloat>(l1_sin_phi), 0,
        0, 0, 0, 0,
        0, 0, 0, 1};

    GLfloat matrix_b[4] = {points[0], points[1], points[2], 1};

    GLfloat out[4];

    bool multiply_success = matrix_multiplication(matrix_a, matrix_b, 4, 4, 4, 1, out);

    ret_out[0] = out[0];
    ret_out[1] = out[1];
    ret_out[2] = out[2];

    return multiply_success;
}

int main(int argc, char **argv)
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    // Create GLFW window
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Projection", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Enable depth testing for 3D rendering
    glEnable(GL_DEPTH_TEST);

    // Set initial perspective projection
    setPerspectiveProjection(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Main rendering loop
    glm::mat4 transform = glm::mat4(1.0f);
    while (!glfwWindowShouldClose(window))
    {
        // Clear the color and depth buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up view matrix
        glm::mat4 view = glm::lookAt(glm::vec3(3.0f, 2.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.0f, 0.0f));

        // Set model matrix
        glm::mat4 model = glm::mat4(1.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(glm::value_ptr(view * model));

        // Define cube vertices
        std::vector<Point3D> cube_vertices = {
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
        render3DCube(cube_vertices, redColor, (GLfloat *)blackColor);

        // Reset transformation matrix
        transform = glm::mat4(1.0f);

        // Compute parallel projection
        std::vector<Point3D> projected_vertices;
        double alpha = 90;
        double phi = 0;
        for (int i = 0; i < cube_vertices.size(); i++)
        {
            GLfloat vertex[3] = {static_cast<GLfloat>(cube_vertices[i].x), static_cast<GLfloat>(cube_vertices[i].y), static_cast<GLfloat>(cube_vertices[i].z)};
            GLfloat out_vertex[3];
            computeParallelProjection(alpha, phi, vertex, out_vertex);
            projected_vertices.push_back({out_vertex[0], out_vertex[1], out_vertex[2]});
        }

        // Render projected vertices
        glColor3fv(blackColor);
        glPointSize(20);
        glBegin(GL_QUADS);
        for (Point3D point : projected_vertices)
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

// Function called when framebuffer size changes
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // Set viewport size
    glViewport(0, 0, width, height);
}

// Function to set perspective projection
void setPerspectiveProjection(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    glLoadMatrixf(glm::value_ptr(projection));
    glMatrixMode(GL_MODELVIEW);
}

// Function to render a 3D cube
void render3DCube(const std::vector<Point3D> &vertices, const GLfloat color[3], GLfloat line_color[3])
{
    // Render cube faces
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

    // Render cube faces continued...
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

    // Render cube edges
    glColor3fv(line_color);
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


