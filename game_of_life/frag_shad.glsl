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

uniform sampler2D tex;

void main()
{
	
	// take the color of the texture pixel
	vec4 color1 = texture2D(tex, gl_TexCoord[0].st);
	
	gl_FragColor = color1;
    
}
