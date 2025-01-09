
#include "PlyFileLoaders.h"

#include <iostream>     // "input output" stream
#include <fstream>      // "file" stream
//#include <string>



//Return true if file is loaded; false if it is not.
bool ReadPlyModelFromFile_xyz_ci(s3DFileData& loadMeshInfo)
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

    std::ifstream plyFile(loadMeshInfo.fileName);
    std::string token = "";

    if (!plyFile.is_open())
    {
        return false;
    }

    // Search for vertices
    while (token != "vertex")
    {
        plyFile >> token;
    };
    plyFile >> loadMeshInfo.numberOfVertices;

    // Search for faces
    while (token != "face")
    {
        plyFile >> token;
    };
    plyFile >> loadMeshInfo.numberOfTriangles;

    // Search for the end of the file's header
    while (token != "end_header")
    {
        plyFile >> token;
    };

    // Spit out the count, for some helpful dev. feedback.
    std::cout << loadMeshInfo.numberOfVertices << std::endl;
    std::cout << loadMeshInfo.numberOfTriangles << std::endl;


    


    // Now load model data from file:
    // Model vertices:
    loadMeshInfo.pPlyVertices = new sPlyVertex[loadMeshInfo.numberOfVertices];
    for (unsigned index = 0; index != loadMeshInfo.numberOfVertices; index++)
    {
        plyFile >> loadMeshInfo.pPlyVertices[index].x;
        plyFile >> loadMeshInfo.pPlyVertices[index].y;
        plyFile >> loadMeshInfo.pPlyVertices[index].z;
        plyFile >> loadMeshInfo.pPlyVertices[index].confidence;
        plyFile >> loadMeshInfo.pPlyVertices[index].intensity;
    }
    // Model triangles
    loadMeshInfo.pPlyTriangles = new sTriangle[loadMeshInfo.numberOfTriangles];
    for (unsigned int index = 0; index != loadMeshInfo.numberOfTriangles; index++)
    {
        // 3 737 103 17 
        int discard = 0;
        plyFile >> discard;
        plyFile >> loadMeshInfo.pPlyTriangles[index].vertIndex_0;
        plyFile >> loadMeshInfo.pPlyTriangles[index].vertIndex_1;
        plyFile >> loadMeshInfo.pPlyTriangles[index].vertIndex_2;
    }

    return true;
}



bool ReadPlyModelFromFile_xyz(s3DFileData& loadMeshInfo)
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

    std::ifstream plyFile(loadMeshInfo.fileName);
    std::string token = "";

    if (!plyFile.is_open())
    {
        return false;
    }

    // Search for vertices
    while (token != "vertex")
    {
        plyFile >> token;
    };
    plyFile >> loadMeshInfo.numberOfVertices;

    // Search for faces
    while (token != "face")
    {
        plyFile >> token;
    };
    plyFile >> loadMeshInfo.numberOfTriangles;

    // Search for the end of the file's header
    while (token != "end_header")
    {
        plyFile >> token;
    };

    // Spit out the count, for some helpful dev. feedback.
    std::cout << loadMeshInfo.numberOfVertices << std::endl;
    std::cout << loadMeshInfo.numberOfTriangles << std::endl;





    // Now load model data from file:
    // Model vertices:
    loadMeshInfo.pPlyVertices = new sPlyVertex[loadMeshInfo.numberOfVertices];
    for (unsigned index = 0; index != loadMeshInfo.numberOfVertices; index++)
    {
        plyFile >> loadMeshInfo.pPlyVertices[index].x;
        plyFile >> loadMeshInfo.pPlyVertices[index].y;
        plyFile >> loadMeshInfo.pPlyVertices[index].z;
        loadMeshInfo.pPlyVertices[index].confidence = 0;
        loadMeshInfo.pPlyVertices[index].intensity = 0;
    }
    // Model triangles
    loadMeshInfo.pPlyTriangles = new sTriangle[loadMeshInfo.numberOfTriangles];
    for (unsigned int index = 0; index != loadMeshInfo.numberOfTriangles; index++)
    {
        // 3 737 103 17 
        int discard = 0;
        plyFile >> discard;
        plyFile >> loadMeshInfo.pPlyTriangles[index].vertIndex_0;
        plyFile >> loadMeshInfo.pPlyTriangles[index].vertIndex_1;
        plyFile >> loadMeshInfo.pPlyTriangles[index].vertIndex_2;
    }

    return true;
}