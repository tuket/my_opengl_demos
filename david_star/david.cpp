#include <GL/glut.h>
#include <cmath>

void init();
void diplay();

static const GLfloat rInt = 0.7;
static const GLfloat rExt = 1;

static const GLuint indices[] = 
{
	
	4, 1, 3, 0, 5, 2, 4, 1,
	10, 7, 9, 6, 11, 8, 10, 7
	
};

static const GLfloat vertices[] =
{
	0, rInt, 0,
	-cos(M_PI_2/3)*rInt, -sin(M_PI_2/3)*rInt, 0,
	cos(M_PI_2/3)*rInt, -sin(M_PI_2/3)*rInt, 0,
	
	0, rExt, 0,
	-cos(M_PI_2/3)*rExt, -sin(M_PI_2/3)*rExt, 0,
	cos(M_PI_2/3)*rExt, -sin(M_PI_2/3)*rExt, 0,
	
	// 6
	
	0, -rInt, 0,
	-cos(M_PI_2/3)*rInt, sin(M_PI_2/3)*rInt, 0,
	cos(M_PI_2/3)*rInt, sin(M_PI_2/3)*rInt, 0,
	
	0, -rExt, 0,
	-cos(M_PI_2/3)*rExt, sin(M_PI_2/3)*rExt, 0,
	cos(M_PI_2/3)*rExt, sin(M_PI_2/3)*rExt, 0
	
};

static const GLfloat colors[] =
{
	1, 0, 0,
	1, 0, 0,
	1, 0, 0,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	1, 0, 0,
	1, 0, 0,
	1, 0, 0,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1
	
};

int main(int argc, char** argv){
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(64, 64);
	glutCreateWindow("Estrella david");
	
	init();
	
	glutMainLoop();
	
	return 0;
	
}

void display(){
	
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	glColor3f(0,0,1);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colors);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_INT, indices);
	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_INT, &indices[8]);
	
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glFlush();
	glutSwapBuffers();
	
}

void init(){
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	
	glutDisplayFunc(display);
	
}
