#ifndef VERTEX_DATA_HPP
#define VERTEX_DATA_HPP

#include <GL/glew.h>

struct Attrib
{
	unsigned num;	//< the number of floats that make this attr
	unsigned name;	//< the name of the attr in the shaders (position in "names")
};

typedef unsigned AttribId;

struct VertexDataFormat
{
	std::vector<AttribId> attribs;	//< list of ids of the attributes
};

class VertexDataFormatManager
{
	
	std::string names;
	std::vector<Attrib>  attribs;
	
	
};

#endif
