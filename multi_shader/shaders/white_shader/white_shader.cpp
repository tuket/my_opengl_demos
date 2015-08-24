#include "white_shader.hpp"
#include <iostream>
#include <string>
#include "../../shader_attrib_locations.hpp"
#include "../compile_shader.hpp"

using namespace std;

bool createWhiteShader(string& name, GLuint& shad)
{
	name = "white_shader";
	shad = glCreateProgram();
	
	glBindAttribLocation
	(
		shad,
		(GLuint)EShaderAttribLocation::position,
		"pos"
	);
	
	bool res = compileShader(shad, name);
	
	return res;
	
}
