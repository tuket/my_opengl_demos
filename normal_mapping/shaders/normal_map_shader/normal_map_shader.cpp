#include "normal_map_shader.hpp"

bool createNormalMapShader(std::string& name, GLuint shad)
{
	
	name = "normal_map_shader";
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
