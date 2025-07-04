#version 450

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTexCoords;
layout(location = 3) in vec3 aVertexTangent;
layout(location = 4) in vec3 aVertexBitagent;

uniform mat4 MVPMatrix; 
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

uniform vec3 lightPos;

out vec3 LightPos;
out vec3 FragPos;
out vec3 Normal; 
out vec2 textureCoords;

void main()
{
    textureCoords = aVertexTexCoords;

    Normal = normalize(mat3(NormalMatrix) * aVertexNormal);
    vec3 T = normalize(vec3(ModelMatrix * vec4(aVertexTangent, 0.0)));
    vec3 B = normalize(vec3(ModelMatrix * vec4(aVertexBitagent, 0.0)));
    vec3 N = normalize(vec3(ModelMatrix * vec4(aVertexNormal, 0.0)));

    T = normalize(T - dot(T, N) * N); // Orthogonalize T with respect to N
    B = cross(N, T);

    mat3 TBN = mat3(T, B, N);
    mat3 invTBN = transpose(TBN);

    LightPos = vec3(ViewMatrix * vec4(lightPos, 1.0)) * invTBN;
    FragPos = vec3(ViewMatrix * ModelMatrix * vec4(aVertexPosition, 1.0)) * invTBN;
    gl_Position = MVPMatrix * vec4(aVertexPosition, 1.0);
}
