# version 130
// Input vertex attributes
in vec4 vPosition;
in vec4 vColors ;
// Output vertex attributes to be interpolated
// for each fragment during rasterization
out vec4 v2fcolor ;

uniform float width;
uniform float height;
uniform vec4 translation;

void main ()
{
// gl_Position is a mandatory built -in output variable
gl_Position = vPosition * vec4(width, height, 1, 1) + translation;
v2fcolor = vColors ;
}
