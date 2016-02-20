#ifndef RENDERCYLINDER
#define RENDERCYLINDER 

#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdlib.h>

typedef struct
{
	GLfloat x,y,z;
	GLfloat r,g,b,a;
} Vertex;

typedef struct
{
	GLfloat r,g,b,a;
} Color;

void renderCylinder(const GLfloat &x1, const GLfloat &y1, const GLfloat &z1,
                    const GLfloat &r1, const GLfloat &g1, const GLfloat &b1,
                    const GLfloat &a1,
                    const GLfloat &x2, const GLfloat &y2, const GLfloat &z2,
                    const GLfloat &r2, const GLfloat &g2, const GLfloat &b2,
                    const GLfloat &a2,
                    const GLfloat &r, const GLfloat &g, const GLfloat &b,
                    const GLfloat &a,
                    const float &radius,const int &subdivisions,
                    GLUquadricObj *quadric);

void renderCylinder_convenient(const Vertex v1, const Vertex v2,
							   float r, float g, float b, float a, 
							   float radius, int subdivisions);

void drawBoundary(const float &MaxDimension);

void drawRowBars(const float &BarRadius, const float &BarApartDis,
                 const int &BarNum, const float &MaxDimension,
                 const float &BarStartPos, const float &BarStopPos,
                 const float &z, const Color &DiskColor, const Color &BarColor,
                 const int &subdivisions,bool upper);

void drawAllBars();

#endif
