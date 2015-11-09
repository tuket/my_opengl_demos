

#ifndef SHADER_UNIFORM_LOCATIONS_HPP
#define SHADER_UNIFORM_LOCATIONS_HPP

enum class EShaderUniformLocation
{
	matrix = 1,			// matrix
	tex0 = matrix + 3	// float
	tex1,				// float
	normal_tex,			// float
	point_light_pos0,		// vec3
	point_light_color0,		// vec3
	point_light_pos1,				// vec3
	point_light_color1,		// vec3
};

#endif
