#version 450

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;

out vec3 vPosition;
out vec3 vNormal;

uniform mat4 uMVPMatrix;
uniform mat4 uModelMatrix;

void main()
{
    gl_Position = uMVPMatrix * vec4(aVertexPosition, 1.0);
    vPosition = vec3(uModelMatrix * vec4(aVertexPosition, 1.0));
    vNormal = mat3(transpose(inverse(uModelMatrix))) * aVertexNormal;
}
