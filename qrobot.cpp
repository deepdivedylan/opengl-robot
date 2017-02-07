#include "robot.h"
#include <QtOpenGL>
#include <cmath>

#define INITIAL_WINDOW_SIZE 500

//! QRobot Constructor
/*! Allocates objects needed by QRobot. Also sets up initial parameters.
  \param parent parent QWidget; automatically handled by Qt */
QRobot::QRobot(QWidget *parent) : QGLWidget(parent)
{
	window_width = INITIAL_WINDOW_SIZE, window_height = INITIAL_WINDOW_SIZE;
	zoomDistance = 300.0;
	drag = 1.5;
	mass = 100.0;
	temperature = 20.0;
	xRot = 0.0;
	yRot = 0.0;
	zRot = 0.0;
	lights = new Lighting();
	connect(lights, SIGNAL(GLDraw()), this, SLOT(GLDraw()));
	textures = false;
	viewMode = true;
	currLight = NONE;
	currLightCoords = new GLfloat[4];
	faces = new QImage*[6];
	for (unsigned short i=0; i<6; i++)
		faces[i] = new QImage;
	loadFaces();
}

//! QRobot Destructor
/*! Frees allocated objects needed by QRobot. */
QRobot::~QRobot()
{
	delete robot;
	delete lights;
	delete currLightCoords;
	for (unsigned short i=0; i<6; i++)
		delete faces[i];
	delete faces;
}

//! Sets Minimum Size
/*! Sets the minimum size for the widget. Currently, this is \f$500\times500\f$. QWindow sets the size statically to \f$500\times500\f$ anyway. This function exists mostly to keep Qt happy. */
QSize QRobot::minimumSizeHint() const
{
	return QSize(500, 500);
}

//! Sets Optimum Size
/*! Sets the optimum size for the widget. Currently, this is \f$500\times500\f$. QWindow sets the size statically to \f$500\times500\f$ anyway. This function exists mostly to keep Qt happy. */
QSize QRobot::sizeHint() const
{
	return QSize(500, 500);
}

//! Get Lights
/*! Returns a pointer to the Lighting system. This is used to send QWindow control over the Lighting.
  \return pointer to the lights */
Lighting *QRobot::getLights()
{
	return lights;
}

//! Get Textures
/*! Returns Pointer to an array of pointers of type QImage containing OpenGL compatible texture data. This is used to send the Cube class the texture data it needs.
  \return pointer to the array of pointers */
QImage **QRobot::getFaces()
{
	return faces;
}

//! Get Texture Flag
/*! Returns the texture flag. This flag is true when loadFaces() exits sucessfully. It is false otherwise. This method is used to tell Cube and QWindow as to whether the textures are ready.
  \return true if the textures are ready, false if not */
bool QRobot::texturesLoaded()
{
	return textures;
}

//! Start Cube Free Fall
/*! This method simulates the Cube in free fall, taking into account all physical parameters set by the user. Special thanks to Brian Carter. */
void QRobot::dropCube()
{
	/* rho is the density of the air and alpha is 1/2 rho times the cross sectional area */
	const double rho = 101325.0 / (287.05 * (temperature + 273.15));
	const double alpha = 0.5 * rho * 25.0;
	const double dt = 0.001, g = 9.80665;
	double h = robot->getCubeHeight();
	double a = g, dv, v_old = 0.0, v = 0.0, dx, x_old = 0.0, x = 0.0, t = 0.0;

	while (x < h)
	{
		a = ((mass * g) - (alpha * pow(v_old, 2.0))) / mass;
		dv = a * t;
		v = v_old + dv;
		dx = v * t;
		x = x_old + dx;
		v_old = v;
		x_old = x;
		t += dt;
		robot->setCubeHeight(h - x);
		updateGL();
	}
	robot->setCubeHeight(0.0);
	robot->setDropped(false);
}

//! Load Cube Textures
/*! This method reads \c textures/diceN.png (for \f$n=1,2,\cdots,6\f$) and converts them from PNG to OpenGL format. The OpenGL images are then stored in the faces array. */
void QRobot::loadFaces()
{
	QImage buffer;
	QString fileName;
	for (unsigned short i=0; i<6; i++)
	{
		fileName = "textures/dice";
		fileName += ('1' + i);
		fileName += ".png";
		if (buffer.load(fileName))
			(*faces[i]) = QGLWidget::convertToGLFormat(buffer);
		else
		{
			fileName = "Unable to load texture image ";
			fileName += ('1' + i);
			fileName += '.';
			Error((char *)fileName.toStdString().data());
			return;
		}
	}
	textures = true;
}

//! Set Current Light
/*! This method changes the current light and redraws the scene.
  \param newLight the enumerated light number */
void QRobot::setCurrLight(unsigned short newLight)
{
	currLight = newLight;
	updateGL();
	if (currLight != NONE)
		emit lightMoved(currLightCoords);

}

