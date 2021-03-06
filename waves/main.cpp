#include "sdl_and_gl.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "resolution_plane.hpp"
#include <glm/glm.hpp>

using namespace std;

const unsigned SCREEN_WIDTH = 800;
const unsigned SCREEN_HEIGHT = 600;

const float PI = 3.14159265359f;

void initGL();

int main()
{
	
	SDL_Init( SDL_INIT_EVERYTHING );
	
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
	SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
	
	
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
	//glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if( GLEW_OK != err )
	{
		cerr << "Error: " << glewGetErrorString(err) << endl;
		exit(1);
	}
	cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;
	
	initGL();
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	ResolutionPlane::initShaders();
	ResolutionPlane plane
	(
		50, 50,
		50, 50
	);
	
	float time = 0;
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
		
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Drawing!
		plane.draw(time);
		
		SDL_GL_SwapWindow(window);
		
		if(time>100*PI) time = 0;
		time += PI/100.0f;
		
	}
		
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	
}

void initGL()
{
	glClearColor(0.5, 0.5, 0.5, 0.0);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}
