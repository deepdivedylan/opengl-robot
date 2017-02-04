#include "robot.h"
#include <cmath>

//! Robot Constructor
/*! Allocates objects needed by the Robot. Also sets up initial parameters. */
Robot::Robot()
{
	try
	{
		double values[3] = {0.0, 1.0, 0.0};
		Vector j_hat(values, 3);
		armAngle = 0.0;
		forearmAngle = 0.0;
		shoulderAngle = 0.0;
		forearmOffset = 0.0;
		fingerAngle = 0.0;
		setMaterial(CARTOON);
		c1 = new Cylinder(20.0, 10.0, 0.0, 0.0, 1.0);
		c2 = new Cylinder(10.0, 20.0, 1.0, 0.0, 0.0);
		c3 = new Cylinder(10.0, 20.0, 1.0, 1.0, 0.0, 90.0, j_hat);
		c4 = new Cylinder(5.0, 30.0, 1.0, 0.0, 1.0, 90.0, j_hat);
		c5 = new Cylinder(3.0, 30.0, 1.0, 0.0, 1.0, 90.0, j_hat);
		c6 = new Cylinder(1.0, 15.0, 1.0, 0.0, 0.0);
		c7 = new Cylinder(1.0, 10.0, 1.0, 0.0, 0.0, 90.0, j_hat);
		c8 = new Cylinder(1.0, 10.0, 1.0, 0.0, 0.0, 90.0, j_hat);
		cube = new Cube(5.0);
		cubeModel = new Matrix(4, 4);
		fingerModel = new Matrix(4, 4);
		cubeOffset[0] = 40.0;
		cubeOffset[1] = 0.0;
		cubeOffset[2] = -10.0;
		cubeRotation[0] = 0.0;
		cubeRotation[1] = 0.0;
		cubeRotation[2] = 0.0;
		grab = false;
	}
	catch (LinAlgException e)
	{
		std::cerr << e.what() << std::endl;
	}
}

//! Robot Destructor.
/*! Frees allocated objects used by the Robot. */
Robot::~Robot()
{
	delete c1;
	delete c2;
	delete c3;
	delete c4;
	delete c5;
	delete c6;
	delete c7;
	delete c8;
	delete cube;
	delete cubeModel;
	delete fingerModel;
}

//! Dropped Method
/*! Indicates whether the Cube is dropped or not.
  \return true if the Cube has been dropped, false if not. */
bool Robot::dropped()
{
	return drop;
}

//! Grab Method
/*! Indicates whether the Cube is grabbed or not.
  \return true if the Cube has been grabbed, false if not. */
bool Robot::grabbed()
{
	return grab;
}

//! inRange Method
/*! Indicates whether the Cube is in range or not.
  \return true if the Cube is in range, false if not. */
bool Robot::inRange()
{
	return ready;
}

//! Mathematically grab the Cube
/*! In general, the Cube is grabbed if \f$P=M_C^{-1}M_F\left[0\quad0\quad0\quad1\right]^T\f$. If \f$P_i<\epsilon\f$, the Cube is close enough and is considered grabbed. */
void Robot::grabCube()
{
	try
	{
		double dx, dy, dz;
		Matrix Point(4,1), transformation = cubeModel->inverse();
		transformation = transformation * (*fingerModel);
		Point.set(3, 0, 1.0);
		Point = transformation * Point;
		
		/* if we just grabbed the cube */
		if (!grab && fabs(Point.at(0, 0)) < 7.5 && fabs(Point.at(1, 0)) < 7.5 && fabs(Point.at(2, 0)) < 7.5)
		{
			if (fingerAngle != 0.0)
			{
				grab = true;
				dx = Point.at(0, 0);
				dy = Point.at(1, 0);
				dz = Point.at(2, 0);
				cubeOffset[0] += dx;
				cubeOffset[1] += dy;
				cubeOffset[2] += dz;
				cubeRotation[0] = forearmAngle;
				cubeRotation[1] = shoulderAngle;
				cubeRotation[2] = fingerAngle;
			}
			else
			{
				grab = false;
				ready = true;
			}
		}
		/* we dropped the cube */
		else if (grab && fingerAngle == 0.0)
		{
			grab = false;
			drop = true;
		}
		/* if we still have the cube in our hand */
		else if (grab && fabs(Point.at(0, 0)) < 7.5 && fabs(Point.at(1, 0)) < 7.5 && fabs(Point.at(2, 0)) < 7.5)
		{
			dx = Point.at(0, 0);
			dy = Point.at(1, 0);
			dz = Point.at(2, 0);
			cubeOffset[0] += dx;
			cubeOffset[1] += dy;
			cubeOffset[2] += dz;
			cubeRotation[0] = forearmAngle;
			cubeRotation[1] = shoulderAngle;
			cubeRotation[2] = fingerAngle;
		}
	}
	catch (LinAlgException e)
	{
		std :: cerr << "Exception: " << e.what() << std :: endl;
	}
		
}

