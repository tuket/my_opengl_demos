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

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
	
	// take the color of the texture pixel
	vec4 color1 = texture2D(tex1, gl_TexCoord[0].st);
	
	// take another color of the texture pixel
	vec4 color2 = texture2D(tex2, gl_TexCoord[0].st);
	
	// blend both colors
	float tweak = 0.5f;
	gl_FragColor = mix(color1, color2, tweak);
    
}
