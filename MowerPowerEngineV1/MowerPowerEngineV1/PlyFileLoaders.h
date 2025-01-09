#ifndef _PlyFileLoaders_HG_
#define _PlyFileLoaders_HG_

#include <string>

// Ply File mesh data structures
// Info for a vertex postion
struct sPlyVertex
{
    float x, y, z, confidence, intensity;
};
// Info for a triangle
struct sTriangle
{
    unsigned int vertIndex_0;
    unsigned int vertIndex_1;
    unsigned int vertIndex_2;
};
// Data froma  model file
struct s3DFileData
{
	std::string fileName;
	int numberOfVertices;
	int numberOfTriangles;	
    sPlyVertex* pPlyVertices = NULL;
    sTriangle* pPlyTriangles = NULL;
};


// Note that it's passing by reference, via the '&', not by value.
bool ReadPlyModelFromFile_xyz_ci(s3DFileData& loadMeshInfo);
bool ReadPlyModelFromFile_xyz(s3DFileData& loadMeshInfo);

#endif // !_PlyFileLoaders_HG_
