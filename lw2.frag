#version 450

layout(location = 0) out vec4 fragColor;
in vec3 colorf;
uniform float c_luminosite;

void main()
{
    vec3 adjustedColor = colorf * c_luminosite; // Adjust color by luminosity
    fragColor = vec4(adjustedColor, 1.0);      // Set fragment color
}
