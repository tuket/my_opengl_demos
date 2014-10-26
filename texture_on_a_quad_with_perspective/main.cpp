#include <GL/glut.h>
#include <GL/gl.h>
#include <FreeImage.h>
#include <stdio.h>
#include <cmath>

const float toRad = 2*M_PI/360;

GLfloat coordinates[] =
{
    -0.5, 0.5, -10,
    -0.5, -0.5, -10,
    0.5, -0.5, -5,
    0.5, 0.5, -5
};

GLfloat texCoords[] =
{
    0, 1,
    0, 0,
    1, 0,
    1, 1
};

BYTE* data;

FIBITMAP* bitmap;
GLuint texture;

void initGlutCallbacks();
void initGL();
void onReshape(int w, int h);
void display();
FIBITMAP* loadTexture(const char* fileName);

int main(int argc, char** argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(64, 64);
    glutCreateWindow("arrays");

    initGlutCallbacks();
    initGL();

    // texture
    bitmap = loadTexture("rufol.png");

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    data = FreeImage_GetBits(bitmap);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA8, 256, 256, 0,
        GL_RGBA, GL_UNSIGNED_BYTE,
        data
    );

    // enable arrays
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // specifying data for the arrays
    glVertexPointer
    (
        3, GL_FLOAT, 0, coordinates
    );

    glTexCoordPointer
    (
        2, GL_FLOAT, 0, texCoords
    );


    glutMainLoop();

}

void initGlutCallbacks(){
    glutReshapeFunc(onReshape);
    glutDisplayFunc(display);
}

void initGL(){

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);

    glEnable ( GL_TEXTURE_2D );

}

void onReshape(int w, int h){

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float hVertAngle = 20 * toRad;
    float hHoriAngle =
    atan(
        (w/2)/
        (
            (h/2)/
            tan(hVertAngle)
        )
    );


    float near = 5;
    float far = 100000;

    float top, bot;
    top = bot = 2 * near * tan(hHoriAngle);
    float left, right;
    left = right = 2 * near * tan(hVertAngle);

    printf("left: %f\nright: %f\nbot: %f\ntop: %f\nnear: %f\nfar: %f\n", left, right, bot, top, near, far);
    glFrustum(-left, right, -bot, top, near, far);

    glMatrixMode(GL_MODELVIEW);

}

void display(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glDrawArrays(GL_QUADS, 0, 4);

    glFlush();
    glutSwapBuffers();


}

FIBITMAP* loadTexture(const char* fileName){

    FIBITMAP *bitmap = FreeImage_Load(FIF_PNG, "rufol.png");
    if(bitmap == 0) printf("error loading the image\n");
    FIBITMAP *fbitmap = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_Unload(bitmap);
    return fbitmap;

}
