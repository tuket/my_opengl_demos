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
uniform float time;

const float PI = 3.14159265359;

void main()
{
	
	// take the color of the texture pixel
	vec4 color1 = texture2D(tex1, gl_TexCoord[0].st);
	
	// take another color of the texture pixel
	vec2 oriTex = gl_TexCoord[0].st;
	float nt = mod(time, 2*PI) / (2*PI);
	vec4 color2 = texture2D( tex2, vec2(oriTex.s+0.01*sin((1+nt)*oriTex.t*PI*4), oriTex.t) );
	
	// blend both colors
	float tweak = 1.0f;
	gl_FragColor = mix(color1, color2, tweak);
    
}
