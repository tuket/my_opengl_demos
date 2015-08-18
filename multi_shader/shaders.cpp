#include "shaders.hpp"
#include "util.hpp"
#include <string>
#include <iostream>

using namespace std;

static vector<GLuint> shaders;

/**
 * return true if success
 **/
bool initShaders()
{
	
	GLuint shad;
	GLuint vertShad;
	GLuint fragShad;
	string vertSrc;
	string fragSrc;
	
	const char* fileName;
	GLint status;
	static char bufferMsg[1024];
	
	bool res = true;
	
	/**
	 * WHITE SHADER
	 **/
	shad = glCreateProgram();
		
		fileName = "vert_shad_1.glsl";
		vertShad = glCreateShader(GL_VERTEX_SHADER);
		vertSrc = loadTextFile(fileName);
		glShaderSource(vertShad, 1, vertSrc.c_str(), 0);
		glCompileShader(vertShad);
		glGetShaderiv(vertShad, GL_COMPILE_STATUS, &status);
		if(status != GL_TRUE)
		{
			cerr << "Error compiling vertex shader: " << fileName << ": " << endl;
			glGetShaderInfoLog(vertShad, 1024, 0, bufferMsg);
			cerr << bufferMsg << endl;
			res = false;
		}
		
		fragShad = glCreateShader(GL_FRAGMENT_SHADER);		
		fragSrc = loadTextFile("frag_shad_1.glsl");
		glShaderSource(fragShad, 1, fragSrc.c_str(), 0);
		glCompileShader(fragShad);
		if(status != GL_TRUE)
		{
			cerr << "Error compiling fragment shader: " << fileName << ": " << endl;
			glGetShaderInfoLog(fragsShad, 1024, 0, bufferMsg);
			cerr << bufferMsg << endl;
			res = false;
		}
		
		glAttachShader(shad, vertShad);
		glAttachShader(shad, fragShad);
		glLinkProgram();
		glGetProgramiv(shad, GL_LINK_STATUS, &status);
		if(status != GL_TRUE)
		{
			cerr << "Error linking program: " << "White Shader" << ": " << endl;
			glGetShaderInfoLog(fragsShad, 1024, 0, bufferMsg);
			cerr << bufferMsg << endl;
			res = false;
		}
		
	shaders.push_back(shad);
	
	
	
	return res;
}

const vector<GLuint>& getShaders()
{
	return shaders;
}
