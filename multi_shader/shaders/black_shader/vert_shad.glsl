#version 120

attribute vec3 pos;

void main()
{
	float x = pos.x;
	float y = pos.y;
	float z = pos.z;
	gl_Position = vec4(x, y, z, 1.0);
}
