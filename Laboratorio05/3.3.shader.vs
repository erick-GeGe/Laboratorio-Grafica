#version 330 core
layout (location = 0) in vec3 aPos;
out vec4 position;
uniform mat4 transform;
 
void main()
{
	gl_Position = transform * vec4(aPos, 1.0f);
};