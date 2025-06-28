#version 450

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gAmbient;
layout(location = 3) out vec3 gDiffuse;
layout(location = 4) out vec4 gSpecular;

in vec3 vPosition;
in vec3 vNormal;

uniform vec3 uAmbientColor;
uniform vec3 uDiffuseColor;
uniform vec3 uSpecularColor;
uniform float uShininess;

void main()
{
    gPosition = vPosition;
    gNormal = normalize(vNormal);
    gAmbient = uAmbientColor;
    gDiffuse = uDiffuseColor;
    gSpecular = vec4(uSpecularColor, uShininess);
}
