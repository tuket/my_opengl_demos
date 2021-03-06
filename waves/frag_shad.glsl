#version 120

/**
 * In the fragment shader you are computing
 * which should be the color of the current pixel
 * ( fragment and pixel are sinonyms in OpenGL,
 * in fact in DirectX it is called pixel shader )
 * 
 * The output goes to -> gl_FragColor
 * 
 **/

uniform float time;

const float PI = 3.14159265;

void main()
{
	
	gl_FragColor = gl_Color;
    
}
