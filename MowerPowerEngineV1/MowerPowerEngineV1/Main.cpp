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
#include "sharedThings.h"

const unsigned int MAX_NUMBER_OF_MESHES = 1000;
unsigned int g_NumberOfMeshesToDraw;
sMesh* g_myMeshes[MAX_NUMBER_OF_MESHES] = { 0 };    // Set all to zeros, by default





static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    const float CAMERA_MOVE_SPEED = 0.1f;

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

    // Callback for keyboard, but for "typing"
    // Like it captures the press and release and repeat
    glfwSetKeyCallback(window, key_callback);

    glfwSetCursorPosCallback(window, cursor_position_callback);     // Gets the mouse position
    glfwSetWindowFocusCallback(window, cursor_enter_callback);      // Prevents it from tracking the mouse off the window
    glfwSetMouseButtonCallback(window, mouse_button_callback);      // When you press a button
    glfwSetScrollCallback(window, scroll_callback);                 // For the scrollwheel

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);


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


    ::g_pFlyCamera = new cBasicFlyCamera();



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

    sModelDrawInfo terrainModel;
    pMeshManager->LoadModelIntoVAO("assets/models/Simple_MeshLab_terrain_xyz_only.ply",
        terrainModel, program);
    std::cout << terrainModel.numberOfVertices << " vertices loaded" << std::endl;


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


    sMesh* pTerrainMesh = new sMesh();
    pTerrainMesh->modelFileName = "assets/models/Simple_MeshLab_terrain_xyz_only.ply";
    pTerrainMesh->positionXYZ = glm::vec3(0.0f, -25.0f, 0.0f);
    //pTerrainMesh->rotationEulerXYZ.x = 90.0f;
    pTerrainMesh->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    pTerrainMesh->bOverrideObjectColour = true;
    //    pTerrainMesh->bIsWireframe = true;
    ::g_myMeshes[::g_NumberOfMeshesToDraw] = pTerrainMesh;
    ::g_NumberOfMeshesToDraw++;


    for (int count = 0; count != 100; count++)
    {
        sMesh* pDragon = new sMesh();
        pDragon->modelFileName = "assets/models/VintageRacingCar_xyz_only.ply";
        pDragon->positionXYZ = glm::vec3(
            getRandomFloat(-5.0f, 5.0f),
            getRandomFloat(-5.0f, 5.0f),
            getRandomFloat(-5.0f, 5.0f));
        pDragon->rotationEulerXYZ.x = 90.0f;
        pDragon->objectColourRGBA = glm::vec4(
            getRandomFloat(-5.0f, 5.0f),
            getRandomFloat(-5.0f, 5.0f),
            getRandomFloat(-5.0f, 5.0f),
            1.0f);
        pDragon->uniformScale = 0.2f;

        if (rand() * 10 % 5)
        {
            pDragon->bIsWireframe = rand() % 2;     // WARNING:  This is a temp thing; this line of code is ill-advised.
                                                        // Will randomly assign either 0, or 1.
        }

        ::g_myMeshes[::g_NumberOfMeshesToDraw] = pDragon;

        ::g_NumberOfMeshesToDraw++;
    }

    // I think this was a new add I missed?:
    /*
    ::g_pFlyCamera = new cBasicFlyCamera();
    ::g_pFlyCamera->setEyeLocation(glm::vec3(0.0f, 0.0f, -10.0f));
    */


    glUseProgram(program);

    // Enable depth buffering (z buffering)
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glEnable.xhtml
    glEnable(GL_DEPTH_TEST);


    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        //       mat4x4 m, p, mvp;
        glm::mat4 m, p, v, mvp;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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
            ::g_pFlyCamera->getEyeLocation(),
            ::g_pFlyCamera->getTargetLocation(),
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

            if (pCurrentMesh->bIsVisible)
            {
                continue;
            }

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
            

            // uniform bool bUseObjectColour;
            GLint bUseObjectColour = glGetUniformLocation(program, "objectColour");
            if (pCurrentMesh->bOverrideObjectColour)
            {
                // bool doesn't really exist; it's a float...
                glUniform1f(bUseObjectColour, GLfloat(GL_TRUE));    // or 1.0f
            }
            else
            {
                glUniform1f(bUseObjectColour, GLfloat(GL_FALSE));   // or 0.0f
            }

            // Set the object colour
            // Reminder:    uniform vec4 objectColour;
            GLint objectColour_UL = glGetUniformLocation(program, "objectColour");
            glUniform4f(
                objectColour_UL,
                pCurrentMesh->objectColourRGBA.r,
                pCurrentMesh->objectColourRGBA.g,
                pCurrentMesh->objectColourRGBA.b,
                1.0f);

            // This one can do solid, wireframe, & other options
            if (pCurrentMesh->bIsWireframe)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            //glDrawArrays(GL_TRIANGLES, 0, numberOfVertices_TO_DRAW);

            sModelDrawInfo meshToDrawInfo;
            if (pMeshManager->FindDrawInfoByModelName(pCurrentMesh->modelFileName, meshToDrawInfo))
            {
                glBindVertexArray(meshToDrawInfo.VAO_ID); 		// enable VAO(and everything else)
                //https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDrawElements.xhtml
                glDrawElements(GL_TRIANGLES,
                    meshToDrawInfo.numberOfIndices,
                    GL_UNSIGNED_INT,
                    (void*)0
                );
                glBindVertexArray(0); 			//disable VAO(and everything else)
            }
        }

        // Handle async IO stuff
        handleKeyboardAsync(window);
        handleMouseAsync(window);

        glfwSwapBuffers(window);    // End of the render call.  This is where it swaps the screens (One screen you view, the other it draws to.)
        glfwPollEvents();

        std::stringstream ssTitle;
        ssTitle << "Camera: "
            << ::g_pFlyCamera->getEyeLocation().x << ", "
            << ::g_pFlyCamera->getEyeLocation().y << ", "
            << ::g_pFlyCamera->getEyeLocation().z << std::endl;
    //    ssTitle << "Camera: "
    //        << cameraEye.x << ", "
    //        << cameraEye.y << ", "
    //        << cameraEye.z << std::endl;
        glfwSetWindowTitle(window, ssTitle.str().c_str());

    }// End of the draw loop
    
    delete ::g_pFlyCamera;

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}