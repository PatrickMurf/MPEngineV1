#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include <iostream>     // "input output" stream
#include <fstream>      // "file" stream
#include <string>

struct sVertex
{
    glm::vec3 pos;      // position   or "float x, y, z"
    glm::vec3 col;      // Colour     or "float x, y, z"
    // Colour range is 0.0 to 1.0
    // 0.0 = black (Red, Green, Blue)
    // 1.0 = white 
};

static const char* vertex_shader_text =
"#version 330\n"
"uniform mat4 MVP;\n"
"in vec3 vCol;\n"
"in vec3 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 330\n"
"in vec3 color;\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
"    fragment = vec4(color, 1.0);\n"
"}\n";


glm::vec3 cameraEye = glm::vec3(0.0, 0.0, 4.0f);



static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    const float CAMERA_MOVE_SPEED = 0.1f;

    if (key == GLFW_KEY_A)
    {
        cameraEye.x -= CAMERA_MOVE_SPEED;
    }

    if (key == GLFW_KEY_D)
    {
        cameraEye.x += CAMERA_MOVE_SPEED;
    }

    if (key == GLFW_KEY_W)
    {
        cameraEye.z -= CAMERA_MOVE_SPEED;
    }

    if (key == GLFW_KEY_S)
    {
        cameraEye.z += CAMERA_MOVE_SPEED;
    }

    if (key == GLFW_KEY_Q)
    {
        cameraEye.y -= CAMERA_MOVE_SPEED;
    }
    if (key == GLFW_KEY_E)
    {
        cameraEye.y += CAMERA_MOVE_SPEED;
    }
}

void FileHandling(void);

