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

void main()
{
	
	// compute the position of the current vertex
	// In this case you are just forwarding the position,
	// but this allows you to achieva cool effects like the
	// mesh bending or the waves of the ocean
	// The fouth coordinate is called W(1.0) and it is used
	// for normalization purposes( read: 
	// "http://stackoverflow.com/questions/2422750/
	// in-opengl-vertex-shaders-what-is-w-and-why-do-i-divide-by-it" )
	gl_Position = mat * vec4(inPosition, 1.0);
    
}