//! Draw the Robot
/*! OpenGL commands to define and draw the robot. */
void Robot::draw()
{
	/* mathematical variables */
	unsigned int material = robotMaterial;
	double model[16];
	double forearmOffsetX, forearmOffsetZ, shoulderRise, shoulderRun;
	double cosPhi, sinPhi, phi;
	double values[3] = {1.0, 0.0, 0.0};
	Vector i_hat(values, 3);
	values[0] = 0.0;
	values[1] = 1.0;
	Vector j_hat(values, 3);
	values[1] = 0.0;
	values[2] = 1.0;
	Vector k_hat(values, 3);
	Vector h(3), v_hat(3);
	
	/* angle calculations */
	phi = DEG2RAD(shoulderAngle);
	cosPhi = cos(phi);
	sinPhi = sin(phi);
	shoulderRise = -30.0 * sinPhi;
	shoulderRun = 30.0 * cosPhi;
	forearmOffsetX = forearmOffset * cosPhi;
	forearmOffsetZ = -forearmOffset * sinPhi;
	
	/* palm is the arm vector cross k hat */
	v_hat.set(0, cosPhi);
	v_hat.set(2, sinPhi);
	h = v_hat % k_hat;
	
	/* draw the cube and save the transformation matrices */
	glPushMatrix();
	setMaterial(CARTOON);
	glTranslated(cubeOffset[0], cubeOffset[1], cubeOffset[2]);
	glRotated(cubeRotation[0], 1.0, 0.0, 0.0);
	glRotated(cubeRotation[1], 0.0, 1.0, 0.0);
	glRotated(cubeRotation[2], 0.0, 0.0, 1.0);
	cube->draw();
	glGetDoublev(GL_MODELVIEW_MATRIX, model);
	cubeModel->load(model, 16);
	glPopMatrix();
	
	/* main robot */
	glPushMatrix();
	setMaterial(material);
	c1->draw();
	glTranslated(0.0, 0.0, 10.0);
	c2->draw();
	glRotated(armAngle, 0.0, 0.0, 1.0);
	glTranslated(-10.0, 0.0, 30.0);
	c3->draw();
	glTranslated(20.0, 0.0, 0.0);
	c4->build(5.0, 30.0, 1.0, 0.0, 1.0, 90.0 + shoulderAngle, j_hat);
	c4->draw();
	glTranslated(shoulderRun - forearmOffsetX, 1.0, shoulderRise - forearmOffsetZ);
	c5->build(3.0, 30.0, 1.0, 0.0, 1.0, 90.0 + shoulderAngle, j_hat);
	c5->draw();
	glTranslated(h[0] + shoulderRun, h[1], h[2] + shoulderRise);
	glRotated(forearmAngle, 1.0, 0.0, 0.0);
	glTranslated(-7.5 * sinPhi, 0.0, -7.5 * cosPhi);
	c6->build(1.0, 15.0, 1.0, 0.0, 0.0, shoulderAngle, j_hat);
	c6->draw();
	c7->build(1.0, 10.0, 1.0, 0.0, 0.0, 90.0 + shoulderAngle - fingerAngle, j_hat);
	c7->draw();
	glTranslated(15.0 * sinPhi, 0.0, 15.0 * cosPhi);
	c8->build(1.0, 10.0, 1.0, 0.0, 0.0, 90.0 + shoulderAngle + fingerAngle, j_hat);
	c8->draw();
	glGetDoublev(GL_MODELVIEW_MATRIX, model);
	fingerModel->load(model, 16);
	glPopMatrix();
}

//! Load Cube Textures
/*! This method exists solely to receive the textures from QRobot and pass them to the Cube.
  \param newFaces the textures to pass along */
void Robot::loadFaces(QImage **newFaces)
{
	cube->loadFaces(newFaces);
}

//! Set Robot Material
/*! This method switches the material the Robot is made of
  \param newMat the new material flag */