int main(void)
{
    /*
    // This is on the stack; allocated at runtime.
    sVertex vertices[3] =
    {
        { { -0.6f, -0.4f }, { 1.0f, 0.0f, 0.0f } },
        { {  0.6f, -0.4f }, { 0.0f, 1.0f, 0.0f } },
        { {  0.0f,  0.6f }, { 0.0f, 0.0f, 1.0f } }
    };

    // This is on the heap; thus dynamically allocated at run time.
    sVertex* pVertices = new sVertex[3];
    pVertices[0] = { { -0.6f, -0.4f }, { 1.0f, 0.0f, 0.0f } };
    pVertices[1] = { {  0.6f, -0.4f }, { 0.0f, 1.0f, 0.0f } };
    pVertices[2] = { {  0.0f,  0.6f }, { 0.0f, 0.0f, 1.0f } };
    */

    std::ifstream plyFile("assets/models/bun_zipper_res3.ply");
    std::string token = "";

    // Search for vertices
    while (token != "vertex")
    {
        plyFile >> token;
    };
    int numberOfVertices = 0;
    plyFile >> numberOfVertices;

    // Search for faces
    while (token != "face")
    {
        plyFile >> token;
    };
    int numberOfTriangles = 0;
    plyFile >> numberOfTriangles;

    // Search for the end of the file's header
    while (token != "end_header")
    {
        plyFile >> token;
    };

    // Spit out the count, for some helpful dev. feedback.
    std::cout << numberOfVertices << std::endl;
    std::cout << numberOfTriangles << std::endl;


    // The bunny's data structure
    struct sPlyVertex
    {
        float x, y, z, confidence, intensity;
    };
    struct sTriangle
    {
        unsigned int vertIndex_0;
        unsigned int vertIndex_1;
        unsigned int vertIndex_2;
    };


    // Now load model data from file:
    // Model vertices:
    sPlyVertex* pPlyVertices = new sPlyVertex[numberOfVertices];
    for (unsigned index = 0; index != numberOfVertices; index++)
    {
        plyFile >> pPlyVertices[index].x;
        plyFile >> pPlyVertices[index].y;
        plyFile >> pPlyVertices[index].z;
        plyFile >> pPlyVertices[index].confidence;
        plyFile >> pPlyVertices[index].intensity;
    }
    // Model triangles
    sTriangle* pPlyTriangles = new sTriangle[numberOfTriangles];
    for (unsigned int index = 0; index != numberOfTriangles; index++)
    {
        // 3 737 103 17 
        int discard = 0;
        plyFile >> discard;
        plyFile >> pPlyTriangles[index].vertIndex_0;
        plyFile >> pPlyTriangles[index].vertIndex_1;
        plyFile >> pPlyTriangles[index].vertIndex_2;
    }


    // Array given to the GPU
    unsigned int numberOfVertices_TO_DRAW = numberOfTriangles * 3;
    sVertex* pVertices = new sVertex[numberOfVertices_TO_DRAW];

    // Putting model data into array
    unsigned int vertexIndex = 0;
    for (unsigned int triIndex = 0; triIndex != numberOfTriangles; triIndex++)
    {
        pVertices[vertexIndex + 0].pos.x = pPlyVertices[pPlyTriangles[triIndex].vertIndex_0].x;
        pVertices[vertexIndex + 0].pos.y = pPlyVertices[pPlyTriangles[triIndex].vertIndex_0].y;
        pVertices[vertexIndex + 0].pos.z = pPlyVertices[pPlyTriangles[triIndex].vertIndex_0].z;
        pVertices[vertexIndex + 0].col.r = 1.0f;
        pVertices[vertexIndex + 0].col.g = 1.0f;
        pVertices[vertexIndex + 0].col.b = 1.0f;

        pVertices[vertexIndex + 1].pos.x = pPlyVertices[pPlyTriangles[triIndex].vertIndex_1].x;
        pVertices[vertexIndex + 1].pos.y = pPlyVertices[pPlyTriangles[triIndex].vertIndex_1].y;
        pVertices[vertexIndex + 1].pos.z = pPlyVertices[pPlyTriangles[triIndex].vertIndex_1].z;
        pVertices[vertexIndex + 1].col.r = 1.0f;
        pVertices[vertexIndex + 1].col.g = 1.0f;
        pVertices[vertexIndex + 1].col.b = 1.0f;

        pVertices[vertexIndex + 2].pos.x = pPlyVertices[pPlyTriangles[triIndex].vertIndex_2].x;
        pVertices[vertexIndex + 2].pos.y = pPlyVertices[pPlyTriangles[triIndex].vertIndex_2].y;
        pVertices[vertexIndex + 2].pos.z = pPlyVertices[pPlyTriangles[triIndex].vertIndex_2].z;
        pVertices[vertexIndex + 2].col.r = 1.0f;
        pVertices[vertexIndex + 2].col.g = 1.0f;
        pVertices[vertexIndex + 2].col.b = 1.0f;

        vertexIndex += 3;
    }


    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    //    int size_in_bytes_of_vertex_array = sizeof(sVertex) * 3;
    int size_in_bytes_of_vertex_array = sizeof(sVertex) * numberOfVertices_TO_DRAW;

    glBufferData(GL_ARRAY_BUFFER,
        size_in_bytes_of_vertex_array,  // The size of the data being copied
        pVertices,                      // Where is the data copied from
        GL_STATIC_DRAW);

    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    const GLint mvp_location = glGetUniformLocation(program, "MVP");
    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    const GLint vcol_location = glGetAttribLocation(program, "vCol");

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(
        vpos_location,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(sVertex),                    // The "stride", aka, how many bytes between vertices
        (void*)offsetof(sVertex, pos));     // This looks for how far into the data "pos" is, so that it's always grabbing the correct value
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(
        vcol_location,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(sVertex),
        (void*)offsetof(sVertex, col));

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        //       mat4x4 m, p, mvp;
        glm::mat4 m, p, v, mvp;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        //         mat4x4_identity(m);
        m = glm::mat4(1.0f);

        //mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        glm::mat4 rotateZ =
            glm::rotate(glm::mat4(1.0f),        // Ignore this
                0.0f, //(float)glfwGetTime(),               // Angle in radians
                glm::vec3(0.0f, 0.0, 1.0f));        // Axis it rotates around

        m = m * rotateZ;

        //mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        p = glm::perspective(0.6f,
            ratio,
            0.1f,
            1000.0f);

        v = glm::mat4(1.0f);

        //        glm::vec3 cameraEye = glm::vec3(0.0, 0.0, 4.0f);
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

        v = glm::lookAt(
            cameraEye,
            cameraTarget,
            upVector);

        //mat4x4_mul(mvp, p, m);
        mvp = p * v * m;
        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp);
        glBindVertexArray(vertex_array);
        //        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 0, numberOfVertices_TO_DRAW);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

// "o" for output.
// std::ofstream myFile("someData.sdfs");
// "i" for input
void FileHandling(void)
{
    std::ifstream readFile("bun_zipper.ply");
    if (readFile.is_open())
    {
        std::string fileDataLine;
        while (readFile >> fileDataLine)
        {
            // Could write data to console here.
        };
    }
    else
    {
        std::cout << "Couldn't find the file." << std::endl;
    }
}