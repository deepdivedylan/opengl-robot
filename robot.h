#ifndef ROBOT_H
#define ROBOT_H

#ifdef Win32
#include <cctype>
#include <windows.h>
#endif

#ifndef MacOSX
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

#include <QActionGroup>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QGLWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include "linalg.h"

/*! \file robot.h
  \brief Main Robot Header File

  Contains all the classes, defines, and enumerations for the Qt Robot. */

//! DEG2RAD Macro
/*! Macro that converts degrees to radians. */
#define DEG2RAD(x) ((x) * (0.01745329251994329576))

//! RAD2DEG Macro
/*! Macro that converts radians to degrees. */
#define RAD2DEG(x) ((x) * (57.29577951308232087721))

//! Light Numbers
/*! Enumeration representing light numbers used by Qt. */
enum lights {NONE, LIGHT1, LIGHT2, LIGHT3, LIGHT4, LIGHT5};

//! Material Types
/*! Enumeration representing material types in the robot. */
enum materials {CARTOON, GOLD, OBSIDIAN, SILVER};

//! Switch States
/*! Enumeration representing whether a switch is on or off. */
enum states {ON, OFF};

//! Light Types
/*! Enumeration with all supported light types. */
enum types {DIRECTIONAL, POSITIONAL, SPOTLIGHT};

//! Light Related Flags
/*! A bit field with five light switches, a master switch, and five light types. */
struct switches
{
	unsigned light1: 1; /*!< Light Switch 1 */
	unsigned light2: 1; /*!< Light Switch 2 */
	unsigned light3: 1; /*!< Light Switch 3 */
	unsigned light4: 1; /*!< Light Switch 4 */
	unsigned light5: 1; /*!< Light Switch 5 */
	unsigned master: 1; /*!< Master Switch */
	unsigned type1: 2; /*!< Light Type 1 */
	unsigned type2: 2; /*!< Light Type 2 */
	unsigned type3: 2; /*!< Light Type 3 */
	unsigned type4: 2; /*!< Light Type 4 */
	unsigned type5: 2; /*!< Light Type 5 */
};


//! OpenGL Lighting Class
/*! A class with 5 fully customizable lights. Designed to interact with the QWindow class to change its parameters. */
class Lighting : public QWidget
{
	Q_OBJECT
public:
	Lighting(QWidget *parent = 0);
	~Lighting();
	bool enabled();
	void enable();
	void disable();
	unsigned short getType(unsigned short lightNum);

signals:
	//! GLDraw Signal
	/*! Signal emitted when the scene needs to be redrawn. This is needed because the Lighting class can't directly access the OpenGL scene. */
	void GLDraw();

public slots:
	/* methods that receive input from QT */
	void masterSwitch(bool state);
	void lightSwitch1(int state);
	void lightSwitch2(int state);
	void lightSwitch3(int state);
	void lightSwitch4(int state);
	void lightSwitch5(int state);
	void setAngle1(double newAngle);
	void setAngle2(double newAngle);
	void setAngle3(double newAngle);
	void setAngle4(double newAngle);
	void setAngle5(double newAngle);
	void setColor1();
	void setColor2();
	void setColor3();
	void setColor4();
	void setColor5();
	void setType1(unsigned short type);
	void setType2(unsigned short type);
	void setType3(unsigned short type);
	void setType4(unsigned short type);
	void setType5(unsigned short type);
	void setX1(double newX);
	void setX2(double newX);
	void setX3(double newX);
	void setX4(double newX);
	void setX5(double newX);
	void setY1(double newY);
	void setY2(double newY);
	void setY3(double newY);
	void setY4(double newY);
	void setY5(double newY);
	void setZ1(double newZ);
	void setZ2(double newZ);
	void setZ3(double newZ);
	void setZ4(double newZ);
	void setZ5(double newZ);

protected:
	//! Light Switch Bit Field
	/*! Bit field with the switches for all lights. */
	struct switches lightSwitches;
	//! Spotlight Angles
	/*! Pointer to an array of 5 spotlight angles. */
	GLfloat *angles;
	//! Light Colors
	/*! Pointer to an array of 5 light colors in the form \f$(r,g,b,\alpha)\f$. */
	GLfloat **colors;
	//! Light Positions
	/*! Pointer to an array of 5 light positions in the form \f$(x,y,z,w)\f$.*/
	GLfloat **coords;

private:
	/* functions that actually process QT's input */
	void lightSwitch(bool state, unsigned short type, unsigned short lightNum);
	void setAngle(unsigned short lightNum);
	void setColor(unsigned short lightNum);
	void setType(unsigned short type, unsigned short lightNum);
	void repositionLight(unsigned short type, unsigned short lightNum);
};

