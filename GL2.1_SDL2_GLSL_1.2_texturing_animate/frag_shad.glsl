#version 120

uniform sampler2D tex1;
uniform sampler2D tex2;

uniform int time;

void main()
{
	vec4 color1 = texture2D(tex1, gl_TexCoord[0].st);
	vec4 color2 = texture2D(tex2, gl_TexCoord[0].st);
    gl_FragColor = mix(color1, color2, mod(time, 1000)/1000.0);
}
