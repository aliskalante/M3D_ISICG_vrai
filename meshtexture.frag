#version 450

layout(location = 0) out vec4 fragColor;

uniform vec3 Camerapos;

uniform vec3 ambient_color;
uniform vec3 diffuse_color;
uniform vec3 specular_color;
uniform float shininess;

uniform bool uHasDiffuseMap;
layout(binding = 1) uniform sampler2D uDiffuseMap;

uniform bool uHasAmbientMap;
layout(binding = 2) uniform sampler2D uAmbientMap;

uniform bool uHasSpecularMap;
layout(binding = 3) uniform sampler2D uSpecularMap;

uniform bool uHasShininessMap;
layout(binding = 4) uniform sampler2D uShininessMap;

uniform bool uHasNormalMap;
layout(binding = 5) uniform sampler2D uNormalMap;

in vec3 LightPos;
in vec2 textureCoords;
in vec3 Normal;
in vec3 FragPos;

void main()
{
    vec3 normal = normalize(Normal);

    if (uHasNormalMap)
    {
        normal = texture(uNormalMap, textureCoords).xyz;
        normal = normalize(normal * 2.0 - 1.0);
    }

    vec3 viewDir = normalize(Camerapos - FragPos);
    vec3 lightDir = normalize(LightPos - FragPos);

    if (texture(uDiffuseMap, textureCoords).a < 0.5f)
        discard;

    vec3 ambient = ambient_color;
    if (uHasAmbientMap)
    {
        ambient = vec3(texture(uAmbientMap, textureCoords));
    }

    vec3 diffuse;
    if (uHasDiffuseMap)
    {
        diffuse = vec3(texture(uDiffuseMap, textureCoords)) * max(dot(normal, lightDir), 0.0);
    }
    else
    {
        diffuse = diffuse_color * max(dot(normal, lightDir), 0.0);
    }

    float shininessValue = shininess;
    if (uHasShininessMap)
    {
        shininessValue = texture(uShininessMap, textureCoords).x;
    }

    vec3 specular;
    if (uHasSpecularMap)
    {
        specular = vec3(texture(uSpecularMap, textureCoords)).xxx * pow(max(dot(normal, normalize(lightDir + viewDir)), 0.0), shininessValue);
    }
    else
    {
        specular = specular_color * pow(max(dot(normal, normalize(lightDir + viewDir)), 0.0), shininessValue);
    }

    vec3 finalColor = ambient + diffuse + specular;
    fragColor = vec4(finalColor, texture(uDiffuseMap, textureCoords).a);
}