//! OpenGL Cube Class
/*! A class that defines a cube with texturing and lighting support. */
class Cube
{
public:
	Cube(GLdouble length, bool light=true);
	~Cube();
	void draw();
	void loadFaces(QImage **newFaces);
	void setTexturing(bool newText);
protected:
	//! Lighting Flag
	/*! Flag that indicates whether lighting is enabled. */
	bool lighting;
	//! Texturing Flag
	/*! Flag that indicates whether texturing is enabled. */
	bool texturing;
	//! Side Length
	/*! Length of one side of the cube. */
	GLdouble side;
	//! Texturing Handles
	/*! Pointer to an array of texture handles. */
	GLuint *textures;
	//! Texture Images
	/*! Pointer to an array of pointers to QImages containing OpenGL-compatible textures for the face of the cube. */
	QImage **faces;
};

//! OpenGL Cylinder Class
/*! A class that defines a cylinder with lighting support. */
class Cylinder
{
public:
	Cylinder(double radius, double height, double red, double green, double blue, double angle, Vector &axis, bool rotate=true, bool light=true);
	Cylinder(double radius, double height, double red, double green, double blue, bool light=true);
	~Cylinder();
	void build(double radius, double height, double red, double green, double blue, double angle, Vector &axis, bool rotate=true);
	void draw();
protected:
	//! Lighting Flag
	/*! Flag that indicates whether lighting is enabled. */
	bool lighting;
	//! GLU Quadric Objects
	/*! Pointer to an array of Quadric objects that make up the cylinder. */
	GLUquadricObj *qobj;
	//! OpenGL Start List Handle
	/*! Handle for using the array of Quadric objects. */
	GLuint startList;
};

//! OpenGL Robot Class
/*! A class that defines a robot with lighting and OpenGL support. This class has is Qt unaware and is the class that QRobot was designed to control. */
class Robot
{
public:
	Robot();
	~Robot();
	bool dropped();
	bool grabbed();
	bool inRange();
	void grabCube();
	void draw();
	void loadFaces(QImage **newFaces);
	GLdouble getArm();
	GLdouble getShoulder();
	GLdouble getForearm();
	GLdouble getForearmOffset();
	GLdouble getFingers();
	GLdouble getCubeHeight();
	void setArm(GLdouble newArm);
	void setShoulder(GLdouble newShoulder);
	void setMaterial(unsigned short newMat);
	void setForearm(GLdouble newForearm);
	void setForearmOffset(GLdouble newForearmOffset);
	void setDropped(bool newDrop);
	void setFingers(GLdouble newFinger);
	void setCubeHeight(GLdouble newHeight);
	void setTexturing(bool newText);

protected:
	//! Drop Flag
	/*! If true, the cube has been dropped; if false, the cube has not been dropped. */
	bool drop;
	//! Grab Flag
	/*! If true, the cube has been grabbed; if false, the cube has not been grabbed. */
	bool grab;
	//! Ready Flag
	/*! If true, the cube is ready to grab; if false, the cube is not ready to be grabbed. */
	bool ready;
	//! Material Flag
	/*! Flag containing the material the Robot is made of. */
	unsigned short robotMaterial;
	//! Arm Angle
	/*! Angle of rotation on the floor. */
	GLdouble armAngle;
	//! Finger Angle
	/*! Angle between the wrist and fingers. */
	GLdouble fingerAngle;
	//! Forearm Angle
	/*! Angle of rotation of the forearm. */
	GLdouble forearmAngle;
	//! Shoulder Angle
	/*! Angle of the shoulder with respect to the \f$y\f$ axis. */
	GLdouble shoulderAngle;
	//! Forearm Offset
	/*! Amount the forearm is extended. */
	GLdouble forearmOffset;
	//! Cube Offset Vector
	/*! Array containing the offset of the cube from its original resting place. */
	GLdouble cubeOffset[3];
	//! Cube Axis of Rotation
	/*! Array containing the axis of rotation of the cube. */
	GLdouble cubeRotation[3];
	//! Cube
	/*! Pointer to the cube itself */
	Cube *cube;
	//@{
	//! Robot Cylinders
	/*! These Cylinders that make up the building blocks of the Robot. */
	Cylinder *c1, *c2, *c3, *c4, *c5, *c6, *c7, *c8;
	//@}
	//! Cube Modelview Matrix
	/*! Matrix containing the current modelview matrix for the Cube. */
	Matrix *cubeModel;
	//! Finger Modelview Matrix
	/*! Matrix containing the current modelview matrix for the finger. */
	Matrix *fingerModel;
};

