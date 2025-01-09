#version 330

uniform mat4 MVP;

in vec3 vCol;
in vec3 vPos;

out vec3 color;

void main()
{
	vec3 finalVert = vPos;
	
	finalVert.x = vPos.x * 1.0f;
	finalVert.y = vPos.y * 1.0f;
	finalVert.z = vPos.z * 1.0f;
	
	gl_Position = MVP * vec4(finalVert, 1.0);
	
	color = vCol;
}
