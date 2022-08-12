#shader vertex
#version 330 core



layout(location = 0) in vec3 finalPosition;
layout(location = 1) in vec3 finalColor;

out vec4 colorpass;


void main()
{
    colorpass = vec4(finalColor,1);
    gl_Position = vec4(finalPosition,1);

}


#shader fragment
#version 330 core


in vec4 colorpass;
out vec4 color;


void main()
{
    color = colorpass;
}