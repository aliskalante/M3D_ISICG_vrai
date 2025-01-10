#version 450

layout(location = 0) out vec4 fragColor;
in vec3 vertexColor;

void main()
{
    fragColor = vec4(vertexColor, 1.0);
}
