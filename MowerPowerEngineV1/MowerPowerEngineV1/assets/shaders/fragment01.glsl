#version 330
// Pixel fragment
in vec3 color;

uniform vec4 objectColour;
uniform bool bUseObjectColour;

out vec4 fragment;

void main()
{
	vec3 finalColour = color;
	if( bUseObjectColou r)
	{
		vec3 finalColour = objectColour.rgb;
	}

	fragment = vec4(finalColour, 1.0);
}
