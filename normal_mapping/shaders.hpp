#include "sdl_and_gl.hpp"
#include <vector>
#include <string>
#include <map>

#ifndef SHADERS_HPP
#define SHADERS_HPP

class Shaders
{
	
	std::map<std::string, GLuint> shaders;
	
	Shaders(){}
	
public:
	
	static Shaders* getSingleton();
	
	bool init();
	const GLuint getShader(const std::string& name)const;
	
private:
	
	
	static bool (*(shaderCreators[2])) (std::string&, GLuint&);
	
};

#endif
