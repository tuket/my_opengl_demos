#define NO_SDL_GLEXT
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <SOIL/SOIL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// initialize OpenGL
void initGL();

// load a text file and return it as a string
string loadTextFile(string fileName);

// load a texture to video memory
void loadTexture(string fileName, int* width, int* height);

// handle event
void handleEvent(SDL_Event event);

bool gameover;

int main()
{
	
	SDL_Init(SDL_INIT_EVERYTHING);	
	
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
	// don't allow deprecated GL functions
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	
	// make the window
	SDL_Window* window =
	SDL_CreateWindow(
		"OpenGL",
		100, 100,
		800, 800,
		SDL_WINDOW_OPENGL
	);
	
	SDL_GLContext context = SDL_GL_CreateContext(window);
	
	// init glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  cerr << "Error: " << glewGetErrorString(err) << endl;
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	
	initGL();
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	// vertices
	// format: x, y, texture_x, texture_y
	const GLfloat vertices[] =
	{
		-0.5, 0.5, 0, 0,
		0.5, 0.5, 1, 0,
		0.5, -0.5, 1, 1,
		-0.5, -0.5, 0, 1
	};
	
	// create one vertex buffer object
	GLuint VBO;
	glGenBuffers(1, &VBO);
	
	// make VBO the current active vertex array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	// upload the vertex data to the video card
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	
	
	// elements (one triangle per line)
	GLuint elements[] =
	{
		0, 1, 3,
		1, 2, 3
	};
	
	// create one element buffer object
	GLuint EBO;
	glGenBuffers(1, &EBO);
	
	// make EBO the current active element array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	
	// upload the element data to the video card
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements)*sizeof(GLuint), elements, GL_STATIC_DRAW);
	
	
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
	GLuint shaderProgram = glCreateProgram();
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
	
	// use the program
	glUseProgram(shaderProgram);
	
	
	// VAO
	// ---
	// VAOs are used to save "settings"
	// VAOs store all the links between the attributes and
	// your VBO with raw vertex data.
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	
	// making the link between vertex data and attributes
	// OpenGL needs to know how the attributes are formatted
	// and ordered
	
	// POSITION ATTRIBUTE
	GLint posAttrib = glGetAttribLocation(shaderProgram, "inPosition");
	
	// specify how the data is structured in the array
	glVertexAttribPointer
	(
		posAttrib,			// input
		2,					// num of vals
		GL_FLOAT,			// type of vals
		GL_FALSE,			// should they be normalized?
		4*sizeof(float),	// stride: how many bytes between each pos
		0					// offset: offset of the attribute wrt pos
	);
	
	// enable vertex attribute array
	glEnableVertexAttribArray(posAttrib);
	
	// TEXCOORD ATTRIBUTE
	GLint texAttrib = glGetAttribLocation(shaderProgram, "inTexCoords");
	
	// specify how the data is structured in the array
	glVertexAttribPointer
	(
		texAttrib,			// input
		2,					// num of vals
		GL_FLOAT,			// type of vals
		GL_FALSE,			// should they be normalized?
		4*sizeof(float),			// stride: how many bytes between each pos
		(void*)(2*sizeof(float))	// offset: offset of the attribute wrt pos
	);
	
	// enable vertex attribute array
	glEnableVertexAttribArray(texAttrib);
	
	// TEXTURE
	glEnable(GL_TEXTURE_2D);
	
	// texture 1
	glActiveTexture(GL_TEXTURE0);
	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	int width1, height1;
	loadTexture("b.png", &width1, &height1);
	
	glUniform1i(glGetUniformLocation(shaderProgram, "tex1"), 0);
	
	// texture 2
	glActiveTexture(GL_TEXTURE1);
	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	int width2, height2;
	loadTexture("e.png", &width2, &height2);
	
	glUniform1i(glGetUniformLocation(shaderProgram, "tex2"), 1);
	
	gameover = false;
	while(!gameover)
	{	
		unsigned int beginTicks = SDL_GetTicks();
		
		SDL_Event event;
		// handle events
		if (SDL_PollEvent(&event)) {
			handleEvent(event);
		}
		
		// Drawing!
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawElements
		(
			GL_TRIANGLES,		// primitive to draw
			6,					// how many vertices to draw
			GL_UNSIGNED_INT,	// type of the indices
			0
		);
		
		SDL_GL_SwapWindow(window);
		
		unsigned int endTicks = SDL_GetTicks();
		int sleepTicks = 1000/60 - (endTicks-beginTicks);
		if(sleepTicks > 0)
			SDL_Delay( sleepTicks );
		
	}
	
	SDL_GL_DeleteContext(context);
	
}




void initGL()
{
	
	glClearColor(0.5, 0, 0, 0);
	
}

string loadTextFile(string fileName)
{
	
	stringstream ss;
	
	fstream fs;
	fs.open(fileName.c_str(), fstream::in);
	
	if(fs.fail())
	{
		cerr << "Error: could not open " << fileName << endl;
		return string();
	}
	
	ss << fs.rdbuf();
	
	return ss.str();
	
}

void handleEvent(SDL_Event event)
{
	
	switch (event.type) {
		/* close button clicked */
		case SDL_QUIT:
			gameover = true;
			break;
			
		/* handle the keyboard */
		case SDL_KEYDOWN:
		
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
				case SDLK_q:
					gameover = true;
					break;
			}
			break;
	}
	
}

void loadTexture(string fileName, int* width, int* height)
{
	
	unsigned char* image =
	SOIL_load_image
	(
		fileName.c_str(),	// file name
		width,				// pointer where width will be saved
		height,				// pointer where height will be saved
		0,					// pointer where num of chanel will be saved
		SOIL_LOAD_RGB		// RGB
	);
	
	// specify a 2-dimensional texture image
	glTexImage2D
	(
		GL_TEXTURE_2D,		// target
		0,					// level of detail (0 is base level)
		GL_RGB,				// internal format
		*width,				// width
		*height,			// height
		0,					// border (must be 0)
		GL_RGB,				// format of the pixel data
		GL_UNSIGNED_BYTE,	// data type of the pixel data
		image				// pointer to iamge data
	);
	
	SOIL_free_image_data(image);
	
}
