#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Debugger.h>
#include <VertexBuffer.h>
#include <VertexBufferLayout.h>
#include <IndexBuffer.h>
#include <VertexArray.h>
#include <Shader.h>
#include <Texture.h>
#include <Camera.h>
#include <smallCube.h>
#include <RubiksCube.h>
#include <iostream>


const float FOVdegree = 45.0f;  // Field Of View Angle
const float near = 0.1f;
const float far = 100.0f;
RubiksCube rubiksCube;



float vertices[] = {
    // positions           // colors           // texCoords
    // Front face (Red)
    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // Bottom-left (Red)
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // Bottom-right (Red)
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top-right (Red)
    -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // Top-left (Red)

    // Back face (Orange)
    -0.5f, -0.5f, -0.5f,   1.0f, 0.5f, 0.0f,   0.0f, 0.0f, // Bottom-left (Orange)
     0.5f, -0.5f, -0.5f,   1.0f, 0.5f, 0.0f,   1.0f, 0.0f, // Bottom-right (Orange)
     0.5f,  0.5f, -0.5f,   1.0f, 0.5f, 0.0f,   1.0f, 1.0f, // Top-right (Orange)
    -0.5f,  0.5f, -0.5f,   1.0f, 0.5f, 0.0f,   0.0f, 1.0f, // Top-left (Orange)

    // Left face (Green)
    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // Bottom-left (Green)
    -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom-right (Green)
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // Top-right (Green)
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, // Top-left (Green)

    // Right face (Blue)
     0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom-left (Blue)
     0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // Bottom-right (Blue)
     0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // Top-right (Blue)
     0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // Top-left (Blue)

    // Top face (White)
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom-left (White)
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom-right (White)
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top-right (White)
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // Top-left (White)

    // Bottom face (Yellow)
    -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, // Bottom-left (Yellow)
     0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom-right (Yellow)
     0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, // Top-right (Yellow)
    -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top-left (Yellow)
};


/* Indices for cube faces */
unsigned int indices[] = {
    // Front face
    0, 1, 2, 2, 3, 0,
    // Back face
    4, 5, 6, 6, 7, 4,
    // Left face
    8, 9, 10, 10, 11, 8,
    // Right face
    12, 13, 14, 14, 15, 12,
    // Top face
    16, 17, 18, 18, 19, 16,
    // Bottom face
    20, 21, 22, 22, 23, 20
};

int main(int argc, char* argv[]) {
    GLFWwindow* window;
    const unsigned int width = 800, height = 800;
    const float FOVdegree = 45.0f, near = 0.1f, far = 100.0f;

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    /* Set OpenGL version to 3.3 */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a window */
    window = glfwCreateWindow(width, height, "OpenGL Rubik's Cube", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);


    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    {
        /* Enable depth testing */
        GLCall(glEnable(GL_DEPTH_TEST));
    
        /* Set up buffers and shaders */
        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices));
        IndexBuffer ib(indices, sizeof(indices));

        VertexBufferLayout layout;
        layout.Push<float>(3); // positions
        layout.Push<float>(3); // colors
        layout.Push<float>(2); // texCoords
        va.AddBuffer(vb, layout);

        Texture texture("res/textures/plane.png");
        texture.Bind();

        Shader shader("res/shaders/basic.shader");
        shader.Bind();

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        /* Initialize the camera */
        Camera camera(width, height , rubiksCube);
        camera.SetPerspective(FOVdegree, near, far);
        camera.SetPosition(glm::vec3(0.0f, 0.0f, 10.0f)); 
        camera.EnableInputs(window);


        while (!glfwWindowShouldClose(window)) {
            GLCall(glClearColor(1.0f, 1.0f, 1.0f, 1.0f)); // Set background to white
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            glm::mat4 view = camera.GetViewMatrix();  // Camera view matrix
            glm::mat4 proj = camera.GetProjectionMatrix();  // Camera projection matrix

            // Render Rubik's Cube using the RubiksCube class

            
            rubiksCube.render(shader, va, ib, proj, view , window);

            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}