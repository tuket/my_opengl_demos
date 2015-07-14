#include "sdl_and_gl.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

void initGL();

/** create a texture with the given text
 * @param text string to be rendered
 * @param font used font
 * @param color color of the text
 **/
unsigned renderText
(
	const string& text,
	const TTF_Font* font,
	const SDL_Color& color
);

string loadTextFile(string fileName);

int main()
{
	
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	TTF_Font* myFont = TTF_OpenFont("Arial.ttf", 20);
	if( !myFont ) cerr << "error opening font" << endl;
	
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
	// don't allow deprecated GL functions
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	SDL_Window* window =
	SDL_CreateWindow
	(
		"OpenGL TTF",
		50, 50,
		800, 600,
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
	
	// render text to texture
	SDL_Color color = {255, 255, 255, 255};
	unsigned texture = renderText( "HELLO", myFont, color );
	
	SDL_Event event;
	
	bool end = false;
	while( !end )
	{
		
		if( SDL_PollEvent(&event) )
		{
			
			// handle quit events
			if( event.type == SDL_QUIT )
			{
				end = true;
			}
			else if( event.type == SDL_KEYDOWN )
			{
				if( event.key.keysym.sym == SDLK_ESCAPE )
				{
					end = true;
				}
			}
			
			
		}
		
		// draw
		
		glClear( GL_COLOR_BUFFER_BIT );
		
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
		
	}
	
	glDeleteTextures( 1, &texture );
	
	TTF_CloseFont(myFont);
	TTF_Quit();
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	
}

void initGL()
{
	
	glClearColor(0, 0, 0, 1.0);
	
	glEnable( GL_TEXTURE_2D );
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
}

unsigned renderText
(
	const string& text,
	const TTF_Font* font,
	const SDL_Color& color
)
{
	
	SDL_Surface* surface =
	TTF_RenderText_Blended
	(
		const_cast<TTF_Font*>( font ),
		text.c_str(),
		color
	);
	
	surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
	
	if(!surface) cerr << "error rendering text" << endl;
	
	unsigned texture;
	
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
	glTexImage2D
	(
		GL_TEXTURE_2D, 0, GL_RGBA8,
		surface->w, surface->h, 0, GL_RGBA,
		GL_UNSIGNED_INT_8_8_8_8, surface->pixels
	);
	
	SDL_FreeSurface( surface );
	
	return texture;
	
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
