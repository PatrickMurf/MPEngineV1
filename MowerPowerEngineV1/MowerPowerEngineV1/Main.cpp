#include "GLCommon.h"

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

#include "PlyFileLoaders.h"
#include "Basic_Shader_Manager/cShaderManager.h"

struct sVertex
{
    glm::vec3 pos;      // position   or "float x, y, z"
    glm::vec3 col;      // Colour     or "float x, y, z"
    // Colour range is 0.0 to 1.0
    // 0.0 = black (Red, Green, Blue)
    // 1.0 = white 
};

//static const char* vertex_shader_text =
//"#version 330\n"
//"uniform mat4 MVP;\n"
//"in vec3 vCol;\n"
//"in vec3 vPos;\n"
//"out vec3 color;\n"
//"void main()\n"
//"{\n"
//"    gl_Position = MVP * vec4(vPos, 1.0);\n"
//"    color = vCol;\n"
//"}\n";
//
//static const char* fragment_shader_text =
//"#version 330\n"
//"in vec3 color;\n"
//"out vec4 fragment;\n"
//"void main()\n"
//"{\n"
//"    fragment = vec4(color, 1.0);\n"
//"}\n";


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

int main(void)
{
    
    s3DFileData plyFileInfoBunny;
    plyFileInfoBunny.fileName = "assets/models/bun_zipper.ply";
    ReadPlyModelFromFile_xyz_ci(plyFileInfoBunny);

    s3DFileData plyFileInfoCar;
    plyFileInfoCar.fileName = "assets/models/VintageRacingCar_xyz_only.ply";
    ReadPlyModelFromFile_xyz(plyFileInfoCar);

    s3DFileData plyFileInfo;
    plyFileInfo.fileName = "assets/models/Dragon 2.5Edited_xyz_only.ply";
    ReadPlyModelFromFile_xyz(plyFileInfo);


    // Array given to the GPU
    unsigned int numberOfVertices_TO_DRAW = plyFileInfo.numberOfTriangles * 3;
    sVertex* pVertices = new sVertex[numberOfVertices_TO_DRAW];

    // Putting model data into array
    unsigned int vertexIndex = 0;
    for (unsigned int triIndex = 0; triIndex != plyFileInfo.numberOfTriangles; triIndex++)
    {
        pVertices[vertexIndex + 0].pos.x = plyFileInfo.pPlyVertices[plyFileInfo.pPlyTriangles[triIndex].vertIndex_0].x;
        pVertices[vertexIndex + 0].pos.y = plyFileInfo.pPlyVertices[plyFileInfo.pPlyTriangles[triIndex].vertIndex_0].y;
        pVertices[vertexIndex + 0].pos.z = plyFileInfo.pPlyVertices[plyFileInfo.pPlyTriangles[triIndex].vertIndex_0].z;
        pVertices[vertexIndex + 0].col.r = 1.0f;
        pVertices[vertexIndex + 0].col.g = 1.0f;
        pVertices[vertexIndex + 0].col.b = 1.0f;

        pVertices[vertexIndex + 1].pos.x = plyFileInfo.pPlyVertices[plyFileInfo.pPlyTriangles[triIndex].vertIndex_1].x;
        pVertices[vertexIndex + 1].pos.y = plyFileInfo.pPlyVertices[plyFileInfo.pPlyTriangles[triIndex].vertIndex_1].y;
        pVertices[vertexIndex + 1].pos.z = plyFileInfo.pPlyVertices[plyFileInfo.pPlyTriangles[triIndex].vertIndex_1].z;
        pVertices[vertexIndex + 1].col.r = 1.0f;
        pVertices[vertexIndex + 1].col.g = 1.0f;
        pVertices[vertexIndex + 1].col.b = 1.0f;

        pVertices[vertexIndex + 2].pos.x = plyFileInfo.pPlyVertices[plyFileInfo.pPlyTriangles[triIndex].vertIndex_2].x;
        pVertices[vertexIndex + 2].pos.y = plyFileInfo.pPlyVertices[plyFileInfo.pPlyTriangles[triIndex].vertIndex_2].y;
        pVertices[vertexIndex + 2].pos.z = plyFileInfo.pPlyVertices[plyFileInfo.pPlyTriangles[triIndex].vertIndex_2].z;
        pVertices[vertexIndex + 2].col.r = 1.0f;
        pVertices[vertexIndex + 2].col.g = 1.0f;
        pVertices[vertexIndex + 2].col.b = 1.0f;

        vertexIndex += 3;
    }


    // Example of transforming a model:
    // Scaling
//    for (unsigned int index = 0; index != numberOfVertices_TO_DRAW; index++)
//    {
//        pVertices[index].pos.x *= 0.01f;
//        pVertices[index].pos.y *= 0.01f;
//        pVertices[index].pos.z *= 0.01f;
//    }
    // Moving
//    for (unsigned int index = 0; index != numberOfVertices_TO_DRAW; index++)
//    {
//        pVertices[index].pos.x += 1.0f;
//    }


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

    cShaderManager* pShaderManager = new cShaderManager();

    cShaderManager::cShader vertexShader;
    vertexShader.fileName = "assets/shaders/vertex01.glsl";

    cShaderManager::cShader fragmentShader;
    fragmentShader.fileName = "assets/shaders/fragment01.glsl";

    if (!pShaderManager->createProgramFromFile(
        "shader01",
        vertexShader,
        fragmentShader
    ))
    {
        std::cout << "Error: " << pShaderManager->getLastError() << std::endl;
    }
    else
    {
        std::cout << "Shader built okay." << std::endl;
    }

    const GLuint program = pShaderManager->getIDFromFriendlyName("shader01");
    glUseProgram(program);

    /*
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
    */

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

        float angleInRadians = (float)glfwGetTime() / 5.0f;

        glm::mat4 rotateZ =
            glm::rotate(glm::mat4(1.0f),            // Ignore this
                angleInRadians, //(float)glfwGetTime(),       // Angle in radians
                glm::vec3(0.0f, 0.0, 1.0f));        // Axis it rotates around
        m = m * rotateZ;

        glm::mat4 translate = glm::translate(glm::mat4(1.0f),
            glm::vec3(5.0f, 0.0f, 0.0f));
        m = m * translate;


        glm::mat4 scale = glm::scale(glm::mat4(1.0f),
            glm::vec3(0.2f, 0.2f, 0.02f));
        m = m * scale;

        //mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        p = glm::perspective(
            0.6f,       // FOV
            ratio,      // Aspect ratio of screen
            0.1f,       // Near plane
            1000.0f);   // Far plane

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
        /*  Info:
        * M = Model matrix
        * V = View matrix
        * P = Projection matrix
        */

        glUseProgram(program);
        const GLint mvp_location = glGetUniformLocation(program, "MVP");
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp);
        glBindVertexArray(vertex_array);
        //        glDrawArrays(GL_TRIANGLES, 0, 3);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, numberOfVertices_TO_DRAW);

        glfwSwapBuffers(window);
        glfwPollEvents();

        //std::cout << "Camera: "
        //    << cameraEye.x << ", "
        //    << cameraEye.y << ", "
        //    << cameraEye.z << std::endl;

    }// End of the draw loop

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}