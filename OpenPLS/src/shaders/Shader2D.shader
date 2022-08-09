#shader vertex
#version 330 core



layout(location = 0) in vec3 finalPosition;
layout(location = 0) in vec3 finalColor;

out vec4 colorpass;


void main()
{
    colorpass = vec4(color,1);
    gl_Position = finalPosition;

}


#shader fragment
#version 330 core


in vec4 colorpass;
out vec4 color;


void main()
{
    color = colorPass;
}