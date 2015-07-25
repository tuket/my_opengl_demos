#include "sdl_and_gl.hpp"

#ifndef GRID_HPP
#define GRID_HPP

class Grid
{

	bool generated;
	bool* cells;
	GLubyte* textureData;
	unsigned width, height;
	// texture dimensions must be power of 2
	unsigned textureWidth, textureHeight;
	
public:

	Grid(unsigned width, unsigned height);
	
	bool operator()(unsigned x, unsigned y)const;
	void set(unsigned x, unsigned y, bool b);
	
	unsigned getWidth()const;
	unsigned getHeight()const;
	
	GLubyte* getTextureData()const;
	
	unsigned getTextureWidth()const;
	unsigned getTextureHeight()const;
	
	~Grid();
	
};

#endif
