#version 120

attribute vec3 pos;
attribute vec2 texCoord;

void main()
{
	float x = pos.x;
	float y = pos.y;
	float z = pos.z;
	gl_Position = vec4( x, y, z, 1.0 );
	
	gl_TexCoord[0] = vec4( texCoord, 0.0, 0.0 );
}
