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
#include <sstream>      // "string" stream ("string builder" in Java c#, etc.)
#include <string>

#include "PlyFileLoaders.h"
#include "Basic_Shader_Manager/cShaderManager.h"
#include "sMesh.h"
#include "cVAOManager/cVAOManager.h"

const unsigned int MAX_NUMBER_OF_MESHES = 1000;
unsigned int g_NumberOfMeshesToDraw;
sMesh* g_myMeshes[MAX_NUMBER_OF_MESHES] = { 0 };    // Set all to zeros, by default

/*
struct sVertex
{
    glm::vec3 pos;      // position   or "float x, y, z"
    glm::vec3 col;      // Colour     or "float x, y, z"
    // Colour range is 0.0 to 1.0
    // 0.0 = black (Red, Green, Blue)
    // 1.0 = white 
};
*/

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


// https://stackoverflow.com/questions/5289613/generate-random-float-between-two-floats
float getRandomFloat(float a, float b) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}


int main(void)
{
    
    /*
    s3DFileData plyFileInfoBunny;
    plyFileInfoBunny.fileName = "assets/models/bun_zipper.ply";
    ReadPlyModelFromFile_xyz_ci(plyFileInfoBunny);

    s3DFileData plyFileInfoCar;
    plyFileInfoCar.fileName = "assets/models/VintageRacingCar_xyz_only.ply";
    ReadPlyModelFromFile_xyz(plyFileInfoCar);

    s3DFileData plyFileInfo;
    plyFileInfo.fileName = "assets/models/Dragon 2.5Edited_xyz_only.ply";
    ReadPlyModelFromFile_xyz(plyFileInfo);
    */


    /*
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
    */


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

    /*
    // NOTE: OpenGL error checks have been omitted for brevity
    // This asks the GPU to allocate some memory, so that the CPU can copy data for it.
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    //    int size_in_bytes_of_vertex_array = sizeof(sVertex) * 3;
    int size_in_bytes_of_vertex_array = sizeof(sVertex) * numberOfVertices_TO_DRAW;
    // This is where it actually copies data form the CPU to the GPU.
    glBufferData(GL_ARRAY_BUFFER,
        size_in_bytes_of_vertex_array,  // The size of the data being copied
        pVertices,                      // Where is the data copied from
        GL_STATIC_DRAW);
    */

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

    /*
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
    */


    cVAOManager* pMeshManager = new cVAOManager();

    sModelDrawInfo carModelInfo;
    pMeshManager->LoadModelIntoVAO(
        "assets/models/VintageRacingCar_xyz_only.ply",
        carModelInfo,
        program);
    std::cout << carModelInfo.numberOfVertices << " vertices loaded" << std::endl;

    sModelDrawInfo dragonModel;
    pMeshManager->LoadModelIntoVAO("assets/models/Dragon 2.5Edited_xyz_only.ply",
        dragonModel, program);
    std::cout << dragonModel.numberOfVertices << " vertices loaded" << std::endl;


    // Loading some models to draw:
    sMesh* pDragon = new sMesh();
    pDragon->modelFileName = "assets/models/Dragon 2.5Edited_xyz_only.ply";
    pDragon->positionXYZ = glm::vec3(20.0f, 0.0f, 0.0f);
    pDragon->rotationEulerXYZ.x = -90.0f;
    pDragon->uniformScale = 0.1f;
    pDragon->objectColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    ::g_myMeshes[0] = pDragon;

    sMesh* pDragon2 = new sMesh();
    pDragon2->modelFileName = "assets/models/Dragon 2.5Edited_xyz_only.ply";
    pDragon2->positionXYZ = glm::vec3(-20.0f, 0.0f, 0.0f);
    pDragon2->rotationEulerXYZ.x = 90.0f;
    pDragon2->uniformScale = 0.2f;
    pDragon2->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    ::g_myMeshes[1] = pDragon2;

    ::g_NumberOfMeshesToDraw = 2;

    for (int count = 0; count != 100; count++)
    {
        sMesh* pDragon = new sMesh();
        pDragon->modelFileName = "assets/models/VintageRacingCar_xyz_only.ply";
        pDragon->positionXYZ = glm::vec3(getRandomFloat(-5.0f, 5.0f),
            getRandomFloat(-5.0f, 5.0f),
            getRandomFloat(-5.0f, 5.0f));
        pDragon->rotationEulerXYZ.x = 90.0f;
        pDragon->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        pDragon->uniformScale = 0.2f;

        ::g_myMeshes[::g_NumberOfMeshesToDraw] = pDragon;

        ::g_NumberOfMeshesToDraw++;
    }


    glUseProgram(program);


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


        glm::mat4 matProjection = glm::mat4(1.0f);
        matProjection = glm::perspective(
            0.6f,       // FOV
            ratio,      // Aspect ratio of screen
            0.1f,       // Near plane
            1000.0f);   // Far plane
            
        // View or "camera"
        glm::mat4 matView = glm::mat4(1.0f);

        //        glm::vec3 cameraEye = glm::vec3(0.0, 0.0, 4.0f);
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

        matView = glm::lookAt(
            cameraEye,
            cameraTarget,
            upVector);

    //    //mat4x4_mul(mvp, p, m);
    //    mvp = p * v * m;
    //    /*  Info:
    //    * M = Model matrix
    //    * V = View matrix
    //    * P = Projection matrix
    //    */


        // Draw all the objects
        for (unsigned int meshIndex = 0; meshIndex != g_NumberOfMeshesToDraw; meshIndex++)
        {
            sMesh* pCurrentMesh = ::g_myMeshes[meshIndex];

            //         mat4x4_identity(m);
            // Could be called the "model" or "world" matrix.  Was explained using the example of a camera moving aorund a stationary movie prop spaceship.
            glm::mat4 matModel = glm::mat4(1.0f);

            // Translation (movement, position, & placement)
            glm::mat4 matTranslate
                = glm::translate(glm::mat4(1.0f),
                    glm::vec3(
                    pCurrentMesh->positionXYZ.x,
                    pCurrentMesh->positionXYZ.y,
                    pCurrentMesh->positionXYZ.z
                ));

            // Rotation
            // Calculating 3 Euler axis matricies 
            glm::mat4 matRotateX =
                glm::rotate(glm::mat4(1.0f),            // Ignore this
                    pCurrentMesh->rotationEulerXYZ.x,   // Angle in radians
                    glm::vec3(1.0f, 0.0, 0.0f));        // Axis it rotates around

            glm::mat4 matRotateY =
                glm::rotate(glm::mat4(1.0f),            // Ignore this
                    pCurrentMesh->rotationEulerXYZ.y,   // Angle in radians
                    glm::vec3(0.0f, 1.0, 0.0f));        // Axis it rotates around

            glm::mat4 matRotateZ =
                glm::rotate(glm::mat4(1.0f),            // Ignore this
                    pCurrentMesh->rotationEulerXYZ.z,   // Angle in radians
                    glm::vec3(0.0f, 0.0, 1.0f));        // Axis it rotates around

            // Scale
            glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
                glm::vec3(pCurrentMesh->uniformScale, pCurrentMesh->uniformScale, pCurrentMesh->uniformScale));

            // Calculate the final model world matrix:
            // Note:  A quirk about the math:  The thing at the bottom of the list, is like the 1st thing to be applied.
            // Later on, a "pChildMeshes" thing would be added; for models attached to other models.  (Example:  Wheels & doors on a car.)
            matModel *= matTranslate;     // A better version;    matModel *= matTranslate
            matModel *= matRotateX;
            matModel *= matRotateY;
            matModel *= matRotateZ;
            matModel *= matScale;

            glm::mat4 matMVP = matProjection * matView * matModel;
                
            //glUseProgram(program);
            const GLint mvp_location = glGetUniformLocation(program, "MVP");
            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&matMVP);
            //glBindVertexArray(vertex_array);
            // This one can do solid, wireframe, & other options
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            //glDrawArrays(GL_TRIANGLES, 0, numberOfVertices_TO_DRAW);

            sModelDrawInfo meshToDrawInfo;
            if (pMeshManager->FindDrawInfoByModelName(pCurrentMesh->modelFileName, meshToDrawInfo))
            {
                glBindVertexArray(meshToDrawInfo.VAO_ID); 		// enable VAO(and everything else)
                //https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDrawElements.xhtml
                glDrawElements(GL_TRIANGLES,
                    meshToDrawInfo.numberOfTriangles,
                    GL_UNSIGNED_INT,
                    (void*)0
                );
                glBindVertexArray(0); 			//disable VAO(and everything else)
            }
        }

        glfwSwapBuffers(window);    // End of the render call.  This is where it swaps the screens (One screen you view, the other it draws to.)
        glfwPollEvents();

        //std::cout << "Camera: "
        //    << cameraEye.x << ", "
        //    << cameraEye.y << ", "
        //    << cameraEye.z << std::endl;

        std::stringstream ssTitle;
        ssTitle << "Camera: "
            << cameraEye.x << ", "
            << cameraEye.y << ", "
            << cameraEye.z << std::endl;
        glfwSetWindowTitle(window, ssTitle.str().c_str());

    }// End of the draw loop

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}