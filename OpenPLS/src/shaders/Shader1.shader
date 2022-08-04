#shader vertex
#version 330 core



layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
out vec4 colorpass;

uniform mat4 M, V, P;

void main()
{
	colorpass = vec4(color, 1);
	vec4 temp = vec4(position, 1.0) * M * V * P;
	temp.z = temp.z - 0.01f;
	gl_Position = temp;
}




#shader fragment
#version 330 core


in vec4 colorpass;
out vec4 color;


void main()
{
	color = colorpass;
}