#include "sdl_and_gl.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <glm/glm.hpp>
#include "grid.hpp"
#include "util.hpp"

using namespace std;

// window dimwnsions
const unsigned SCREEN_WIDTH = 800;
const unsigned SCREEN_HEIGHT = 600;

// grid dimnsions
const unsigned GRID_WIDTH = 100;
const unsigned GRID_HEIGHT = 75;

const float PI = 3.14159265359f;
const unsigned STEP_PERIOD = 1000;

void initGL();
void handleEvent(SDL_Event event);
void applyGameOfLifeStep(Grid& dest, Grid& orig);

static bool gameover;
static bool pause = true;

int main()
{
	
	SDL_Init( SDL_INIT_EVERYTHING );
	
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	
	SDL_Window* window =
	SDL_CreateWindow
	(
		"Such waves",
		50, 50,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_OPENGL
	);
	
	SDL_GLContext context = SDL_GL_CreateContext(window);
	
	// init glew
	GLenum err = glewInit();
	if( GLEW_OK != err )
	{
		cerr << "Error: " << glewGetErrorString(err) << endl;
		exit(1);
	}
	cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;
	
	initGL();
	
	// greate 2 grids
	Grid grid(GRID_WIDTH, GRID_HEIGHT);
	Grid grid2(GRID_WIDTH, GRID_HEIGHT);
	
	// enabling texture
	glActiveTexture(GL_TEXTURE0);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// we want our texture to be pixelated
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	const unsigned textureWidth = grid.getTextureWidth();
	const unsigned width = grid.getWidth();
	const unsigned textureHeight = grid.getTextureHeight();
	const unsigned height = grid.getHeight();
	GLubyte* textureData = grid.getTextureData();
	
	// load texture to gpu
	glTexImage2D
	(
		GL_TEXTURE_2D,
		0,	// level
		GL_RGB,
		textureWidth,
		textureHeight,
		0,	// border
		GL_RGB,
		GL_UNSIGNED_BYTE,
		textureData
	);
	
	// vertices
	const GLfloat vertices[] =
	{
		-1, -1, 0, 0,
		+1, -1, (float)width/textureWidth, 0,
		+1, +1, (float)width/textureWidth, (float)height/textureHeight,
		-1, +1, 0, (float)height/textureHeight
	};
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData
	(
		GL_ARRAY_BUFFER,
		sizeof(vertices),
		vertices,
		GL_STATIC_DRAW
	);
	
	// indices
	const GLuint indices[] =
	{
		0, 2, 3,
		0, 1, 2
	};
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(indices),
		indices,
		GL_STATIC_DRAW
	);
	
	// shaders
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
	
	// We must connect the two shaders.
	// We do this with the "linking" step.
	glLinkProgram(shaderProgram);
	
	// use the program
	glUseProgram(shaderProgram);
	
	// VAO
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	
	// bind attributes
	GLint posAttrib = glGetAttribLocation(shaderProgram, "inPosition");
	glVertexAttribPointer
	(
		posAttrib,			// input
		2,					// num of vals
		GL_FLOAT,			// type of vals
		GL_FALSE,			// should they be normalized?
		4*sizeof(GLfloat),	// stride: how many bytes between each pos
		0					// offset: offset of the attribute wrt pos
	);
	glEnableVertexAttribArray(posAttrib);
	
	GLint texAttrib = glGetAttribLocation(shaderProgram, "inTexCoords");
	glVertexAttribPointer
	(
		texAttrib,			// input
		2,					// num of vals
		GL_FLOAT,			// type of vals
		GL_FALSE,			// should they be normalized?
		4*sizeof(float),			// stride: how many bytes between each pos
		(void*)(2*sizeof(GLfloat))	// offset: offset of the attribute wrt pos
	);
	glEnableVertexAttribArray(texAttrib);
	
	glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);
	
	Grid* currentGrid = &grid;
	Grid* backGrid = &grid2;
	gameover = false;
	int mouseX, mouseY;
	bool mouseWasPressed = false;
	unsigned ticks = SDL_GetTicks();
	while(!gameover)
	{	
		bool needToRefresh = false;
		unsigned int beginTicks = SDL_GetTicks();
		
		SDL_Event event;
		// handle events
		if (SDL_PollEvent(&event)) {
			handleEvent(event);
		}
		
		Uint32 mouseStat = SDL_GetMouseState(&mouseX, &mouseY);
		if( mouseStat & SDL_BUTTON(SDL_BUTTON_LEFT) )
		{
			cout << "pressed" << endl;
			if( !mouseWasPressed )
			{
				mouseWasPressed = true;
				float px = (float)mouseX/SCREEN_WIDTH;
				float py = (float)mouseY/SCREEN_HEIGHT;
				int x = px*GRID_WIDTH;
				int y = py*GRID_HEIGHT;
				y = GRID_HEIGHT-y-1;	// (SDL has coords on top left)
				currentGrid->set(x, y, !grid(x, y));
				cout << "x: " << x << endl;
				cout << "y: " << y << endl;
				
				needToRefresh = true;
			}
		}
		else
		{
			mouseWasPressed = false;
		}
		
		if( !pause )
		if( beginTicks - ticks > STEP_PERIOD )
		{
			ticks = beginTicks;
			needToRefresh = true;
			
			applyGameOfLifeStep(*backGrid, *currentGrid);
			Grid* aux = backGrid;
			backGrid = currentGrid;
			currentGrid = aux;
		}
		
		// update texture
		if( needToRefresh )
		{
			glTexSubImage2D
			(
				GL_TEXTURE_2D,
				0,			// level
				0,			// x offset
				0,			// y offset
				textureWidth,
				textureHeight,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				currentGrid->getTextureData()
			);
			needToRefresh = false;
		}
		
		// Drawing!
		glClear(GL_COLOR_BUFFER_BIT);
		
		glBindVertexArray(VAO);
		glUseProgram(shaderProgram);
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
					
				case SDLK_RETURN:
					pause = !pause;
					break;
			}
			break;
	}
	
}

void initGL()
{
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
}

void applyGameOfLifeStep(Grid& dest, Grid& orig)
{
	const unsigned width = dest.getWidth();
	const unsigned height = dest.getHeight();
	for(int y=0; y<height; y++)
	for(int x=0; x<width; x++)
	{
		
		unsigned countSurround = 0;
		for(int i=-1; i<=1; i++)
		for(int j=-1; j<=1; j++)
		// not the center
		if( i != 0 || j != 0 )
		// not out of bounds
		if( y+i >= 0 && y+i < height && x+j >= 0 && x+j < width)	
		if( orig(x+j, y+i) )
		{
			countSurround++;
		}
		
		if( orig(x, y) )
		{
			if( countSurround == 2 || countSurround == 3 )
			{
				dest.set(x, y, true);
			}
			else
			{
				dest.set(x, y, false);
			}
		}
		else
		{
			if( countSurround == 3 )
			{
				dest.set(x, y, true);
			}
			else
			{
				dest.set(x, y, false);
			}
		}
		
	}
}
