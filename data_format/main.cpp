#include <iostream>
#include <vector>
#include "SDL2/SDL.h"
#include <GL/glew.h>
#include "../inc/vertex_data.hpp"

using namespace std;

int main(int argc, char* argv[]){

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_Window* window =
		SDL_CreateWindow
		(
		"data_format",
		10, 10, 800, 600,
		SDL_WINDOW_OPENGL
		);

	SDL_GLContext context = SDL_GL_CreateContext(window);

	// init glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cerr << "Error: " << glewGetErrorString(err) << endl;
		exit(1);
	}
	cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;
	
	

	glClear(GL_COLOR_BUFFER_BIT);
		
	return 0;
}