void Robot::setMaterial(unsigned short newMat)
{
	GLfloat black[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat cartoon_ambient[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat cartoon_diffuse[] = {0.8, 0.8, 0.8, 1.0};
	GLfloat cartoon_shininess = 16.0;
	GLfloat gold_ambient[] = {0.24725, 0.1995, 0.0745, 1.0};
	GLfloat gold_diffuse[] = {0.75164, 0.60648, 0.22648, 1.0};
	GLfloat gold_specular[] = {0.628281, 0.555802, 0.366065, 1.0};
	GLfloat gold_shininess = 51.2;
	GLfloat obsidian_ambient[] = {0.05375, 0.05, 0.06625, 1.0};
	GLfloat obsidian_diffuse[] = {0.18275, 0.17, 0.22525, 1.0};
	GLfloat obsidian_specular[] = {0.332741, 0.328634, 0.346435, 1.0};
	GLfloat obsidian_shininess = 38.4;
	GLfloat silver_ambient[] = {0.19225, 0.19225, 0.19225, 1.0};
	GLfloat silver_diffuse[] = {0.50754, 0.50754, 0.50754, 1.0};
	GLfloat silver_specular[] = {0.508273, 0.508273, 0.508273, 1.0};
	GLfloat silver_shininess = 51.2;
	
	robotMaterial = newMat;
	glMaterialfv(GL_FRONT, GL_EMISSION, black);
	switch (robotMaterial)
	{
		case SILVER:
		{
			glDisable(GL_COLOR_MATERIAL);
			glMaterialfv(GL_FRONT, GL_AMBIENT, silver_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, silver_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, silver_specular);
			glMaterialf(GL_FRONT, GL_SHININESS, silver_shininess);
			break;
		}
		case OBSIDIAN:
		{
			glDisable(GL_COLOR_MATERIAL);
			glMaterialfv(GL_FRONT, GL_AMBIENT, obsidian_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, obsidian_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, obsidian_specular);
			glMaterialf(GL_FRONT, GL_SHININESS, obsidian_shininess);
			break;
		}
		case GOLD:
		{
			glDisable(GL_COLOR_MATERIAL);
			glMaterialfv(GL_FRONT, GL_AMBIENT, gold_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, gold_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, gold_specular);
			glMaterialf(GL_FRONT, GL_SHININESS, gold_shininess);
			break;
		}
		case CARTOON:
		default:
		{
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
			glMaterialfv(GL_FRONT, GL_AMBIENT, cartoon_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, cartoon_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, black);
			glMaterialf(GL_FRONT, GL_SHININESS, cartoon_shininess);
			glEnable(GL_COLOR_MATERIAL);
			break;
		}
	}
}

//! Set Drop Flag
/*! Used by QRobot to change the drop flag.
  \param newDrop the new flag state */
void Robot::setDropped(bool newDrop)
{
	drop = newDrop;
}

//! Set Finger Angle
/*! Used by QRobot to set the finger angle.
  \param newFinger the new finger angle */
void Robot::setFingers(GLdouble newFinger)
{
	fingerAngle = newFinger;
}

//! Set Cube Height
/*! Used by QRobot to set the distance from the floor to the Cube. This method is used when the cube is in free fall.
  \param newHeight the new height from the floor */
void Robot::setCubeHeight(GLdouble newHeight)
{
	cubeOffset[2] = newHeight;
}

//! Get Arm Angle
/*! Get the angle of rotation orthagonal the floor
  \return the angle of rotation */
GLdouble Robot::getArm()
{
	return armAngle;
}

//! Get Shoulder Angle
/*! Get the angle of the shoulder with respect to the \f$y\f$ axis
  \return the angle of rotation */
GLdouble Robot::getShoulder()
{
	return shoulderAngle;
}

//! Get Forearm Angle
/*! Get the angle of the forearm with respect to the shoulder
  \return the angle of rotation */
GLdouble Robot::getForearm()
{
	return forearmAngle;
}

//! Get Forearm Offset
/*! Get the amount the forearm is extended.
  \return the distance the forearm is extended */
GLdouble Robot::getForearmOffset()
{
	return forearmOffset;
}

//! Get Finger Angle
/*! Get the angle between the wrist and fingers
  \return the angle */
GLdouble Robot::getFingers()
{
	return fingerAngle;
}

//! Get Cube Height
/*! Used by QRobot to get the distance from the floor to the Cube. This method is used when the cube is in free fall.
  \return the height from the floor */
GLdouble Robot::getCubeHeight()
{
	return cubeOffset[2];
}

//! Set Arm Angle
/*! Set the angle of rotation orthagonal the floor
  \param newArm the angle of rotation */
void Robot::setArm(GLdouble newArm)
{
	if (newArm < 0.0)
		newArm += 360;
	else if (newArm > 360.0)
		newArm -= 360.0;
	armAngle = newArm;
}

//! Set Shoulder Angle
/*! Set the angle of the shoulder with respect to the \f$y\f$ axis
  \param newShoulder the angle of rotation */
void Robot::setShoulder(GLdouble newShoulder)
{
	if (newShoulder < -60.0)
		newShoulder = -60.0;
	if (newShoulder > 60.0)
		newShoulder = 60.0;
	shoulderAngle = newShoulder;
}

//! Set Forearm Angle
/*! Set the angle of the forearm with respect to the shoulder
  \param newForearm the angle of rotation */
void Robot::setForearm(GLdouble newForearm)
{
	if (newForearm < 0.0)
		newForearm += 360;
	else if (newForearm > 360.0)
		newForearm -= 360.0;
	forearmAngle = newForearm;
}

//! Set Forearm Offset
/*! Set the amount the forearm is extended.
  \param newForearmOffset the distance the forearm is extended */
void Robot::setForearmOffset(GLdouble newForearmOffset)
{
	if (newForearmOffset < 1.0)
		newForearmOffset = 1.0;
	if (newForearmOffset > 30.0)
		newForearmOffset = 30.0;
	forearmOffset = newForearmOffset;
}

//! Set Cube Texture Flag
/*! This method exists solely to receive the texture flag from QRobot and pass it to the Cube.
  \param newText the flag to pass along */
void Robot::setTexturing(bool newText)
{
	cube->setTexturing(newText);
}