//! Set Texturing Flag
/*! This method passes the texturing flag to the Robot which will in turn pass it to the Cube. This flag is true if loadFaces() is successful and is false otherwise.
  \param newText the flag state
  \sa loadFaces() */
void QRobot::setTexturing(bool newText)
{
	robot->setTexturing(newText);
	updateGL();
}

//! Get Current Light
/*! This method returns an emuerated number representing the current light
  \return current light */
unsigned short QRobot::getCurrLight()
{
	return currLight;
}

//! Get Zoom Distance
/*! This method returns the distance from the camera to the Robot.
  \return the distance */
double QRobot::getZoom()
{
	return zoomDistance;
}

//! GLDraw Slot
/*! This slot is used by Lighting. Lighting will raise the GLDraw() signal when the scene needs to be redrawn. */
void QRobot::GLDraw()
{
	updateGL();
}

//! Finger Angle Slot
/*! This slot passes the new finger angle to the Robot.
  \param newFingerAngle the new angle */
void QRobot::setFingerAngle(int newFingerAngle)
{
	robot->setFingers((GLdouble)newFingerAngle / 10.0);
	updateGL();
}

//! Material Slot
/*! This slot passes the new material to the Robot.
  \param newMat the new material */
void QRobot::setMaterial(unsigned short newMat)
{
	robot->setMaterial(newMat);
	updateGL();
}

//! Mode Slot
/*! This is used by QWindow to set whether the Robot is in view/rotate mode or control mode.
  \param whichMode the new mode */
void QRobot::setMode(bool whichMode)
{
	viewMode = whichMode;
}

//! Zoom Slot
/*! This slot is used by QWindow to set the distance from the camera to the Robot.
  \param newZoom the new distance */
void QRobot::setZoom(int newZoom)
{
	zoomDistance = newZoom;
	resizeGL(window_width, window_height);
	updateGL();
}

//! Drag Slot
/*! This slot is used by QWindow to set the drag coefficient of the air.
  \param newDrag the new coefficient */
void QRobot::setDrag(double newDrag)
{
	drag = newDrag;
}

//! Mass Slot
/*! This slot is used by QWindow to set the mass of the Cube.
  \param newMass the new mass */
void QRobot::setMass(double newMass)
{
	mass = newMass;
}

//! Temperature Slot
/*! This slot is used by QWindow to set the temperature of the air.
  \param newTemp the new temperature */
void QRobot::setTemperature(double newTemp)
{
	temperature = newTemp;
}

//! OpenGL Setup Method
/*! This method is overloaded from QGLWidget and is used to setup the initial OpenGL environment. The constructors from Robot and Lighting is all that's really needed here. */
void QRobot::initializeGL()
{
	robot=new Robot();
	robot->loadFaces(faces);
	lights->enable();
}

//! Resize Event Handler
/*! This method is overloaded from QGLWidget and is called whenever the widget is resized (programatically disabled) or when the user zooms in or out.
  \param w new widget width
  \param h new widget height */
