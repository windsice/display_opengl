#include "renderCylinder.hpp"

/**
 * Rendering a Cylinder Between Two Points in OpenGL
 * -------------------------------------------------
 * This problem is more of a pain than one would think!
 * Here is some code that does it, based on the implementation
 * which used to be posted at the following URL by Joel J. Parris:
 * http://home.neo.rr.com/jparris/OpenGL%20-%20draw%20cylinder%20between%202%20pts.htm
 *
 * Unfortunately Joel's link is down, and I'm not totally sure what
 * kind of license he put on it if any. I hope he doesn't mind that I declare:
 *
 * This code is in the public domain.
 *
 * I would welcome any improvements as code contributions.
 *
 * -- Curran Kelleher 12/22/2010
 */
 
 /*
  * Edited using the suggestions found in the replies to the blog post (mostly Edward's version)
  * And some tinkering of my own in the edge cases where it still messed up
  * Any further bugfixes are welcome.
  * -- Joost 08/10/2012
*/
void renderCylinder(const GLfloat &x1, const GLfloat &y1, const GLfloat &z1,
					const GLfloat &r1, const GLfloat &g1, const GLfloat &b1,
					const GLfloat &a1,
					const GLfloat &x2, const GLfloat &y2, const GLfloat &z2, 
					const GLfloat &r2, const GLfloat &g2, const GLfloat &b2,
					const GLfloat &a2,
					const GLfloat &r, const GLfloat &g, const GLfloat &b,
					const GLfloat &a,
					const float &radius,const int &subdivisions,
					GLUquadricObj *quadric)
{
  GLfloat vx = x2-x1;
  GLfloat vy = y2-y1;
  GLfloat vz = z2-z1;
  GLfloat v = sqrt( vx*vx + vy*vy + vz*vz );
  GLfloat ax;

  if (fabs(vz) < 1.0e-3) {
    ax = 57.2957795*acos( vx/v ); // rotation angle in x-y plane
    if ( vy <= 0.0 )
      ax = -ax;
  }
  else {
    ax = 57.2957795*acos( vz/v ); // rotation angle
    if ( vz <= 0.0 )
      ax = -ax;
  }
  
  GLfloat rx = -vy*vz;
  GLfloat ry = vx*vz;
  
  glPushMatrix();
  //draw the cylinder body
  glTranslatef( x1,y1,z1 );
  if (fabs(vz) < 1.0e-3) {
    glRotated(90.0, 0, 1, 0.0); // Rotate & align with x axis
    glRotated(ax, -1.0, 0.0, 0.0); // Rotate to point 2 in x-y plane
  }
  else {
    glRotated(ax, rx, ry, 0.0); // Rotate about rotation vector
  }

  glColor4f(r,g,b,a);
  gluQuadricOrientation(quadric,GLU_OUTSIDE);
  gluCylinder(quadric, radius, radius, v, subdivisions, 1);

  glColor4f(r1,g1,b1,a1);
  //draw the first cap
  gluQuadricOrientation(quadric,GLU_INSIDE);
  gluDisk( quadric, 0.0, radius, subdivisions, 1);
  glTranslatef( 0,0,v );

  glColor4f(r2,g2,b2,a2);
  //draw the second cap
  gluQuadricOrientation(quadric,GLU_OUTSIDE);
  gluDisk( quadric, 0.0, radius, subdivisions, 1);
  glPopMatrix();
}

void renderCylinder_convenient(const Vertex v1, const Vertex v2,
							   float r, float g, float b, float a,
								float radius, int subdivisions)
{
  //the same quadric can be re-used for drawing many cylinders
  GLUquadricObj *quadric=gluNewQuadric();
  gluQuadricNormals(quadric, GLU_SMOOTH);
  renderCylinder(v1.x,v1.y,v1.z,v1.r,v1.g,v1.b,v1.a,
				 v2.x,v2.y,v2.z,v2.r,v2.g,v2.b,v2.a,
				 r,g,b,a,radius,subdivisions,quadric);
  gluDeleteQuadric(quadric);
}