//! Qt-enabled OpenGL Robot Class
/*! A class that acts as a proxy between pure OpenGL classes such as Robot and pure Qt classes such as QWindow. QRobot sets up the initial OpenGL environment and integrates it into a Qt interface. Using Qt slots and signals, it passes data between the pure OpenGL classes and Qt classes as needed. */
class QRobot : public QGLWidget
{
	Q_OBJECT
public:
	QRobot(QWidget *parent = 0);
	~QRobot();
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	Lighting *getLights();
	QImage **getFaces();
	bool texturesLoaded();
	void dropCube();
	void loadFaces();
	void setCurrLight(unsigned short newLight);
	void setTexturing(bool newText);
	unsigned short getCurrLight();
	double getZoom();
	double getFingerAngle();
	double getDrag();
	double getMass();
	double getTemperature();

public slots:
	void GLDraw();
	void setFingerAngle(int newFingerAngle);
	void setMaterial(unsigned short newMat);
	void setMode(bool whichMode);
	void setZoom(int newZoom);
	void setDrag(double newDrag);
	void setMass(double newMass);
	void setTemperature(double newTemp);

signals:
	//! Cube Status Signal
	/*! This signal changes the status text in the QWindow. */
	void cubeGrabbed(QString);
	//! Light Position Signal
	/*! This signal changes the position of the current light in the QWindow. */
	void lightMoved(GLfloat *);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

private:
	//! Widget Width
	/*! \deprecated This variable may be removed in future revisions */
	int window_width;
	//! Widget Height
	/*! \deprecated This variable may be removed in future revisions */
	int window_height;
	//! Drag Coeffecient
	/*! Coefficient for air drag; used when dropping the Cube */
	double drag;
	//! Mass
	/*! Mass of the Cube; used when dropping the Cube */
	double mass;
	//! Temperature
	/*! Temperature of the room (affects air drag); used when dropping the Cube */
	double temperature;
	//! Finger Rotation
	/*! Angle of rotation of the fingers with respect to the wrists */
	double fingerAngle;
	//! Zoom Distance
	/*! Distance the camera is from the Robot */
	double zoomDistance;
	//! \f$x\f$ Rotation
	/*! Angle of rotation of the Robot with respect to the \f$x\f$ axis */
	double xRot;
	//! \f$y\f$ Rotation
	/*! Angle of rotation of the Robot with respect to the \f$y\f$ axis */
	double yRot;
	//! \f$z\f$ Rotation
	/*! Angle of rotation of the Robot with respect to the \f$z\f$ axis */
	double zRot;
	//! Texture Flag
	/*! If true, textures have been loaded; if false, textures failed to load */
	bool textures;
	//! View Flag
	/*! If true, the Robot is in view/rotate mode; if false, the Robot is in control mode */
	bool viewMode;
	//! Current Light
	/*! Enumeration of which light is set as the current light */
	unsigned short currLight;
	//! Current Light Coordinates
	/*! Pointer to an array of the coordinates of the current light */
	GLfloat *currLightCoords;
	//! Textures
	/*! Pointer to an array of pointers of type QImage containing OpenGL compatible texture data */
	QImage **faces;
	//! Last Position
	/*! Last position of the mouse, in window coordinates */
	QPoint lastPos;
	//! Robot
	/*! The robot itself */
	Robot *robot;
	//! Lighting
	/*! The lighting itself */
	Lighting *lights;

	void Error(char *msg);
	void drawFloor();
};

