#version 450

layout(location = 0) out vec4 fragColor;

uniform vec3 Camerapos;
uniform vec3 lightPos;
uniform vec3 ambient_color;
uniform vec3 diffuse_color;
uniform vec3 specular_color;
uniform float shininess;

in vec3 FragPos;
in vec3 Normal;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(Camerapos - FragPos);

    // Ambient component
    vec3 ambient = ambient_color;

    // Diffuse component
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuse_color;

    // Specular component
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * specular_color;

    // Final color
    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0);
}
