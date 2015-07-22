#include "resolution_plane.hpp"
#include <iostream>
#include "util.hpp"
#include <cstdlib>
#include <cassert>
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
const float PI = 3.14159265359f;

using namespace std;

GLuint ResolutionPlane::shaderProgram = 0;

ResolutionPlane::ResolutionPlane
(
	float w, float h,
	unsigned rw, unsigned rh
)
{
	
	if( rw < 1 || rh < 1)
	{
		cerr << "Error: not valid resolution for plane" << endl;
		error = true;
	}
	else
	{
		
		error = false;
		
		this->rw = rw;
		this->rh = rh;
		this->w = w;
		this->h = h;
		
		float iw = w / rw;
		float ih = h / rh;
		
		verticesCount = (rw+1) * (rh+1);
		trianglesCount = 2 * (rw*rh);
		indicesCount = trianglesCount*3;
		
		vertices = new GLfloat[verticesCount*3];
		indices = new GLuint[indicesCount];
		
		for(int y=0; y<=rh; y++)
		for(int x=0; x<=rw; x++)
		{
			assert(3*(x + y*(rw+1)) + 2 < verticesCount*3);
			vertices[3*(x + y*(rw+1)) + 0] = x * iw - w/2;
			vertices[3*(x + y*(rw+1)) + 1] = 0;
			vertices[3*(x + y*(rw+1)) + 2] = y * ih - h/2;
		}
		
		int ind = 0;
		for (int row = 0; row < rh; row++)
		{
			int p0 = row*(rw + 1);
			int p1 = (row + 1)*(rw + 1);
			int p2 = p1 + 1;
			for (int i = 0; i < rw; i++)
			{
				indices[ind++] = p0;
				indices[ind++] = p1;
				indices[ind++] = p2;
				p0++; p1++; p2++;
			}

			p0 = row*(rw + 1);
			p1 = (row + 1)*(rw + 1) + 1;
			p2 = p0 + 1;
			for (int i = 0; i < rw; i++)
			{
				indices[ind++] = p0;
				indices[ind++] = p1;
				indices[ind++] = p2;
				p0++; p1++; p2++;
			}
		}

		initShaders();
		glUseProgram(shaderProgram);
		
		// VAO
		// ---
		// VAOs are used to save "settings"
		// VAOs store all the links between the attributes and
		// your VBO with raw vertex data.
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		
		glUseProgram(shaderProgram);
		
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData
		(
			GL_ARRAY_BUFFER,
			3*verticesCount*sizeof(GLfloat), vertices,
			GL_STATIC_DRAW
		);
		
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData
		(
			GL_ELEMENT_ARRAY_BUFFER,
			indicesCount*sizeof(GLuint), indices,
			GL_STATIC_DRAW
		);
		
		// making the link between vertex data and attributes
		// OpenGL needs to know how the attributes are formatted
		// and ordered
		
		// POSITION ATTRIBUTE
		GLint posAttrib = glGetAttribLocation(shaderProgram, "inPosition");
		
		// specify how the data is structured in the array
		glVertexAttribPointer
		(
			posAttrib,			// input
			3,					// num of vals
			GL_FLOAT,			// type of vals
			GL_FALSE,			// should they be normalized?
			3*sizeof(GLfloat),	// stride: how many bytes between each
			0					// offset: offset of the attribute wrt first
		);
		
		// enable vertex attribute array
		glEnableVertexAttribArray(posAttrib);
		
		// uniforms
		
		float fov = (45.0f / 180.0f)*PI;
		glm::mat4 proj = glm::perspective(fov, 4.0f/3.0f, 0.1f, 100.0f);
		glm::mat4 view =
			glm::lookAt
			(
			glm::vec3(0, 40, 50),
			glm::vec3(0, 0, 0),
			glm::vec3(0, 1, 0)
			);

		glm::mat4 M = proj*view;

		GLint matLoc = glGetUniformLocation( shaderProgram, "mat" );
		glUniformMatrix4fv
		(
			matLoc,
			1,
			GL_FALSE,
			&M[0][0]
		);
		
	}
	
}

void ResolutionPlane::draw(float time)
{
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUseProgram( shaderProgram );
	GLint loc = glGetUniformLocation( shaderProgram, "time" );
	glUniform1f( loc, time );
	cout << time << endl;
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	
	glDrawElements
	(
		GL_TRIANGLES,		// primitive to draw
		indicesCount,		// how many indices to draw
		GL_UNSIGNED_INT,	// type of the indices
		0
	);
	
}



void ResolutionPlane::initShaders()
{
	static bool firstTime = true;
	if( firstTime )
	{
		firstTime = false;
		
		string vertSource = loadTextFile("vert_shad.glsl");
		const char* vSource = vertSource.c_str();
		string fragSource = loadTextFile("frag_shad.glsl");
		const char* fSource = fragSource.c_str();
		
		// compiling shaders
		// compile vertex shader
		GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource
		(
			vertShader,			// shader id
			1,					// number of strings
			&vSource,			// array of strings
			0					// length of each string(0 means stop at null char)
		);
		glCompileShader(vertShader);
		
		// check if the shader compiled OK
		GLint status;
		glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
		if( status != GL_TRUE )
		{
			cerr << "Error: compiling vertex shader" << endl;
			exit(1);
		}
		
		// compile fragment shader
		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource
		(
			fragShader,
			1,
			&fSource,
			0
		);
		glCompileShader(fragShader);
		
		// check if the shader compiled OK
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
		if( status != GL_TRUE )
		{
			cerr << "Error: compiling fragment shader" << endl;
			exit(1);
		}
		
		
		// combining shaders into a program
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertShader);
		glAttachShader(shaderProgram, fragShader);
		
		// Since fragment shader is allowed to write to multiple buffers,
		// you need to specify which output is written to which buffer.
		// You must do this before linking.
		// However, since 0 is the default and there is only one output
		// the following line is not necessary.
		/// glBindFragDataLocation(shaderProgram, 0, "outColor");
		
		// We must connect the two shaders.
		// We do this with the "linking" step.
		glLinkProgram(shaderProgram);
		
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
		if(!status) {
			cerr << "error linking shader" << endl;
		}
		
	}
}
