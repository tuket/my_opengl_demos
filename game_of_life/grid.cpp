#include "grid.hpp"
#include "util.hpp"

Grid::Grid(unsigned width, unsigned height)
{
	this->width = width;
	this->height = height;
	
	textureWidth = nextPowerOfTwo(width);
	textureHeight = nextPowerOfTwo(height);
	
	cells = new bool[width*height];
	textureData = new GLubyte[textureWidth*textureHeight*3];
	
	for(unsigned i=0; i<width*height; i++)
	{
		cells[i] = false;
	}
	for(unsigned i=0; i<textureWidth*textureHeight*3; i++)
	{
		textureData[i] = 0;
	}
}

bool Grid::operator()(unsigned x, unsigned y)const
{
	return cells[x + y*width];
}

void Grid::set(unsigned x, unsigned y, bool b)
{
	cells[x + y*width] = b;
	GLubyte col = b ? ~((GLubyte)0) : 0;
	textureData[3*(x + y*textureWidth)+0] = col;
	textureData[3*(x + y*textureWidth)+1] = col;
	textureData[3*(x + y*textureWidth)+2] = col;
}

unsigned Grid::getWidth()const
{
	return width;
}

unsigned Grid::getHeight()const
{
	return height;
}

GLubyte* Grid::getTextureData()const
{
	return textureData;
}

unsigned Grid::getTextureWidth()const
{
	return textureWidth;
}

unsigned Grid::getTextureHeight()const
{
	return textureHeight;
}

Grid::~Grid()
{
	delete[] cells;
	delete[] textureData;
}