void drawRowBars(const float &BarRadius, const float &BarApartDis,
				 const int &BarNum, const float &MaxDimension,
				 const float &BarStartPos, const float &BarStopPos,
				 const float &z, const Color &DiskColor, const Color &BarColor,
				 const int &subdivisions, bool upper)
{
    Vertex *v1 = (Vertex *) calloc(BarNum,sizeof(Vertex));
    Vertex *v2 = (Vertex *) calloc(BarNum,sizeof(Vertex));

	float pos;
	int i;
    for(pos = BarStartPos, i = 0; pos < BarStopPos;
        pos+= BarApartDis, i++){
        //position
		if(upper){
			v1[i].x = v2[i].x = pos;
			v1[i].y = 0;
			v2[i].y = MaxDimension;
		} else {
			v1[i].y = v2[i].y = pos;
			v1[i].x = 0;
			v2[i].x = MaxDimension;
		}
        v1[i].z = v2[i].z = z;

        //color
		v1[i].r = v2[i].r = DiskColor.r;
		v1[i].g = v2[i].g = DiskColor.g;
		v1[i].b = v2[i].b = DiskColor.b;
		v1[i].a = v2[i].a = DiskColor.a;

        renderCylinder_convenient(v1[i],v2[i],
                                  BarColor.r,BarColor.g,BarColor.b,
								  BarColor.a,BarRadius,subdivisions);
    }
	free(v1);
	free(v2);
}

void drawBoundary(const float &MaxDimension){
	Color c = {1,1,1,1};

	Vertex v1 = {0,0,0,c.r,c.g,c.b,c.a};
	Vertex v2 = {0,0,MaxDimension,c.r,c.g,c.b,c.a};
	Vertex v3 = {0,MaxDimension,0,c.r,c.g,c.b,c.a};
	Vertex v4 = {MaxDimension,0,0,c.r,c.g,c.b,c.a};
	Vertex v5 = {MaxDimension,MaxDimension,0,c.r,c.g,c.b,c.a};
	Vertex v6 = {0,MaxDimension,MaxDimension,c.r,c.g,c.b,c.a};
	Vertex v7 = {MaxDimension,0,MaxDimension,c.r,c.g,c.b,c.a};
	Vertex v8 = {MaxDimension,MaxDimension,MaxDimension,c.r,c.g,c.b,c.a};

	glLineWidth(1.0f);
	
	glColor4f(c.r,c.g,c.b,c.a);

	glBegin(GL_LINES);
	glVertex3f(v1.x,v1.y,v1.z);
	glVertex3f(v3.x,v3.y,v3.z);

	glVertex3f(v1.x,v1.y,v1.z);
	glVertex3f(v4.x,v4.y,v4.z);

	glVertex3f(v1.x,v1.y,v1.z);
	glVertex3f(v2.x,v2.y,v2.z);

	glVertex3f(v3.x,v3.y,v3.z);
	glVertex3f(v6.x,v6.y,v6.z);

	glVertex3f(v3.x,v3.y,v3.z);
	glVertex3f(v5.x,v5.y,v5.z);

	glVertex3f(v2.x,v2.y,v2.z);
	glVertex3f(v6.x,v6.y,v6.z);

	glVertex3f(v4.x,v4.y,v4.z);
	glVertex3f(v5.x,v5.y,v5.z);

	glVertex3f(v2.x,v2.y,v2.z);
	glVertex3f(v7.x,v7.y,v7.z);

	glVertex3f(v4.x,v4.y,v4.z);
	glVertex3f(v7.x,v7.y,v7.z);

	glVertex3f(v8.x,v8.y,v8.z);
	glVertex3f(v6.x,v6.y,v6.z);

	glVertex3f(v8.x,v8.y,v8.z);
	glVertex3f(v7.x,v7.y,v7.z);

	glVertex3f(v8.x,v8.y,v8.z);
	glVertex3f(v5.x,v5.y,v5.z);
	glEnd();
}

void drawAllBars(){

    float BarRadius = 0.1;
    float BarApartDis = 0.25; //distance between center point of two bar
    float UBarStartPosition = 2.88; //center point of the first bar (Upper)
    float LBarStartPosition = 3.01;
    float UBarStopPosition = 12.13; //center point of the last bar (Upper)
	float LBarStopPosition = 12.0;

    float UpperBarZ = 11;
    float LowerBarZ = 4;

    float MaxDimension = 15;

    int HighBarNum = 38;
    int LowBarNum = 37;
    float BarZDiff = 0.44;

	Color disk = {1,1,1,1};
	Color bar = {0.3,0.3,0.3,0.3};
	drawRowBars(BarRadius, BarApartDis,HighBarNum, MaxDimension,
				UBarStartPosition,UBarStopPosition,UpperBarZ,disk,bar,10,true);
	drawRowBars(BarRadius, BarApartDis,LowBarNum, MaxDimension,
				LBarStartPosition,LBarStopPosition,
				UpperBarZ-BarZDiff,disk,bar,10,true);
	drawRowBars(BarRadius, BarApartDis,LowBarNum, MaxDimension,
				LBarStartPosition,LBarStopPosition,LowerBarZ,disk,bar,10,false);
	drawRowBars(BarRadius, BarApartDis,HighBarNum, MaxDimension,
				UBarStartPosition,UBarStopPosition,
				LowerBarZ-BarZDiff,disk,bar,10,false);

	drawBoundary(MaxDimension);
}

