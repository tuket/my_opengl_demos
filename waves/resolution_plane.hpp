#define GLEW_STATIC
#include <GL/glew.h>

#ifndef RESOLUTION_PLANE
#define RESOLUTION_PLANE

class ResolutionPlane
{
	
	unsigned rw, rh;
	float w, h;
	
	GLfloat* vertices;
	GLuint* indices;
	
	unsigned verticesCount;
	unsigned indicesCount;
	unsigned trianglesCount;
	
	GLuint VAO;		//< vertex attibute object
	GLuint VBO;		//< vertex buffer object
	GLuint EBO;		//< element buffer object
	
	bool error;
	
	static GLuint shaderProgram;
	
public:
	
	ResolutionPlane
	(
		float w, float h,
		unsigned rw, unsigned rh
	);
	
	unsigned getTrianglesCount()const;
	unsigned getIndicesCount()const;
	unsigned getVerticesCount()const;
	
	void draw(float time);
	
	static void initShaders();
	
};

#endif
