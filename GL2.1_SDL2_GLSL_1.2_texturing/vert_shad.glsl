#version 120

/**
 * In the vertex shader you are computing the position
 * of the current vertex.
 * Also you compute the texture coordinate corresponding
 * to this vertex.
 **/

// these are the parameters received by the main program
attribute vec2 inPosition;
attribute vec2 inTexCoords;

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
    gl_Position = vec4(inPosition, 0.0, 1.0);
    
    // compute the texture coordina corresponding to this
    // vertex
    gl_TexCoord[0] = vec4(inTexCoords, 0, 0);
    
}
