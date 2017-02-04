#include "robot.h"
#include <QColorDialog>

//! Lighting constructor
/*! Allocates needed objects and sets default parameters.
  \param parent parent widget - automatically taken care of by Qt */
Lighting::Lighting(QWidget *parent) : QWidget(parent)
{
	/* allocate spotlight angles */
	angles = new GLfloat[5];
	for (unsigned int i=0; i<5; i++)
		angles[i] = 45.0;

	/* allocate light colors */
	colors = new GLfloat*[5];
	for (unsigned int i=0; i<5; i++)
		colors[i] = new GLfloat[4];

	/* default light color is white */
	for (unsigned int i=0; i<5; i++)
		for (unsigned int j=0; j<4; j++)
			colors[i][j] = 1.0;

	/* allocate light coordinates */
	coords = new GLfloat*[5];
	for (unsigned int i=0; i<5; i++)
		coords[i] = new GLfloat[4];

	/* default coordinate is the origin */
	for (unsigned int i=0; i<5; i++)
		for (unsigned int j=0; j<4; j++)
			coords[i][j] = 0.0;

	/* special characteristics for lights 1 and 2 */
	coords[0][0] = 10.0;
	coords[0][1] = 10.0;
	coords[0][2] = 10.0;
	coords[1][0] = -10.0;
	coords[1][1] = -10.0;
	coords[1][2] = -10.0;

	/* flip light switches */
	lightSwitches.light1 = OFF;
	lightSwitches.light2 = OFF;
	lightSwitches.light3 = OFF;
	lightSwitches.light4 = OFF;
	lightSwitches.light5 = OFF;
	lightSwitches.master = ON;
	lightSwitches.type1 = DIRECTIONAL;
	lightSwitches.type2 = DIRECTIONAL;
	lightSwitches.type3 = DIRECTIONAL;
	lightSwitches.type4 = DIRECTIONAL;
	lightSwitches.type5 = DIRECTIONAL;
}

//! Lighting Constructor
/*! Frees previously allocated memory. */
Lighting::~Lighting()
{
	disable();
	for (unsigned int i=0; i<5; i++)
	{
		delete colors[i];
		delete coords[i];
	}
	delete angles;
	delete colors;
	delete coords;
}

//! Enabled Member
/*! Tells whether lighting is enabled.
  \return true if enabled, false if not */
bool Lighting::enabled()
{
	if (lightSwitches.master==ON)
		return true;
	else
		return false;
}

