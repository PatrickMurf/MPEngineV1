#version 330
// Pixel fragment
in vec3 color;

out vec4 fragment;

void main()
{
	vec3 finalColour = color;
	finalColour *= 0.001f;
	finalColour.g = 1.0f;
	fragment = vec4(finalColour, 1.0);
}
