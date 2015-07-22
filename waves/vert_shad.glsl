#version 120

/**
 * In the vertex shader you are computing the position
 * of the current vertex.
 * Also you compute the texture coordinate corresponding
 * to this vertex.
 **/

// these are the parameters received by the main program
attribute vec3 inPosition;

uniform mat4 mat;
uniform float time;
varying vec4 gl_FrontColor;

void main()
{
	
	// compute the position of the current vertex
	// In this case you are just forwarding the position,
	// but this allows you to achieve cool effects like the
	// mesh bending or the waves of the ocean
	// The fouth coordinate is called W(1.0) and it is used
	// for normalization purposes( read: 
	// "http://stackoverflow.com/questions/2422750/
	// in-opengl-vertex-shaders-what-is-w-and-why-do-i-divide-by-it" )
	float x = inPosition.x;
	float z = inPosition.z;
	float y = sin(x+time)*sin(z+time)/2.0;
	

	gl_Position = mat * vec4(x, y, z, 1.0);
	gl_FrontColor = vec4(0.1, 0.3, y/4.0 + 0.5, 1);
    
}
