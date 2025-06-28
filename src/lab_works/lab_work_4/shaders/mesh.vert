#version 450

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;

uniform mat4 MVP;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    FragPos = vec3(ViewMatrix * ModelMatrix * vec4(aVertexPosition, 1.0));
    Normal = normalize(NormalMatrix * aVertexNormal);
    gl_Position = MVP * vec4(aVertexPosition, 1.0);
}