//! Qt Window Class
/*! A class that builds and drives the main user interface. QWindow is connected via signals and slots to QRobot and Lighting. */
class RobotWindow : public QWidget
{
	Q_OBJECT
public:
	RobotWindow(QWidget *parent = 0);

public slots:
	void menu1(QAction *action);
	void menu2(QAction *action);
	void menu3(QAction *action);
	void menu4(QAction *action);
	void menu5(QAction *action);
	void cubeTexture(QAction *action);
	void currentLight(QAction *action);
	void robotMaterial(QAction *action);
	void lightMoved(GLfloat *coords);

private:
	//! QRobot Object
	/*! This is the main object that gives rise to the entire Robot. */
	QRobot *qrobot;
	//! Lighting Object
	/*! This allows the QWindow to control the Lighting. */
	Lighting *lights;

	//@{
	//! Main Widgets
	/*! These widgets create the core interface which includes the movement and control widgets as well as the main containers the overall interfaces lives in. */
	QDoubleSpinBox *dragSpin, *massSpin, *tempSpin;
	QGridLayout *controlContainerBox, *controlGrid, *mainLayout;
	QGroupBox *additionalBox, *controlBox, *controlContainer, *viewControls;
	QLabel *cubeStatus, *dragLabel, *fingerLabel, *massLabel, *tempLabel, *zoomLabel;
	QRadioButton *viewRadio, *controlRadio;
	QSlider *fingerSlider, *zoomSlider;
	QVBoxLayout *additionalLayout, *viewControlsBox;
	//@}

	//@{
	//! Lighting Widgets
	/*! These widgets create the controls for the 5 lights. */
	QAction *directional1, *directional2, *directional3, *directional4, *directional5;
    	QAction *positional1, *positional2, *positional3, *positional4, *positional5;
	QAction *spotlight1, *spotlight2, *spotlight3, *spotlight4, *spotlight5;
	QActionGroup *typeGroup1, *typeGroup2, *typeGroup3, *typeGroup4, *typeGroup5;
	QCheckBox *switch1, *switch2, *switch3, *switch4, *switch5;
	QDoubleSpinBox *lightX1, *lightX2, *lightX3, *lightX4, *lightX5;
	QDoubleSpinBox *lightY1, *lightY2, *lightY3, *lightY4, *lightY5;
	QDoubleSpinBox *lightZ1, *lightZ2, *lightZ3, *lightZ4, *lightZ5;
	QDoubleSpinBox *spotAngle1, *spotAngle2, *spotAngle3, *spotAngle4, *spotAngle5;
	QGridLayout *lightGrid;
	QGroupBox *lightBox;
	QLabel *xLabel1, *xLabel2, *xLabel3, *xLabel, *xLabel4, *xLabel5;
	QLabel *yLabel1, *yLabel2, *yLabel3, *yLabel, *yLabel4, *yLabel5;
	QLabel *zLabel1, *zLabel2, *zLabel3, *zLabel, *zLabel4, *zLabel5;
	QLabel *angleLabel1, *angleLabel2, *angleLabel3, *angleLabel4, *angleLabel5;
	QMenu *lightMenu1, *lightMenu2, *lightMenu3, *lightMenu4, *lightMenu5;
	QPushButton *colorButton1, *colorButton2, *colorButton3, *colorButton4, *colorButton5;
   	QPushButton *lightType1, *lightType2, *lightType3, *lightType4, *lightType5;
	//@}

	//@{
	//! Visual Effects Widgets
	/*! These widgets create the controls that allow the user to change textures and materials. */
	QAction *noLight, *currLight1, *currLight2, *currLight3, *currLight4, *currLight5;
	QAction *cartoon, *gold, *obsidian, *silver;
	QAction *classic, *dice;
	QActionGroup *currLightGroup, *materialGroup, *textureGroup;
	QGroupBox *visualBox;
	QVBoxLayout *visualLayout;
	QMenu *currLightMenu, *materialMenu, *textureMenu;
	QPushButton *currLight, *material, *texture;
	//@}

	/* helper functions */
	void mainWidgets();
	void lightWidgets();
	void light1();
	void light2();
	void light3();
	void light4();
	void light5();
	void packLights();
	void packMain();
	void physics();
	void viewControl();
	void visual();
	void visualWidgets();
};

#endif
