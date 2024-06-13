#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <vector>

// Window dimensions
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 800;

// Color definitions
float blackColor[3] = {0.0, 0.0, 0.0};
float redColor[3] = {1.0, 0.0, 0.0};
float greenColor[3] = {0.0, 1.0, 0.0};
float blueColor[3] = {0.0, 0.0, 1.0};

// Structure for 3D points
typedef struct Point3D
{
    double x, y, z;
} Point3D;

// Function declarations
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void setPerspectiveProjection(int width, int height);
void render3DCube(const std::vector<Point3D> &vertices, const GLfloat color[3], GLfloat line_color[3]);

// Overloaded << operator for glm::mat4 printing
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

int main(int argc, char **argv)
{
    // GLFW initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    // Creating GLFW window
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3d Transformation", NULL, NULL);
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

    // Enabling depth testing
    glEnable(GL_DEPTH_TEST);

    // Setting perspective projection
    setPerspectiveProjection(SCREEN_WIDTH, SCREEN_HEIGHT);

    // User input for transformation type
    int transformationType;
    std::cout << "Enter the type of transformation (1: translation, 2: rotation, 3: scaling, 4: shearing): ";
    std::cin >> transformationType;

    // Initializing transformation matrix
    glm::mat4 transform = glm::mat4(1.0f);
    if (transformationType == 1)
    {
        // Translation
        float tx, ty, tz;
        std::cout << "Enter translation factors (tx, ty, tz): ";
        std::cin >> tx >> ty >> tz;
        transform = glm::translate(transform, glm::vec3(tx, ty, tz));
        std::cout << "Translation Matrix:\n"
                  << transform; // Print transformation matrix
    }
    else if (transformationType == 2)
    {
        // Rotation
        float angle, rx, ry, rz;
        std::cout << "Enter rotation angle (degrees) and axis (rx, ry, rz): ";
        std::cin >> angle >> rx >> ry >> rz;
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(rx, ry, rz));
        std::cout << "Rotation Matrix:\n"
                  << transform; // Print transformation matrix
    }
    else if (transformationType == 3)
    {
        // Scaling
        float sx, sy, sz;
        std::cout << "Enter scaling factors (sx, sy, sz): ";
        std::cin >> sx >> sy >> sz;
        transform = glm::scale(transform, glm::vec3(sx, sy, sz));
        std::cout << "Scaling Matrix:\n"
                  << transform; // Print transformation matrix
    }
    else if (transformationType == 4)
    {
        // Shearing
        float shx, shy;
        std::cout << "Enter shearing factors (shx, shy): ";
        std::cin >> shx >> shy;
        glm::mat4 shearMatrix = glm::mat4(1.0f);
        shearMatrix[2][0] = shx;
        shearMatrix[2][1] = shy;
        transform = shearMatrix;
        std::cout << "Shearing Matrix:\n"
                  << transform; // Print transformation matrix
    }
    else
    {
        std::cout << "Invalid transformation type!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Cube vertices definition
    std::vector<Point3D> cube_vertices = {
        {-0.25, -0.25, -0.25},
        {0.25, -0.25, -0.25},
        {0.25, 0.25, -0.25},
        {-0.25, 0.25, -0.25},
        {-0.25, -0.25, 0.25},
        {0.25, -0.25, 0.25},
        {0.25, 0.25, 0.25},
        {-0.25, 0.25, 0.25}};

    // Applying transformation to cube vertices
    std::vector<Point3D> transformed_vertices;
    transformed_vertices.reserve(cube_vertices.size());

    for (const auto &vertex : cube_vertices)
    {
        glm::vec4 temp(vertex.x, vertex.y, vertex.z, 1.0f);
        glm::vec4 transformed = transform * temp;
        transformed_vertices.push_back({transformed.x, transformed.y, transformed.z});
    }

    const std::vector<Point3D> const_transformed_vertices = transformed_vertices;

    // Output transformed cube vertices
    std::cout << "\nTransformed Cube Vertices:" << std::endl;
    for (const auto &vertex : const_transformed_vertices)
    {
        std::cout << "{" << vertex.x << ", " << vertex.y << ", " << vertex.z << "}" << std::endl;
    }

    // Main rendering loop
    while (!glfwWindowShouldClose(window))
    {
        // Clearing buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Setting up view matrix
        glm::mat4 view = glm::lookAt(glm::vec3(3.0f, 2.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.0f, 0.0f));

        // Setting up model matrix
        glm::mat4 model = glm::mat4(1.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(glm::value_ptr(view * model));

        // Rendering original cube
        render3DCube(cube_vertices, redColor, blackColor);

        // Rendering transformed cube
        render3DCube(transformed_vertices, blueColor, blackColor);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
    return 0;
}

// Function to render a 3D cube
void render3DCube(const std::vector<Point3D> &vertices, const GLfloat color[3], GLfloat line_color[3])
{
    glColor3fv(color); // Set cube color

    // Render each face of the cube
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

    // left side
    glBegin(GL_QUADS);
    glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
    glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
    glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
    glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
    glEnd();

    // right side
    glBegin(GL_QUADS);
    glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
    glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
    glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
    glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
    glEnd();

    // top side
    glBegin(GL_QUADS);
    glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
    glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
    glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
    glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
    glEnd();

    // bottom side
    glBegin(GL_QUADS);
    glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
    glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
    glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
    glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
    glEnd();
    glColor3fv(line_color); // Set line color for edges

    // Render edges of the cube
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

// Callback function for framebuffer resize
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
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
