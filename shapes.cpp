#include "robot.h"

//! Cube Constructor
/*! Sets up initial parameters for the Cube
  \param length the length of a side of the cube
  \param light will use lighting if true (default), won't use lighting if false */
Cube::Cube(GLdouble length, bool light)
{
	side = length;
	lighting = light;
	texturing = true;
	textures = new GLuint[6];
}

//! Cube Destructor
/*! Frees allocated objects needed by the Cube */
Cube::~Cube()
{
	delete textures;
}

//! Draw Method
/*! Draws the cube using GL_QUADS. Sets normals if lighting is enabled. Loads and maps textures if texturing is enabled. */
void Cube::draw()
{
	/* color for the cube */
	glColor3d(1.0, 0.0, 0.0);

	/* top face: 6 */
	if (texturing)
	{
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &textures[5]);
		glBindTexture(GL_TEXTURE, textures[5]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, faces[5]->width(), faces[5]->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, faces[5]->bits());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glBegin(GL_QUADS);
		if (lighting)
			glNormal3d(0.0, 1.0, 0.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-side, -side,  side);
		glTexCoord2d(1.0, 0.0); glVertex3d( side, -side,  side);
		glTexCoord2d(1.0, 1.0); glVertex3d( side,  side,  side);
		glTexCoord2d(0.0, 1.0); glVertex3d(-side,  side,  side);
	glEnd();

	/* bottom face: 1 */
	if (texturing)
	{
		glGenTextures(1, &textures[0]);
		glBindTexture(GL_TEXTURE, textures[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, faces[0]->width(), faces[0]->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, faces[0]->bits());
	}
	glBegin(GL_QUADS);
		if (lighting)
			glNormal3d(0.0, -1.0, 0.0);
		glTexCoord2d(1.0, 0.0); glVertex3d(-side, -side, -side);
		glTexCoord2d(1.0, 1.0); glVertex3d(-side,  side, -side);
		glTexCoord2d(0.0, 1.0); glVertex3d( side,  side, -side);
		glTexCoord2d(0.0, 0.0); glVertex3d( side, -side, -side);
	glEnd();

	/* back face: 5 */
	if (texturing)
	{
		glGenTextures(1, &textures[4]);
		glBindTexture(GL_TEXTURE, textures[4]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, faces[4]->width(), faces[4]->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, faces[4]->bits());
	}
	glBegin(GL_QUADS);
		if (lighting)
			glNormal3d(0.0, 0.0, -1.0);
		glTexCoord2d(0.0, 1.0); glVertex3d(-side,  side, -side);
		glTexCoord2d(0.0, 0.0); glVertex3d(-side,  side,  side);
		glTexCoord2d(1.0, 0.0); glVertex3d( side,  side,  side);
		glTexCoord2d(1.0, 1.0); glVertex3d( side,  side, -side);
	glEnd();

	/* front face: 2 */
	if (texturing)
	{
		glGenTextures(1, &textures[1]);
		glBindTexture(GL_TEXTURE, textures[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, faces[1]->width(), faces[1]->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, faces[1]->bits());
	}
	glBegin(GL_QUADS);
		if (lighting)
			glNormal3d(0.0, 0.0, 1.0);
		glTexCoord2d(1.0, 1.0); glVertex3d(-side, -side, -side);
		glTexCoord2d(0.0, 1.0); glVertex3d( side, -side, -side);
		glTexCoord2d(0.0, 0.0); glVertex3d( side, -side,  side);
		glTexCoord2d(1.0, 0.0); glVertex3d(-side, -side,  side);
	glEnd();

	/* right face: 4 */
	if (texturing)
	{
		glGenTextures(1, &textures[3]);
		glBindTexture(GL_TEXTURE, textures[3]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, faces[3]->width(), faces[3]->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, faces[3]->bits());
	}
	glBegin(GL_QUADS);
		if (lighting)
			glNormal3d(1.0, 0.0, 0.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( side, -side, -side);
		glTexCoord2d(1.0, 1.0); glVertex3d( side,  side, -side);
		glTexCoord2d(0.0, 1.0); glVertex3d( side,  side,  side);
		glTexCoord2d(0.0, 0.0); glVertex3d( side, -side,  side);
	glEnd();

	/* left face: 3 */
	if (texturing)
	{
		glGenTextures(1, &textures[2]);
		glBindTexture(GL_TEXTURE, textures[2]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, faces[2]->width(), faces[2]->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, faces[2]->bits());
	}
	glBegin(GL_QUADS);
		if (lighting)
			glNormal3d(-1.0, 0.0, 0.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-side, -side, -side);
		glTexCoord2d(1.0, 0.0); glVertex3d(-side, -side,  side);
		glTexCoord2d(1.0, 1.0); glVertex3d(-side,  side,  side);
		glTexCoord2d(0.0, 1.0); glVertex3d(-side,  side, -side);
	glEnd();
	if (texturing)
		glDisable(GL_TEXTURE_2D);
}

//! LoadFaces Method
/*! Loads an array of textures into an array of pointers of type QImage that contain OpenGL-compatible image data. 
 \param newFaces pointer to an array of QImages containing the textures */
void Cube::loadFaces(QImage **newFaces)
{
	faces = newFaces;
}

//! setTexturing Method
/*! Sets the texturing flag.
  \param newText the state of the new flag */
void Cube::setTexturing(bool newText)
{
	texturing = newText;
}

//! Cylinder Constructor
/*! Performs initial setup of variables and calls build() to build the cylinder.
  \param radius the radius of the cylinder
  \param height the height of the cylinder
  \param red the red component of the cylinder
  \param green the green component of the cylinder
  \param blue the blue component of the cylinder
  \param light if true, lighting is used (default); if false, lighting is not used 
  \sa build() */
Cylinder::Cylinder(double radius, double height, double red, double green, double blue, bool light)
{
	startList = glGenLists(3);
	qobj = gluNewQuadric();
	lighting = light;
	build(radius, height, red, green, blue, 0.0, *(Vector *)NULL, false);
}

//! Cylinder Constructor
/*! Performs initial setup of variables and calls build() to build the Cylinder.
  \param radius the radius of the cylinder
  \param height the height of the cylinder
  \param red the red component of the cylinder
  \param green the green component of the cylinder
  \param blue the blue component of the cylinder
  \param angle angle of rotation
  \param axis reference to a Vector containing the axis of rotation
  \param rotate if true, the Cylinder will be rotated by \a angle around \a axis (default); if false, the Cylinder is not rotated
  \param light if true, lighting is used (default); if false, lighting is not used 
  \sa build() */
Cylinder::Cylinder(double radius, double height, double red, double green, double blue, double angle, Vector &axis, bool rotate, bool light)
{
	startList = glGenLists(3);
	qobj = gluNewQuadric();
	lighting = light;
	build(radius, height, red, green, blue, angle, axis, rotate);
}

//! Cylinder Destructor
/*! Frees objects needed by the Cylinder. */
Cylinder::~Cylinder()
{
	gluDeleteQuadric(qobj);
}

//! Cylinder Builder
/*! Called by the constructor to setup the Cylinder before it is drawn.
  \param radius the radius of the cylinder
  \param height the height of the cylinder
  \param red the red component of the cylinder
  \param green the green component of the cylinder
  \param blue the blue component of the cylinder
  \param angle angle of rotation
  \param axis reference to a Vector containing the axis of rotation
  \param rotate if true, the Cylinder will be rotated by \a angle around \a axis (default); if false, the Cylinder is not rotated */
void Cylinder::build(double radius, double height, double red, double green, double blue, double angle, Vector &axis, bool rotate)
{
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);
	glNewList(startList, GL_COMPILE);
	if (lighting)
		glNormal3d(0.0, -1.0, 0.0);
	glColor3d(red, green, blue);
	if (rotate)
	{
		glPushMatrix();
		glRotated(angle, axis[0], axis[1], axis[2]);
	}
	gluDisk(qobj, 0.0, radius, 32, 32);
	if (rotate)
		glPopMatrix();
	glEndList();
	
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);
	glNewList(startList+1, GL_COMPILE);
	if (lighting)
		glNormal3d(0.0, 1.0, 0.0);
	glColor3d(red, green, blue);
	glPushMatrix();
	if (rotate)
		glRotated(angle, axis[0], axis[1], axis[2]);
	else
		glTranslated(0.0, 0.0, height);
	gluDisk(qobj, 0.0, radius, 32, 32);
	glPopMatrix();
	glEndList();
	
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);
	glNewList(startList+2, GL_COMPILE);
	if (lighting)
		glNormal3d(0.0, 0.0, 1.0);
	glColor3d(red, green, blue);
	if (rotate)
	{
		glPushMatrix();
		glRotated(angle, axis[0], axis[1], axis[2]);
	}
	gluCylinder(qobj, radius, radius, height, 32, 32);
	if (rotate)
		glPopMatrix();
	glEndList();
}

//! Draw Method
/*! Executes the startList to render the Cylinder */
void Cylinder::draw()
{
	glPushMatrix();
	glCallList(startList);
	glCallList(startList+1);
	glCallList(startList+2);
	glPopMatrix();
}
