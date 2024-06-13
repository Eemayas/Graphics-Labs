// #include <glad/glad.h>
// #include <GLFW/glfw3.h>

// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

// #include <iostream>
// #include <cmath>
// #include <vector>

// // Settings
// constexpr unsigned int SCR_WIDTH = 800;
// constexpr unsigned int SCR_HEIGHT = 800;

// constexpr float BACKGROUND_COLOR[4] = {1.0, 1.0, 1.0, 1.0};
// constexpr float COLOR_YELLOW[3] = {244 / 255.0, 232 / 255.0, 112 / 255.0};
// constexpr float blackColor[3] = {0.0, 0.0, 0.0};
// constexpr float COLOR_WHITE[3] = {1.0, 1.0, 1.0};
// constexpr float COLOR_RED[3] = {1.0, 0.0, 0.0};
// constexpr float COLOR_GREEN[3] = {0.0, 1.0, 0.0};
// constexpr float COLOR_BLUE[3] = {0.0, 0.0, 1.0};

// typedef struct Point3D
// {
//     double x, y, z;
// } Point3D;

// void framebuffer_size_callback(GLFWwindow *window, int width, int height);
// void setPerspectiveProjection(int width, int height);
// void render3DCube(const std::vector<Point3D> &vertices, const GLfloat color[3], GLfloat line_color[3]);

// // Overload the << operator for glm::mat4
// std::ostream &operator<<(std::ostream &os, const glm::mat4 &matrix)
// {
//     const float *data = glm::value_ptr(matrix);
//     for (int i = 0; i < 4; ++i)
//     {
//         os << "| ";
//         for (int j = 0; j < 4; ++j)
//         {
//             os << data[j * 4 + i] << " "; // glm::value_ptr gives a column-major order array
//         }
//         os << "|\n";
//     }
//     return os;
// }

// void setOrthographicProjection(int width, int height)
// {
//     // Set the matrix mode to GL_PROJECTION for working with projection matrix
//     glMatrixMode(GL_PROJECTION);

//     // Load an identity matrix to reset any previous projection transformations
//     glLoadIdentity();

//     // Calculate the aspect ratio of the window
//     float aspectRatio = (float)width / (float)height;

//     // Set the size of the orthographic view (adjust as needed)
//     float orthoSize = 2.0f;

//     // Create an orthographic projection matrix using glm library
//     glm::mat4 projection = glm::ortho(-orthoSize * aspectRatio, orthoSize * aspectRatio, -orthoSize, orthoSize, -10.0f, 10.0f);

//     // Load the projection matrix into OpenGL
//     glLoadMatrixf(glm::value_ptr(projection));

//     // Set the matrix mode back to GL_MODELVIEW for working with model-view matrix
//     glMatrixMode(GL_MODELVIEW);
// }

// int main(int argc, char **argv)
// {
//     glfwInit();
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

//     GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Lab", NULL, NULL);
//     if (window == NULL)
//     {
//         std::cout << "Failed to create GLFW window" << std::endl;
//         glfwTerminate();
//         return -1;
//     }
//     glfwMakeContextCurrent(window);
//     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//     {
//         std::cout << "Failed to initialize GLAD" << std::endl;
//         return -1;
//     }

//     // Configure global OpenGL state
//     glEnable(GL_DEPTH_TEST);

//     // Set up the initial perspective projection
//     setPerspectiveProjection(SCR_WIDTH, SCR_HEIGHT);

//     // Transformation parameters
//     glm::mat4 transform = glm::mat4(1.0f);

//     setOrthographicProjection(SCR_WIDTH, SCR_HEIGHT);

//     while (!glfwWindowShouldClose(window))
//     {
//         // Rendering commands
//         glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         // Set the view transformation
//         glm::mat4 view = glm::lookAt(glm::vec3(3.0f, 2.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.0f, 0.0f));

//         // Render the original cube
//         glm::mat4 model = glm::mat4(1.0f); // Identity matrix for the original cube
//         glMatrixMode(GL_MODELVIEW);
//         glLoadMatrixf(glm::value_ptr(view * model));
//         std::vector<Point3D> cube_vertices = {
//             {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, // front face
//             {-0.5, -0.5, 0.5},
//             {0.5, -0.5, 0.5},
//             {0.5, 0.5, 0.5},
//             {-0.5, 0.5, 0.5}, // back face
//         };
//         render3DCube(cube_vertices, COLOR_RED, (GLfloat *)blackColor);

//         // Reset transformation matrix to identity
//         transform = glm::mat4(1.0f);

//         // Render the cube without any transformations
//         model = transform; // Apply the chosen transformation to the second cube
//         glLoadMatrixf(glm::value_ptr(view * model));
//         render3DCube(cube_vertices, COLOR_BLUE, (GLfloat *)blackColor);

//         // Event handling and buffer swapping
//         glfwSwapBuffers(window);
//         glfwPollEvents();
//     }

//     glfwTerminate();
//     return 0;
// }

