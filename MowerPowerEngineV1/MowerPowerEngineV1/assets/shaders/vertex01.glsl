#version 330

uniform mat4 MVP;

in vec3 vCol;
in vec3 vPos;

out vec3 color;

void main()
{
	vec3 finalVert = vPos;	
	
	gl_Position = MVP * vec4(finalVert, 1.0);
	
	color = vCol;
}
