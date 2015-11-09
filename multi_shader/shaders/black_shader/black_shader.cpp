#include "black_shader.hpp"
#include <iostream>
#include <string>
#include "../../shader_attrib_locations.hpp"
#include "../compile_shader.hpp"

using namespace std;

bool createBlackShader(string& name, GLuint& shad)
{
	name = "black_shader";
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
