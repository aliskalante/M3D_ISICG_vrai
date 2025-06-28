#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 vertexColor;

uniform mat4 MVP;

void main()
{
    vertexColor = color;
    gl_Position = MVP * vec4(position, 1.0);
}
