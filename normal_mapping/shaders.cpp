#include "shaders.hpp"
#include "util.hpp"
#include <string>
#include <iostream>
#include <cassert>

#include "shaders/white_shader/normal_map_shader.hpp"

//#define STOP_ON_COMPILE_ERROR

using namespace std;

Shaders* Shaders::getSingleton()
{
	static Shaders* instance = new Shaders();
	return instance;
}

/**
 * return true if success
 **/
bool Shaders::init()
{
	
	GLuint shad;
	string name;
	bool res = true;
	bool b;
	
	for
	(
		int i=0;
		i<sizeof(shaderCreators) / sizeof(bool (*)(GLuint&));
		i++
	)
	{
		b = shaderCreators[i](name, shad);
		
		#ifndef NDEBUG
		if( shaders.find(name) != shaders.end() )
		{
			cerr << "Warning: The is already a shader with name: "
			<< name << endl;
		}
		#endif
		
		shaders[name] = shad;
		#ifdef STOP_ON_COMPILE_ERROR
		if(!b) return false;
		#endif
		res &= b;
	}
	
	return res;
	
}

const GLuint Shaders::getShader(const string& name)const
{
	map<string, GLuint>::const_iterator it = shaders.find(name);
	assert( it != shaders.end() );
	return it->second;
}

bool (*(Shaders::shaderCreators[2])) (string&, GLuint&) =
{
	createNormalMapShader,
};
