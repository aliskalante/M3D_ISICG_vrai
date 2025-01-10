#version 450

in vec2 vUV;
out vec4 fragColor;

uniform sampler2D gPositionTex;
uniform sampler2D gNormalTex;
uniform sampler2D gAmbientTex;
uniform sampler2D gDiffuseTex;
uniform sampler2D gSpecularTex;

uniform vec3 uLightDirection = vec3(0.0, 0.0, 1.0);

void main()
{
    vec3 position = texture(gPositionTex, vUV).rgb;
    vec3 normal = normalize(texture(gNormalTex, vUV).rgb);

    vec3 ambient = texture(gAmbientTex, vUV).rgb;
    vec3 diffuse = texture(gDiffuseTex, vUV).rgb;
    vec4 specInfo = texture(gSpecularTex, vUV);
    vec3 specular = specInfo.rgb;
    float shininess = specInfo.a;

    vec3 L = normalize(uLightDirection);
    float ndotl = max(dot(normal, L), 0.0);
    vec3 color = ambient + diffuse * ndotl;

    vec3 V = vec3(0.0, 0.0, 1.0);
    vec3 H = normalize(L + V);
    float ndoth = max(dot(normal, H), 0.0);
    float specFactor = pow(ndoth, shininess);
    color += specular * specFactor;

    fragColor = vec4(color, 1.0);
}