void QRobot::resizeGL(int w, int h)
{
	window_width = w;
	window_height = h;
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 1.0, zoomDistance);
	gluLookAt(0.0, -zoomDistance/2.0, 30.0,
			  0.0, 0.0, 0.0,
			  0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

//! OpenGL Redraw Method
/*! This method is overloaded from QGLWidget and is called whenever updateGL() is called or whenever the GLDraw signal is received from Lighting. This method will also move the current light in unison with the camera. */
void QRobot::paintGL()
{
	/* save old flag states */
	bool oldGrab = robot->grabbed(), oldRange = robot->inRange(), oldDrop = robot->dropped();

	/* clear the screen and rotate the world */
	glClearColor(0.0, 0.8, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRotated(xRot, 1.0, 0.0, 0.0);
	glRotated(yRot, 0.0, 1.0, 0.0);
	glRotated(zRot, 0.0, 0.0, 1.0);
	if (currLight != NONE)
	{
		try
		{
			/* C = M^-1 * P * O
			   where...
			   	C is the point of the camera in model coordinates
				M is the modelview matrix
				P is the projection matrix
				O is the origin (i.e. O = [0 0 0 1]^T */
			double values[16];
			glGetDoublev(GL_PROJECTION_MATRIX, values);
			Matrix projection(values, 16);
			glGetDoublev(GL_MODELVIEW_MATRIX, values);
			Matrix modelview(values, 16);
			Matrix transformation(4, 4);
			Matrix camera(4, 1), origin(4, 1);
			origin[3][0] = 1.0;
			transformation = modelview.inverse() * projection;
			camera = transformation * origin;
			currLightCoords[0] = camera[0][0];
			currLightCoords[1] = camera[1][0];
			currLightCoords[2] = camera[2][0];
			currLightCoords[3] = 1.0;
			glLightfv(GL_LIGHT0 + (currLight - 1), GL_POSITION, currLightCoords);
		}
		catch (LinAlgException e)
		{
			Error(e.what());
		}
	}
	/* draw the floor and run the robot/cube through its paces */
	glDisable(GL_DEPTH_TEST);
	drawFloor();
	glEnable(GL_DEPTH_TEST);
	robot->draw();
	robot->grabCube();
	/* cube was dropped */
	if (robot->dropped() && oldDrop != robot->dropped())
		dropCube();
	/* cube was either recently grabbed or dropped */
	if (oldGrab != robot->grabbed())
	{
		if (robot->grabbed())
			emit cubeGrabbed(tr("Cube grabbed."));
		else
			emit cubeGrabbed(tr("Cube is on the floor."));
	}
	/* cube just came in or went out of range */
	if (oldRange != robot->inRange())
	{
		if (robot->inRange())
		{
			emit cubeGrabbed(tr("Cube is ready to grab."));
			QMessageBox::information(this, tr("Cube in Range"), tr("The cube is range. Just close the claws to grab it."));
		}
		else
			emit cubeGrabbed(tr("Cube is on the floor."));
	}
	swapBuffers();
}

//! Mouse Click Event Handler
/*! This method is overloaded from QGLWidget and handles single clicks in the scence. Currently, all it does is save the coordinates of the click.
  \param event QMouseEvent data from Qt */
void QRobot::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
	if (!viewMode && (event->buttons() & Qt::MidButton))
		Error((char  *)"The forearm and wrist are now controlled by the left button.");
}

//! Mouse Drag Handler
/*! This method is overloaded from QGLWidget and handles mouse drags in the scene. This method handles both rotation in view mode as well as movement in control mode.
  \param event QMouseEvent data from Qt */
void QRobot::mouseMoveEvent(QMouseEvent *event)
{

	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	/* rotate the world */
	if (viewMode && (event->buttons() & Qt::LeftButton))
	{
		xRot += (dx / 100.0);
		yRot += (dy / 100.0);
	}
	/* rotate the world about the y axis */
	if (viewMode && (event->buttons() & Qt::RightButton))
	{
		xRot += (dx / 100.0);
		zRot += (dy / 100.0);
	}
	/* move the arm & shoulder */
	if (!viewMode && (event->buttons() & Qt::LeftButton))
	{
		robot->setArm(robot->getArm() + (dx / 100.0));
		robot->setShoulder(robot->getShoulder() + (dy / 100.0));
	}
	/* move the forearm & rotate the hand */
	if (!viewMode && (event->buttons() & Qt::RightButton))
	{
		robot->setForearm(robot->getForearm() + (dy / 100.0));
		robot->setForearmOffset(robot->getForearmOffset() + (dx / 100.0));
	}
	updateGL();
}

//! Mouse Release Handler
/*! This method is overloaded from QGLWidget and handles the end of a mouse drag in the scene. Currently, this updates the position of the current light on the screen.
  \param event QMouseEvent data from Qt */
void QRobot::mouseReleaseEvent(QMouseEvent *event)
{
	if (currLight != NONE)
		emit lightMoved(currLightCoords);
}

//! Get Finger Angle
/*! This method is used by QWindow to retreive the angle between the wrist and fingers.
  \return the angle */
double QRobot::getFingerAngle()
{
	return (robot->getFingers() * 10.0);
}

//! Get Drag Coefficient
/*! This method is used by QWindow to retreive the drag coefficient.
  \return the drag coefficient */
double QRobot::getDrag()
{
	return drag;
}

//! Get Mass
/*! This method is used by QWindow to retreive the mass of the Cube.
  \return the mass */
double QRobot::getMass()
{
	return mass;
}

//! Get Temperature
/*! This method is used by QWindow to retreive the air temperature (affects air drag).
  \return the air temperature */
double QRobot::getTemperature()
{
	return temperature;
}

//! Error Message Function
/*! This method displays a Qt-style error message in case (God forbid) an error occurs.
  \param msg the error message */
void QRobot::Error(char *msg)
{
	QMessageBox::critical(this, "QT Robot Arm", tr(msg));
}

//! Draw Floor Function
/*! This method draws the floor. It is called by paintGL().
  \sa paintGL() */
void QRobot::drawFloor()
{
	/* cartoon style parameters */
	GLfloat black[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat cartoon_ambient[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat cartoon_diffuse[] = {0.8, 0.8, 0.8, 1.0};
	GLfloat cartoon_shininess = 16.0;
	GLdouble floorSize = zoomDistance / 2.0;

	glPushMatrix();
	/* foce the floor to be drawn in the cartoon style */
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_AMBIENT, cartoon_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cartoon_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glMaterialf(GL_FRONT, GL_SHININESS, cartoon_shininess);
	glEnable(GL_COLOR_MATERIAL);

	/* define the floor */
	glBegin(GL_QUADS);
		glNormal3d(0.0, 1.0, 0.0);
		glColor3d(0.0, 0.5, 0.25);
		glVertex3d(floorSize, 0.0, 0.0);
		glVertex3d(0.0, floorSize, 0.0);
		glVertex3d(-floorSize, 0.0, 0.0);
		glVertex3d(0.0, -floorSize, 0.0);
	glEnd();
	glPopMatrix();
}