// void render3DCube(const std::vector<Point3D> &vertices, const GLfloat color[3], GLfloat line_color[3])
// {
//     // the cube
//     glColor3fv(color);

//     // first four points forms a quadrilateral // front
//     glBegin(GL_QUADS);
//     for (int i = 0; i < 4; i++)
//     {
//         glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
//     }
//     glEnd();

//     // other four points forms another quadrilateral // back
//     glBegin(GL_QUADS);
//     for (int i = 4; i < 8; i++)
//     {
//         glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
//     }
//     glEnd();

//     // left side
//     glBegin(GL_QUADS);
//     glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
//     glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
//     glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
//     glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
//     glEnd();

//     // right side
//     glBegin(GL_QUADS);
//     glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
//     glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
//     glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
//     glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
//     glEnd();

//     // top side
//     glBegin(GL_QUADS);
//     glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
//     glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
//     glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
//     glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
//     glEnd();

//     // bottom side
//     glBegin(GL_QUADS);
//     glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
//     glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
//     glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
//     glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
//     glEnd();

//     // the lines
//     glColor3fv(line_color);
//     glBegin(GL_LINE_STRIP);
//     for (const Point3D &p : vertices)
//     {
//         glVertex3f(p.x, p.y, p.z);
//     }
//     glEnd();

//     // connecting the faces
//     glBegin(GL_LINES);
//     glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
//     glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
//     glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
//     glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
//     glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
//     glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
//     glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
//     glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
//     glEnd();
// }

// void framebuffer_size_callback(GLFWwindow *window, int width, int height)
// {
//     glViewport(0, 0, width, height);
// }

// void setPerspectiveProjection(int width, int height)
// {
//     glMatrixMode(GL_PROJECTION);
//     glLoadIdentity();
//     glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
//     glLoadMatrixf(glm::value_ptr(projection));
//     glMatrixMode(GL_MODELVIEW);
// }
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

// Settings
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 800;

constexpr float BACKGROUND_COLOR[4] = {1.0, 1.0, 1.0, 1.0};
constexpr float COLOR_YELLOW[3] = {244 / 255.0, 232 / 255.0, 112 / 255.0};
constexpr float COLOR_BLACK[3] = {0.0, 0.0, 0.0};
constexpr float COLOR_WHITE[3] = {1.0, 1.0, 1.0};
constexpr float COLOR_RED[3] = {1.0, 0.0, 0.0};
constexpr float COLOR_GREEN[3] = {0.0, 1.0, 0.0};
constexpr float COLOR_BLUE[3] = {0.0, 0.0, 1.0};

typedef struct Point3D
{
    double x, y, z;

    // Constructor for Point3D
    Point3D(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
} Point3D;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
std::vector<Point3D> setOrthographicProjection(int width, int height);
void render3DCube(const std::vector<Point3D> &vertices, const GLfloat color[3], GLfloat line_color[3]);
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
std::vector<Point3D> mat4ToPoints(const glm::mat4 &matrix)
{
    std::vector<Point3D> points;

    // Extract each column from the matrix
    glm::vec4 col1 = matrix[0];
    glm::vec4 col2 = matrix[1];
    glm::vec4 col3 = matrix[2];
    glm::vec4 col4 = matrix[3];

    // Create Point3D objects and add them to the vector
    points.push_back(Point3D(col1.x, col1.y, col1.z));
    points.push_back(Point3D(col2.x, col2.y, col2.z));
    points.push_back(Point3D(col3.x, col3.y, col3.z));
    points.push_back(Point3D(col4.x, col4.y, col4.z));

    return points;
}
std::vector<Point3D> setOrthographicProjection(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspectRatio = (float)width / (float)height;
    float orthoSize = 2.0f;

    glm::mat4 projection = glm::ortho(-orthoSize * aspectRatio, orthoSize * aspectRatio, -orthoSize, orthoSize, -10.0f, 10.0f);
    glLoadMatrixf(glm::value_ptr(projection));
    std::cout << projection;

    glMatrixMode(GL_MODELVIEW);
    return mat4ToPoints(projection);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Lab", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    std::vector<Point3D> project_coordinate = setOrthographicProjection(SCR_WIDTH, SCR_HEIGHT);

    std::vector<Point3D> cube_vertices = {
        {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, // front face
        {-0.5, -0.5, 0.5},
        {0.5, -0.5, 0.5},
        {0.5, 0.5, 0.5},
        {-0.5, 0.5, 0.5} // back face
    };

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(glm::vec3(3.0f, 2.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.0f, 0.0f));
        glm::mat4 model = glm::mat4(1.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(glm::value_ptr(view * model));

        // render3DCube(cube_vertices, COLOR_RED, (GLfloat *)COLOR_BLACK);
        render3DCube(project_coordinate, COLOR_RED, (GLfloat *)COLOR_BLACK);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void render3DCube(const std::vector<Point3D> &vertices, const GLfloat color[3], GLfloat line_color[3])
{
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    setOrthographicProjection(width, height);
}