//! Enable Member
/*! Flips the master switch and sets up lighting. */
void Lighting::enable()
{
	GLfloat ambient[] = {0.1, 0.1, 0.1, 1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	glEnable(GL_LIGHTING);
	lightSwitches.master = ON;
}

//! Disable Member
/*! Flips the master switch and disables lighting. */
void Lighting::disable()
{
	glDisable(GL_LIGHTING);
	lightSwitches.master = OFF;
}

//! GetType Accessor Method
/*! Tells the type of the specified light.
  \param lightNum the light to check
  \return Enumerated light type */
unsigned short Lighting::getType(unsigned short lightNum)
{
	switch (lightNum)
	{
		case LIGHT1:
			return lightSwitches.type1;
		case LIGHT2:
			return lightSwitches.type2;
		case LIGHT3:
			return lightSwitches.type3;
		case LIGHT4:
			return lightSwitches.type4;
		case LIGHT5:
			return lightSwitches.type5;
		default:
			return 0;
	}
}

//! masterSwitch Slot
/*! Slot for so the QWindow class can flip the master switch.
  \param state the new state of the master switch */
void Lighting::masterSwitch(bool state)
{
	if (state)
		enable();
	else
		disable();
	emit GLDraw();
}

//! setColor Slot 1
/*! Slot so the QWindow class can change the color of light 1. */
void Lighting::setColor1()
{
	setColor(1);
}

//! setColor Slot 2
/*! Slot so the QWindow class can change the color of light 2. */
void Lighting::setColor2()
{
	setColor(2);
}

//! setColor Slot 3
/*! Slot so the QWindow class can change the color of light 3. */
void Lighting::setColor3()
{
	setColor(3);
}

//! setColor Slot 4
/*! Slot so the QWindow class can change the color of light 4. */
void Lighting::setColor4()
{
	setColor(4);
}

//! setColor Slot 5
/*! Slot so the QWindow class can change the color of light 5. */
void Lighting::setColor5()
{
	setColor(5);
}

//! lightSwitch Slot 1
/*! Slot so the QWindow class can flip the switch for light 1.
  \param state the new state of the switch */
void Lighting::lightSwitch1(int state)
{
	/* we need a switch because QT's checkbox can be "partially" checked */
	switch (state)
	{
		case Qt::Checked:
		{
			lightSwitches.light1 = ON;
			lightSwitch(true, lightSwitches.type1, 1);
			break;
		}
		case Qt::Unchecked:
		{
			lightSwitches.light1 = OFF;
			lightSwitch(false, lightSwitches.type1, 1);
			break;
		}
		default:
			break;
	}
}

//! lightSwitch Slot 2
/*! Slot so the QWindow class can flip the switch for light 2.
  \param state the new state of the switch */
void Lighting::lightSwitch2(int state)
{
	/* we need a switch because QT's checkbox can be "partially" checked */
	switch (state)
	{
		case Qt::Checked:
		{
			lightSwitches.light2 = ON;
			lightSwitch(true, lightSwitches.type2, 2);
			break;
		}
		case Qt::Unchecked:
		{
			lightSwitches.light2 = OFF;
			lightSwitch(false, lightSwitches.type2, 2);
			break;
		}
		default:
			break;
	}
}

//! lightSwitch Slot 3
/*! Slot so the QWindow class can flip the switch for light 3.
  \param state the new state of the switch */
void Lighting::lightSwitch3(int state)
{
	/* we need a switch because QT's checkbox can be "partially" checked */
	switch (state)
	{
		case Qt::Checked:
		{
			lightSwitches.light3 = ON;
			lightSwitch(true, lightSwitches.type3, 3);
			break;
		}
		case Qt::Unchecked:
		{
			lightSwitches.light3 = OFF;
			lightSwitch(false, lightSwitches.type3, 3);
			break;
		}
		default:
			break;
	}
}

//! lightSwitch Slot 4
/*! Slot so the QWindow class can flip the switch for light 4.
  \param state the new state of the switch */
void Lighting::lightSwitch4(int state)
{
	/* we need a switch because QT's checkbox can be "partially" checked */
	switch (state)
	{
		case Qt::Checked:
		{
			lightSwitches.light4 = ON;
			lightSwitch(true, lightSwitches.type4, 4);
			break;
		}
		case Qt::Unchecked:
		{
			lightSwitches.light4 = OFF;
			lightSwitch(false, lightSwitches.type4, 4);
			break;
		}
		default:
			break;
	}
}

//! lightSwitch Slot 5
/*! Slot so the QWindow class can flip the switch for light 5.
  \param state the new state of the switch */
void Lighting::lightSwitch5(int state)
{
	/* we need a switch because QT's checkbox can be "partially" checked */
	switch (state)
	{
		case Qt::Checked:
		{
			lightSwitches.light5 = ON;
			lightSwitch(true, lightSwitches.type5, 5);
			break;
		}
		case Qt::Unchecked:
		{
			lightSwitches.light5 = OFF;
			lightSwitch(false, lightSwitches.type5, 5);
			break;
		}
		default:
			break;
	}
}

//! setAngle Slot 1
/*! Slot so the QWindow class can change the spotlight angle for light 1.
  \param newAngle the new spotlight angle */
void Lighting::setAngle1(double newAngle)
{
	angles[0] = newAngle;
	setAngle(1);
}

//! setAngle Slot 2
/*! Slot so the QWindow class can change the spotlight angle for light 2.
  \param newAngle the new spotlight angle */
void Lighting::setAngle2(double newAngle)
{
	angles[1] = newAngle;
	setAngle(2);
}

//! setAngle Slot 3
/*! Slot so the QWindow class can change the spotlight angle for light 3.
  \param newAngle the new spotlight angle */
void Lighting::setAngle3(double newAngle)
{
	angles[2] = newAngle;
	setAngle(3);
}

//! setAngle Slot 4
/*! Slot so the QWindow class can change the spotlight angle for light 4.
  \param newAngle the new spotlight angle */
void Lighting::setAngle4(double newAngle)
{
	angles[3] = newAngle;
	setAngle(4);
}

//! setAngle Slot 5
/*! Slot so the QWindow class can change the spotlight angle for light 5.
  \param newAngle the new spotlight angle */
void Lighting::setAngle5(double newAngle)
{
	angles[4] = newAngle;
	setAngle(5);
}

//! setType Slot 1
/*! Slot so the QWindow class can change the type for light 1.
  \param type the new type of light */
void Lighting::setType1(unsigned short type)
{
	lightSwitches.type1 = type;
	setType(type, 1);
}

//! setType Slot 2
/*! Slot so the QWindow class can change the type for light 2.
  \param type the new type of light */
void Lighting::setType2(unsigned short type)
{
	lightSwitches.type2 = type;
	setType(type, 2);
}

//! setType Slot 3
/*! Slot so the QWindow class can change the type for light 3.
  \param type the new type of light */
void Lighting::setType3(unsigned short type)
{
	lightSwitches.type3 = type;
	setType(type, 3);
}

//! setType Slot 4
/*! Slot so the QWindow class can change the type for light 4.
  \param type the new type of light */
void Lighting::setType4(unsigned short type)
{
	lightSwitches.type4 = type;
	setType(type, 4);
}

//! setType Slot 5
/*! Slot so the QWindow class can change the type for light 5.
  \param type the new type of light */
void Lighting::setType5(unsigned short type)
{
	lightSwitches.type5 = type;
	setType(type, 5);
}

//! setX Slot 1
/*! Slot so the QWindow class can set the \f$x\f$ coordinate for light 1.
  \param newX the new \f$x\f$ coordinate */
void Lighting::setX1(double newX)
{
	coords[0][0] = newX;
	repositionLight(lightSwitches.type1, 1);
}

//! setX Slot 2
/*! Slot so the QWindow class can set the \f$x\f$ coordinate for light 2.
  \param newX the new \f$x\f$ coordinate */
void Lighting::setX2(double newX)
{
	coords[1][0] = newX;
	repositionLight(lightSwitches.type2, 2);
}

//! setX Slot 3
/*! Slot so the QWindow class can set the \f$x\f$ coordinate for light 3.
  \param newX the new \f$x\f$ coordinate */
void Lighting::setX3(double newX)
{
	coords[2][0] = newX;
	repositionLight(lightSwitches.type3, 3);
}

//! setX Slot 4
/*! Slot so the QWindow class can set the \f$x\f$ coordinate for light 4.
  \param newX the new \f$x\f$ coordinate */
void Lighting::setX4(double newX)
{
	coords[3][0] = newX;
	repositionLight(lightSwitches.type4, 4);
}

//! setX Slot 5
/*! Slot so the QWindow class can set the \f$x\f$ coordinate for light 5.
  \param newX the new \f$x\f$ coordinate */
void Lighting::setX5(double newX)
{
	coords[4][0] = newX;
	repositionLight(lightSwitches.type5, 5);
}

//! setY Slot 1
/*! Slot so the QWindow class can set the \f$y\f$ coordinate for light 1.
  \param newY the new \f$y\f$ coordinate */
void Lighting::setY1(double newY)
{
	coords[0][1] = newY;
	repositionLight(lightSwitches.type1, 1);
}

//! setY Slot 2
/*! Slot so the QWindow class can set the \f$y\f$ coordinate for light 2.
  \param newY the new \f$y\f$ coordinate */
void Lighting::setY2(double newY)
{
	coords[1][1] = newY;
	repositionLight(lightSwitches.type2, 2);
}

//! setY Slot 3
/*! Slot so the QWindow class can set the \f$y\f$ coordinate for light 3.
  \param newY the new \f$y\f$ coordinate */
void Lighting::setY3(double newY)
{
	coords[2][1] = newY;
	repositionLight(lightSwitches.type3, 3);
}

//! setY Slot 4
/*! Slot so the QWindow class can set the \f$y\f$ coordinate for light 4.
  \param newY the new \f$y\f$ coordinate */
void Lighting::setY4(double newY)
{
	coords[3][1] = newY;
	repositionLight(lightSwitches.type4, 4);
}

//! setY Slot 5
/*! Slot so the QWindow class can set the \f$y\f$ coordinate for light 5.
  \param newY the new \f$y\f$ coordinate */
void Lighting::setY5(double newY)
{
	coords[4][1] = newY;
	repositionLight(lightSwitches.type5, 5);
}

//! setZ Slot 1
/*! Slot so the QWindow class can set the \f$z\f$ coordinate for light 1.
  \param newZ the new \f$z\f$ coordinate */
void Lighting::setZ1(double newZ)
{
	coords[0][2] = newZ;
	repositionLight(lightSwitches.type1, 1);
}

//! setZ Slot 2
/*! Slot so the QWindow class can set the \f$z\f$ coordinate for light 2.
  \param newZ the new \f$z\f$ coordinate */
void Lighting::setZ2(double newZ)
{
	coords[1][2] = newZ;
	repositionLight(lightSwitches.type2, 2);
}

//! setZ Slot 3
/*! Slot so the QWindow class can set the \f$z\f$ coordinate for light 3.
  \param newZ the new \f$z\f$ coordinate */
void Lighting::setZ3(double newZ)
{
	coords[2][2] = newZ;
	repositionLight(lightSwitches.type3, 3);
}

//! setZ Slot 4
/*! Slot so the QWindow class can set the \f$z\f$ coordinate for light 4.
  \param newZ the new \f$z\f$ coordinate */
void Lighting::setZ4(double newZ)
{
	coords[3][2] = newZ;
	repositionLight(lightSwitches.type4, 4);
}

//! setZ Slot 5
/*! Slot so the QWindow class can set the \f$z\f$ coordinate for light 5.
  \param newZ the new \f$z\f$ coordinate */
void Lighting::setZ5(double newZ)
{
	coords[4][2] = newZ;
	repositionLight(lightSwitches.type5, 5);
}

//! lightSwitch Worker Function
/*! Receives data from lightSwitchN() and does the actual work of flipping the light switch.
  \param state the new state of the switch
  \param type the current type of light
  \param lightNum the light number */
void Lighting::lightSwitch(bool state, unsigned short type, unsigned short lightNum)
{
	unsigned short GLNum = lightNum - 1;
	if (state)
	{
		/* set light parameters */
		if (type != SPOTLIGHT)
			glLightfv(GL_LIGHT0 + GLNum, GL_POSITION, coords[GLNum]);
		else
		{
			glLightf(GL_LIGHT0 + GLNum, GL_SPOT_CUTOFF, angles[GLNum]);
			glLightfv(GL_LIGHT0 + GLNum, GL_SPOT_DIRECTION, coords[GLNum]);
		}
		glLightfv(GL_LIGHT0 + GLNum, GL_DIFFUSE, colors[GLNum]);
		glLightfv(GL_LIGHT0 + GLNum, GL_SPECULAR, colors[GLNum]);
		glEnable(GL_LIGHT0 + GLNum);
	}
	else
	{
		/* disable the light */
		glDisable(GL_LIGHT0 + GLNum);
	}
	emit GLDraw();
}

//! setAngle Worker Function
/*! Receives data from setAngleN() and does the actual work of changing the spotlight angle.
  \param lightNum the light number */
void Lighting::setAngle(unsigned short lightNum)
{
	unsigned short GLNum = lightNum - 1;
	glLightf(GL_LIGHT0 + GLNum, GL_SPOT_CUTOFF, angles[GLNum]);
	emit GLDraw();
}

//! setColor Worker Function
/*! Receives data from setColorN() and opens a color chooser for the user to interactively choose the light color.
  \param lightNum the light number */
void Lighting::setColor(unsigned short lightNum)
{
	unsigned short GLNum = lightNum - 1;
	QColor color = QColorDialog::getColor(Qt::white, this);
	/* if the user didn't cancel */
	if (color.isValid())
	{
		colors[GLNum][0] = color.redF();
		colors[GLNum][1] = color.greenF();
		colors[GLNum][2] = color.blueF();
		colors[GLNum][3] = 1.0;
		glLightfv(GL_LIGHT0 + GLNum, GL_DIFFUSE, colors[GLNum]);
		glLightfv(GL_LIGHT0 + GLNum, GL_SPECULAR, colors[GLNum]);
		emit GLDraw();
	}
}

//! setType Worker Function
/*! Receives data from setTypeN() and does the actual work of changing the light type.
  \param type the new light type
  \param lightNum the light number */
void Lighting::setType(unsigned short type, unsigned short lightNum)
{
	unsigned short GLNum = lightNum - 1;
	switch (type)
	{
		case DIRECTIONAL:
		{
			/* easy case */
			coords[GLNum][3] = 0.0;
			glLightfv(GL_LIGHT0 + GLNum, GL_POSITION, coords[0]);
			emit GLDraw();
			break;
		}
		case POSITIONAL:
		{
			/* we set spot light parameters back to default */
			/* otherwise this stays a spot light */
			coords[GLNum][3] = 1.0;
			GLfloat direction[] = {0.0, 0.0, -1.0};
			glLightfv(GL_LIGHT0 + GLNum, GL_POSITION, coords[GLNum]);
			glLightf(GL_LIGHT0 + GLNum, GL_SPOT_CUTOFF, 180.0);
			glLightfv(GL_LIGHT0 + GLNum, GL_SPOT_DIRECTION, direction);
			emit GLDraw();
			break;
		}
		case SPOTLIGHT:
		{
			/* the spot light always points toward the origin - for now */
			coords[GLNum][3] = 1.0;
			GLfloat direction[] = {-coords[GLNum][0], -coords[GLNum][1], -coords[GLNum][2]};
			glLightfv(GL_LIGHT0 + GLNum, GL_POSITION, coords[GLNum]);
			glLightf(GL_LIGHT0 + GLNum, GL_SPOT_CUTOFF, angles[GLNum]);
			glLightfv(GL_LIGHT0 + GLNum, GL_SPOT_DIRECTION, direction);
			emit GLDraw();
			break;
		}
		default:
			break;
	}

}

//! repositionLight Worker Function
/*! Receives data from setXN(), setYN(), or setZN() and does the actual work of moving the light around. This function is not called for the current light.
  \param type the new light type
  \param lightNum the light number */
void Lighting::repositionLight(unsigned short type, unsigned short lightNum)
{
	unsigned short GLNum = lightNum - 1;
	switch (type)
	{
		case DIRECTIONAL:
		case POSITIONAL:
		{
			/* move the light */
			glLightfv(GL_LIGHT0 + GLNum, GL_POSITION, coords[GLNum]);
			emit GLDraw();
			break;
		}
		case SPOTLIGHT:
		{
			/* the spot light always points toward the origin - for now */
			GLfloat direction[] = {-coords[GLNum][0], -coords[GLNum][1], -coords[GLNum][2]};
			glLightfv(GL_LIGHT0 + GLNum, GL_POSITION, coords[GLNum]);
			glLightfv(GL_LIGHT0 + GLNum, GL_SPOT_DIRECTION, direction);
			emit GLDraw();
			break;
		}
		default:
			break;
	}
}
