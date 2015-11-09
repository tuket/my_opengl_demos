#version 120

uniform sampler2D normalMap;

void main()
{
	
	vec4 col = texture2D(normalMap, gl_texCoord[0].st);
	vec3 nVec = col.xyz;
	vec3 uni = vec3(0.0, 0.0, 1.0);
	float fc = dot(nVec, uni);
	gl_FragColor = fc;
	
}
