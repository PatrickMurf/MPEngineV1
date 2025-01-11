#pragma once

// The 3D drawing informaiton, for a single 3D mesh file.  (PLY file)

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4

struct sMesh
{
	std::string modelFileName;			// File.ply
	std::string uniqueFriendlyName;		// Unique identifier, if want to specify a specific instance
	unsigned int uniqueID;				// Theoretically, this provides a unique ID number for every single thing...

	glm::vec3 positionXYZ;				// Position in 3D space
	glm::vec3 rotationEulerXYZ;			// Rotation in 3D space
	float uniformScale = 1.0f;					// Scale of object

	glm::vec4 objectColourRGBA;						// Colors: Red, Green, Blue, Alpha

	bool bOverrideObjectColour = true;
	bool bIsWireframe = false;
	bool bIsVisible = true;
};