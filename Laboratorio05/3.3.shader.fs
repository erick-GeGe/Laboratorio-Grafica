#version 330 core
out vec4 FragColor;
uniform vec4 ourColor;
in vec4 position;

void main()
{
   FragColor = vec4(ourColor.x, ourColor.y, ourColor.z, 1.0f);
};