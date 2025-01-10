#version 450 
layout (location = 0) in vec3 aVertexPosition; 
layout (location =1) in vec3 color;
out vec3 colorf;
uniform float uTranslationX;
void main() 
{
colorf=color;
//gl_Position=vec4(aVertexPosition,1.f);// La sortie 
gl_Position=vec4(aVertexPosition.x+uTranslationX,aVertexPosition.y,aVertexPosition.z,1.f); 
																							
}