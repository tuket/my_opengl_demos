#include "compile_shader.hpp"
#include <iostream>
#include "../util.hpp"

using namespace std;

bool compileShader(GLuint& shad, const string& name)
{
	
	const string vertShadFileName = "shaders/" + name + "/vert_shad.glsl";
	const string fragShadFileName = "shaders/" + name + "/frag_shad.glsl";
	
	GLuint vertShad;
	GLuint fragShad;
	string vertSrc;
	string fragSrc;
	
	GLint status;
	static char bufferMsg[1024];
	
	bool res = true;
	
	vertShad = glCreateShader(GL_VERTEX_SHADER);
	vertSrc = loadTextFile(vertShadFileName);
	const char* pVertSrc[1] = { vertSrc.c_str() };
	glShaderSource(vertShad, 1, pVertSrc, 0);
	glCompileShader(vertShad);
	glGetShaderiv(vertShad, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE)
	{
		cerr << "Error compiling vertex shader: " << name << ": " << endl;
		glGetShaderInfoLog(vertShad, 1024, 0, bufferMsg);
		cerr << bufferMsg << endl;
		res = false;
	}
	
	fragShad = glCreateShader(GL_FRAGMENT_SHADER);		
	fragSrc = loadTextFile(fragShadFileName);
	const char* pFragSrc[1] = { fragSrc.c_str() };
	glShaderSource(fragShad, 1, pFragSrc, 0);
	glCompileShader(fragShad);
	if(status != GL_TRUE)
	{
		cerr << "Error compiling fragment shader: " << name << ": " << endl;
		glGetShaderInfoLog(fragShad, 1024, 0, bufferMsg);
		cerr << bufferMsg << endl;
		res = false;
	}
	
	glAttachShader(shad, vertShad);
	glAttachShader(shad, fragShad);
	glLinkProgram(shad);
	glGetProgramiv(shad, GL_LINK_STATUS, &status);
	if(status != GL_TRUE)
	{
		cerr << "Error linking program: " << "White Shader" << ": " << endl;
		glGetProgramInfoLog(shad, 1024, 0, bufferMsg);
		cerr << bufferMsg << endl;
		res = false;
	}
	#ifndef NDEBUG
	else log_stream << "White shader ... OK" << endl;
	#endif
	
}
