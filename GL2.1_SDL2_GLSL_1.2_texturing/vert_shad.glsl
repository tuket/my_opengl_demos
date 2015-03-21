#version 120

attribute vec2 inPosition;
attribute vec2 inTexCoords;

void main()
{
    gl_Position = vec4(inPosition, 0.0, 1.0);
    gl_TexCoord[0] = vec4(inTexCoords, 0, 0);
}
