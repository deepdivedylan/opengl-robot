#include "robot.h"
#include <QMenu>
#include <QMessageBox>

//! RobotWindow Constructor
/*! Allocates objects needed by RobotWindow and calls helper functions to create, pack and connect widgets.
  \param parent parent widget - automatically handled by Qt */
RobotWindow::RobotWindow(QWidget *parent) : QWidget(parent)
{
	/* the main objects this interface controls */
	qrobot = new QRobot();
	qrobot->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	lights = qrobot->getLights();
	connect(qrobot, SIGNAL(lightMoved(GLfloat *)), this, SLOT(lightMoved(GLfloat *)));

	/* construct widgets */
	mainWidgets();
	lightWidgets();
	visualWidgets();

	/* set widget options */
	viewControl();
	physics();
	light1();
	light2();
	light3();
	light4();
	light5();
	visual();

	/* pack the final interface */
	packLights();
	packMain();
}

//! Texture Slot
/*! Receives user input and changes the texture of the Cube.
  \param action the user's choice */
void RobotWindow::cubeTexture(QAction *action)
{
	QString actionText = action->text();
	action->setChecked(true);
	if (actionText == "Classic")
		qrobot->setTexturing(false);
	else if (actionText == "Dice")
	{
		if (qrobot->texturesLoaded())
			qrobot->setTexturing(true);
		else
		{
			classic->setChecked(true);
			QMessageBox::critical(this, "QT Robot Arm", tr("Option not available since textures could not be loaded."));
		}
	}
	else
		return;
}

//! Current Light Slot
/*! Receives user input and changes the current light.
  \param action the user's choice */
void RobotWindow::currentLight(QAction *action)
{
	unsigned short oldLight = qrobot->getCurrLight();
	QString actionText = action->text();
	action->setChecked(true);

	/* re-enable previously disabled controls */
	switch (oldLight)
	{
		case LIGHT5:
		{
			lightType5->setEnabled(true);
			xLabel5->setEnabled(true);
			yLabel5->setEnabled(true);
			zLabel5->setEnabled(true);
			lightX5->setEnabled(true);
			lightY5->setEnabled(true);
			lightZ5->setEnabled(true);
			break;
		}
		case LIGHT4:
		{
			lightType4->setEnabled(true);
			xLabel4->setEnabled(true);
			yLabel4->setEnabled(true);
			zLabel4->setEnabled(true);
			lightX4->setEnabled(true);
			lightY4->setEnabled(true);
			lightZ4->setEnabled(true);
			break;
		}
		case LIGHT3:
		{
			lightType3->setEnabled(true);
			xLabel3->setEnabled(true);
			yLabel3->setEnabled(true);
			zLabel3->setEnabled(true);
			lightX3->setEnabled(true);
			lightY3->setEnabled(true);
			lightZ3->setEnabled(true);
			break;
		}
		case LIGHT2:
		{
			lightType2->setEnabled(true);
			xLabel2->setEnabled(true);
			yLabel2->setEnabled(true);
			zLabel2->setEnabled(true);
			lightX2->setEnabled(true);
			lightY2->setEnabled(true);
			lightZ2->setEnabled(true);
			break;
		}
		case LIGHT1:
		{
			lightType1->setEnabled(true);
			xLabel1->setEnabled(true);
			yLabel1->setEnabled(true);
			zLabel1->setEnabled(true);
			lightX1->setEnabled(true);
			lightY1->setEnabled(true);
			lightZ1->setEnabled(true);
			break;
		}
		default:
			break;
	}

	/* the user shouldn't be able to manually change the current light's position */
	/* instead, they can rotate the camera */
	if (actionText == "(None)")
		qrobot->setCurrLight(NONE);
	else if (actionText == "Light 1")
	{
		if (lights->getType(LIGHT1) == DIRECTIONAL)
		{
			QMessageBox::information(this, "QT Robot Arm", tr("You cannot have a directional light as a current light. Changing the light to a positional light."));
			positional1->setChecked(true);
			lights->setType1(POSITIONAL);
		}
		qrobot->setCurrLight(LIGHT1);
		lightType1->setDisabled(true);
		xLabel1->setDisabled(true);
		yLabel1->setDisabled(true);
		zLabel1->setDisabled(true);
		lightX1->setDisabled(true);
		lightY1->setDisabled(true);
		lightZ1->setDisabled(true);
	}
	else if (actionText == "Light 2")
	{
		if (lights->getType(LIGHT2) == DIRECTIONAL)
		{
			QMessageBox::information(this, "QT Robot Arm", tr("You cannot have a directional light as a current light. Changing the light to a positional light."));
			positional2->setChecked(true);
			lights->setType2(POSITIONAL);
		}
		qrobot->setCurrLight(LIGHT2);
		lightType2->setDisabled(true);
		xLabel2->setDisabled(true);
		yLabel2->setDisabled(true);
		zLabel2->setDisabled(true);
		lightX2->setDisabled(true);
		lightY2->setDisabled(true);
		lightZ2->setDisabled(true);
	}
	else if (actionText == "Light 3")
	{
		if (lights->getType(LIGHT3) == DIRECTIONAL)
		{
			QMessageBox::information(this, "QT Robot Arm", tr("You cannot have a directional light as a current light. Changing the light to a positional light."));
			positional3->setChecked(true);
			lights->setType3(POSITIONAL);
		}
		qrobot->setCurrLight(LIGHT3);
		lightType3->setDisabled(true);
		xLabel3->setDisabled(true);
		yLabel3->setDisabled(true);
		zLabel3->setDisabled(true);
		lightX3->setDisabled(true);
		lightY3->setDisabled(true);
		lightZ3->setDisabled(true);
	}
	else if (actionText == "Light 4")
	{
		if (lights->getType(LIGHT4) == DIRECTIONAL)
		{
			QMessageBox::information(this, "QT Robot Arm", tr("You cannot have a directional light as a current light. Changing the light to a positional light."));
			positional4->setChecked(true);
			lights->setType4(POSITIONAL);
		}
		qrobot->setCurrLight(LIGHT4);
		lightType4->setDisabled(true);
		xLabel4->setDisabled(true);
		yLabel4->setDisabled(true);
		zLabel4->setDisabled(true);
		lightX4->setDisabled(true);
		lightY4->setDisabled(true);
		lightZ4->setDisabled(true);
	}
	else if (actionText == "Light 5")
	{
		if (lights->getType(LIGHT5) == DIRECTIONAL)
		{
			QMessageBox::information(this, "QT Robot Arm", tr("You cannot have a directional light as a current light. Changing the light to a positional light."));
			positional5->setChecked(true);
			lights->setType5(POSITIONAL);
		}
		qrobot->setCurrLight(LIGHT5);
		lightType5->setDisabled(true);
		xLabel5->setDisabled(true);
		yLabel5->setDisabled(true);
		zLabel5->setDisabled(true);
		lightX5->setDisabled(true);
		lightY5->setDisabled(true);
		lightZ5->setDisabled(true);
	}
	else
		return;
}

//! Material Slot
/*! Receives user input and changes the material the Robot is made of.
  \param action the user's choice */
void RobotWindow::robotMaterial(QAction *action)
{
	QString actionText = action->text();
	action->setChecked(true);
	if (actionText == "Cartoon")
		qrobot->setMaterial(CARTOON);
	else if (actionText == "Gold")
		qrobot->setMaterial(GOLD);
	else if (actionText == "Obsidian")
		qrobot->setMaterial(OBSIDIAN);
	else if (actionText == "Silver")
		qrobot->setMaterial(SILVER);
	else
		return;
}

//! Light Moved Slot
/*! Receives the updated coordinates of the current light from QRobot and updates the user interface.
  \param coords the new position */
void RobotWindow::lightMoved(GLfloat *coords)
{
	switch(qrobot->getCurrLight())
	{
		case LIGHT5:
		{
			lightX5->setValue(coords[0]);
			lightY5->setValue(coords[1]);
			lightZ5->setValue(coords[2]);
			break;
		}
		case LIGHT4:
		{
			lightX4->setValue(coords[0]);
			lightY4->setValue(coords[1]);
			lightZ4->setValue(coords[2]);
			break;
		}
		case LIGHT3:
		{
			lightX3->setValue(coords[0]);
			lightY3->setValue(coords[1]);
			lightZ3->setValue(coords[2]);
			break;
		}
		case LIGHT2:
		{
			lightX2->setValue(coords[0]);
			lightY2->setValue(coords[1]);
			lightZ2->setValue(coords[2]);
			break;
		}
		case LIGHT1:
		{
			lightX1->setValue(coords[0]);
			lightY1->setValue(coords[1]);
			lightZ1->setValue(coords[2]);
			break;
		}
		default:
			break;
	}
}

//! Light 1 Slot
/*! Receives user input and changes the type of light 1.
  \param action the user's choice */
void RobotWindow::menu1(QAction *action)
{
	QString actionText = action->text();
	action->setChecked(true);
	if (actionText == "Directional")
	{
		lights->setType1(DIRECTIONAL);
		lights->setAngle1(45.0);
		angleLabel1->setDisabled(true);
		spotAngle1->setDisabled(true);
	}
	else if (actionText == "Positional")
	{
		lights->setType1(POSITIONAL);
		lights->setAngle1(45.0);
		angleLabel1->setDisabled(true);
		spotAngle1->setDisabled(true);
	}
	else if (actionText == "Spotlight")
	{
		lights->setType1(SPOTLIGHT);
		lights->setAngle1(45.0);
		angleLabel1->setDisabled(false);
		spotAngle1->setValue(45.0);
		spotAngle1->setDisabled(false);
	}
	else
		return;
}

//! Light 2 Slot
/*! Receives user input and changes the type of light 2.
  \param action the user's choice */
void RobotWindow::menu2(QAction *action)
{
	QString actionText = action->text();
	action->setChecked(true);
	if (actionText == "Directional")
	{
		lights->setType2(DIRECTIONAL);
		lights->setAngle2(45.0);
		angleLabel2->setDisabled(true);
		spotAngle2->setDisabled(true);
	}
	else if (actionText == "Positional")
	{
		lights->setType2(POSITIONAL);
		lights->setAngle2(45.0);
		angleLabel2->setDisabled(true);
		spotAngle2->setDisabled(true);
	}
	else if (actionText == "Spotlight")
	{
		lights->setType2(SPOTLIGHT);
		lights->setAngle2(45.0);
		angleLabel2->setDisabled(false);
		spotAngle2->setValue(45.0);
		spotAngle2->setDisabled(false);
	}
	else
		return;
}

//! Light 3 Slot
/*! Receives user input and changes the type of light 3.
  \param action the user's choice */
void RobotWindow::menu3(QAction *action)
{
	QString actionText = action->text();
	action->setChecked(true);
	if (actionText == "Directional")
	{
		lights->setType3(DIRECTIONAL);
		lights->setAngle3(45.0);
		angleLabel3->setDisabled(true);
		spotAngle3->setDisabled(true);
	}
	else if (actionText == "Positional")
	{
		lights->setType3(POSITIONAL);
		lights->setAngle3(45.0);
		angleLabel3->setDisabled(true);
		spotAngle3->setDisabled(true);
	}
	else if (actionText == "Spotlight")
	{
		lights->setType3(SPOTLIGHT);
		lights->setAngle3(45.0);
		angleLabel3->setDisabled(false);
		spotAngle3->setValue(45.0);
		spotAngle3->setDisabled(false);
	}
	else
		return;
}

//! Light 4 Slot
/*! Receives user input and changes the type of light 4.
  \param action the user's choice */
void RobotWindow::menu4(QAction *action)
{
	QString actionText = action->text();
	action->setChecked(true);
	if (actionText == "Directional")
	{
		lights->setType4(DIRECTIONAL);
		lights->setAngle4(45.0);
		angleLabel4->setDisabled(true);
		spotAngle4->setDisabled(true);
	}
	else if (actionText == "Positional")
	{
		lights->setType4(POSITIONAL);
		lights->setAngle4(45.0);
		angleLabel4->setDisabled(true);
		spotAngle4->setDisabled(true);
	}
	else if (actionText == "Spotlight")
	{
		lights->setType4(SPOTLIGHT);
		lights->setAngle4(45.0);
		angleLabel4->setDisabled(false);
		spotAngle4->setValue(45.0);
		spotAngle4->setDisabled(false);
	}
	else
		return;
}

//! Light 5 Slot
/*! Receives user input and changes the type of light 5.
  \param action the user's choice */
void RobotWindow::menu5(QAction *action)
{
	QString actionText = action->text();
	action->setChecked(true);
	if (actionText == "Directional")
	{
		lights->setType5(DIRECTIONAL);
		lights->setAngle5(45.0);
		angleLabel5->setDisabled(true);
		spotAngle5->setDisabled(true);
	}
	else if (actionText == "Positional")
	{
		lights->setType5(POSITIONAL);
		lights->setAngle5(45.0);
		angleLabel5->setDisabled(true);
		spotAngle5->setDisabled(true);
	}
	else if (actionText == "Spotlight")
	{
		lights->setType5(SPOTLIGHT);
		lights->setAngle5(45.0);
		angleLabel5->setDisabled(true);
		spotAngle5->setValue(45.0);
		spotAngle5->setDisabled(true);
	}
	else
		return;
}

//! Main Widget Helper Function
/*! Allocates all the widgets in the main group. */
void RobotWindow::mainWidgets()
{
	/* control/view mode controls */
	zoomLabel = new QLabel(tr("Zoom"));
	fingerLabel = new QLabel(tr("Finger Angle"));
	viewControls = new QGroupBox(tr("Robot View Controls"));
	viewRadio = new QRadioButton(tr("&View Mode"));
	controlRadio = new QRadioButton(tr("&Control Mode"));
	zoomSlider = new QSlider(Qt::Horizontal);
	fingerSlider = new QSlider(Qt::Horizontal);
	cubeStatus = new QLabel(tr("Cube is on the floor."));

	/* main angle controls */
	controlBox = new QGroupBox(tr("Physics Controls"));
	tempLabel = new QLabel(tr("Temperature (C)"));
	massLabel = new QLabel(tr("Mass"));
	dragLabel = new QLabel(tr("Drag Coefficient"));

	/* spin boxes */
	tempSpin = new QDoubleSpinBox;
	massSpin = new QDoubleSpinBox;
	dragSpin = new QDoubleSpinBox;

	/* containers */
	additionalBox = new QGroupBox(tr("Additional Controls"));
	additionalLayout = new QVBoxLayout;
	controlGrid = new QGridLayout;
	controlContainer = new QGroupBox(tr("Robot Controls"));
	controlContainerBox = new QGridLayout;
	mainLayout = new QGridLayout;
	viewControlsBox = new QVBoxLayout;
}

//! Light Widget Helper Function
/*! Allocates all objects in the Lighting group. */
void RobotWindow::lightWidgets()
{
	/* lighting controls */
	lightBox = new QGroupBox(tr("Lighting Controls"));
	lightGrid = new QGridLayout;

	/* light 1 */
	lightType1 = new QPushButton(tr("Light 1"));
	lightMenu1 = new QMenu(this);
	directional1 = new QAction(tr("Directional"), lightType1);
	positional1 = new QAction(tr("Positional"), lightType1);
	spotlight1 = new QAction(tr("Spotlight"), lightType1);
	typeGroup1 = new QActionGroup(lightType1);
	switch1 = new QCheckBox(tr("On"));
	xLabel1 = new QLabel(tr("X1"));
	yLabel1 = new QLabel(tr("Y1"));
	zLabel1 = new QLabel(tr("Z1"));
	lightX1 = new QDoubleSpinBox;
	lightY1 = new QDoubleSpinBox;
	lightZ1 = new QDoubleSpinBox;
	angleLabel1 = new QLabel(tr("Spotlight Angle"));
	spotAngle1 = new QDoubleSpinBox;
	colorButton1 = new QPushButton(tr("Set Light Color"));

	/* light 2 */
	lightType2 = new QPushButton(tr("Light 2"));
	lightMenu2 = new QMenu(this);
	directional2 = new QAction(tr("Directional"), lightType2);
	positional2 = new QAction(tr("Positional"), lightType2);
	spotlight2 = new QAction(tr("Spotlight"), lightType2);
	typeGroup2 = new QActionGroup(lightType2);
	switch2 = new QCheckBox(tr("On"));
	xLabel2 = new QLabel(tr("X2"));
	yLabel2 = new QLabel(tr("Y2"));
	zLabel2 = new QLabel(tr("Z2"));
	lightX2 = new QDoubleSpinBox;
	lightY2 = new QDoubleSpinBox;
	lightZ2 = new QDoubleSpinBox;
	angleLabel2 = new QLabel(tr("Spotlight Angle"));
	spotAngle2 = new QDoubleSpinBox;
	colorButton2 = new QPushButton(tr("Set Light Color"));

	/* light 3 */
	lightType3 = new QPushButton(tr("Light 3"));
	lightMenu3 = new QMenu(this);
	directional3 = new QAction(tr("Directional"), lightType3);
	positional3 = new QAction(tr("Positional"), lightType3);
	spotlight3 = new QAction(tr("Spotlight"), lightType3);
	typeGroup3 = new QActionGroup(lightType3);
	switch3 = new QCheckBox(tr("On"));
	xLabel3 = new QLabel(tr("X3"));
	yLabel3 = new QLabel(tr("Y3"));
	zLabel3 = new QLabel(tr("Z3"));
	lightX3 = new QDoubleSpinBox;
	lightY3 = new QDoubleSpinBox;
	lightZ3 = new QDoubleSpinBox;
	angleLabel3 = new QLabel(tr("Spotlight Angle"));
	spotAngle3 = new QDoubleSpinBox;
	colorButton3 = new QPushButton(tr("Set Light Color"));

	/* light 4 */
	lightType4 = new QPushButton(tr("Light 4"));
	lightMenu4 = new QMenu(this);
	directional4 = new QAction(tr("Directional"), lightType4);
	positional4 = new QAction(tr("Positional"), lightType4);
	spotlight4 = new QAction(tr("Spotlight"), lightType4);
	typeGroup4 = new QActionGroup(lightType4);
	switch4 = new QCheckBox(tr("On"));
	xLabel4 = new QLabel(tr("X4"));
	yLabel4 = new QLabel(tr("Y4"));
	zLabel4 = new QLabel(tr("Z4"));
	lightX4 = new QDoubleSpinBox;
	lightY4 = new QDoubleSpinBox;
	lightZ4 = new QDoubleSpinBox;
	angleLabel4 = new QLabel(tr("Spotlight Angle"));
	spotAngle4 = new QDoubleSpinBox;
	colorButton4 = new QPushButton(tr("Set Light Color"));

	/* light 5 */
	lightType5 = new QPushButton(tr("Light 5"));
	lightMenu5 = new QMenu(this);
	directional5 = new QAction(tr("Directional"), lightType5);
	positional5 = new QAction(tr("Positional"), lightType5);
	spotlight5 = new QAction(tr("Spotlight"), lightType5);
	typeGroup5 = new QActionGroup(lightType5);
	switch5 = new QCheckBox(tr("On"));
	xLabel5 = new QLabel(tr("X5"));
	yLabel5 = new QLabel(tr("Y5"));
	zLabel5 = new QLabel(tr("Z5"));
	lightX5 = new QDoubleSpinBox;
	lightY5 = new QDoubleSpinBox;
	lightZ5 = new QDoubleSpinBox;
	angleLabel5 = new QLabel(tr("Spotlight Angle"));
	spotAngle5 = new QDoubleSpinBox;
	colorButton5 = new QPushButton(tr("Set Light Color"));
}

//! Light 1 Helper Function
/*! Sets options for the widgets that control light 1. */
void RobotWindow::light1()
{
	/* light 1 control options */
	lightBox->setCheckable(true);
	lightBox->setChecked(true);
	lightMenu1->addAction(directional1);
	lightMenu1->addAction(positional1);
	lightMenu1->addAction(spotlight1);
	typeGroup1->addAction(directional1);
	typeGroup1->addAction(positional1);
	typeGroup1->addAction(spotlight1);
	typeGroup1->setExclusive(true);
	directional1->setCheckable(true);
	positional1->setCheckable(true);
	spotlight1->setCheckable(true);
	directional1->setChecked(true);
	switch1->setCheckState(Qt::Unchecked);
	lightType1->setMenu(lightMenu1);
	lightX1->setMinimum(-1000.0);
	lightX1->setMaximum(1000.0);
	lightX1->setSingleStep(10.0);
	lightY1->setMinimum(-1000.0);
	lightY1->setMaximum(1000.0);
	lightY1->setSingleStep(10.0);
	lightZ1->setMinimum(-1000.0);
	lightZ1->setMaximum(1000.0);
	lightZ1->setSingleStep(10.0);
	lightX1->setValue(10.0);
	lightY1->setValue(10.0);
	lightZ1->setValue(10.0);
	angleLabel1->setDisabled(true);
	spotAngle1->setMinimum(0.0);
	spotAngle1->setMaximum(90.0);
	spotAngle1->setSingleStep(1.0);
	spotAngle1->setDisabled(true);

	/* light 1 signals */
	connect(lightBox, SIGNAL(toggled(bool)), lights, SLOT(masterSwitch(bool)));
	connect(switch1, SIGNAL(stateChanged(int)), lights, SLOT(lightSwitch1(int)));
	connect(lightMenu1, SIGNAL(triggered(QAction *)), this, SLOT(menu1(QAction *)));
	connect(lightX1, SIGNAL(valueChanged(double)), lights, SLOT(setX1(double)));
	connect(lightY1, SIGNAL(valueChanged(double)), lights, SLOT(setY1(double)));
	connect(lightZ1, SIGNAL(valueChanged(double)), lights, SLOT(setZ1(double)));
	connect(spotAngle1, SIGNAL(valueChanged(double)), lights, SLOT(setAngle1(double)));
	connect(colorButton1, SIGNAL(clicked()), lights, SLOT(setColor1()));
}

//! Light 2 Helper Function
/*! Sets options for the widgets that control light 2. */
void RobotWindow::light2()
{
	/* light 2 control options */
	lightMenu2->addAction(directional2);
	lightMenu2->addAction(positional2);
	lightMenu2->addAction(spotlight2);
	typeGroup2->addAction(directional2);
	typeGroup2->addAction(positional2);
	typeGroup2->addAction(spotlight2);
	typeGroup2->setExclusive(true);
	directional2->setCheckable(true);
	positional2->setCheckable(true);
	spotlight2->setCheckable(true);
	directional2->setChecked(true);
	switch2->setCheckState(Qt::Unchecked);
	lightType2->setMenu(lightMenu2);
	lightX2->setMinimum(-1000.0);
	lightX2->setMaximum(1000.0);
	lightX2->setSingleStep(10.0);
	lightY2->setMinimum(-1000.0);
	lightY2->setMaximum(1000.0);
	lightY2->setSingleStep(10.0);
	lightZ2->setMinimum(-1000.0);
	lightZ2->setMaximum(1000.0);
	lightZ2->setSingleStep(10.0);
	lightX2->setValue(-10.0);
	lightY2->setValue(-10.0);
	lightZ2->setValue(-10.0);
	angleLabel2->setDisabled(true);
	spotAngle2->setMinimum(0.0);
	spotAngle2->setMaximum(90.0);
	spotAngle2->setSingleStep(1.0);
	spotAngle2->setDisabled(true);

	/* light 2 signals */
	connect(switch2, SIGNAL(stateChanged(int)), lights, SLOT(lightSwitch2(int)));
	connect(lightMenu2, SIGNAL(triggered(QAction *)), this, SLOT(menu2(QAction *)));
	connect(lightX2, SIGNAL(valueChanged(double)), lights, SLOT(setX2(double)));
	connect(lightY2, SIGNAL(valueChanged(double)), lights, SLOT(setY2(double)));
	connect(lightZ2, SIGNAL(valueChanged(double)), lights, SLOT(setZ2(double)));
	connect(spotAngle2, SIGNAL(valueChanged(double)), lights, SLOT(setAngle2(double)));
	connect(colorButton2, SIGNAL(clicked()), lights, SLOT(setColor2()));
}

//! Light 3 Helper Function
/*! Sets options for the widgets that control light 3. */
void RobotWindow::light3()
{
	/* light 3 control options */
	lightMenu3->addAction(directional3);
	lightMenu3->addAction(positional3);
	lightMenu3->addAction(spotlight3);
	typeGroup3->addAction(directional3);
	typeGroup3->addAction(positional3);
	typeGroup3->addAction(spotlight3);
	typeGroup3->setExclusive(true);
	directional3->setCheckable(true);
	positional3->setCheckable(true);
	spotlight3->setCheckable(true);
	directional3->setChecked(true);
	switch3->setCheckState(Qt::Unchecked);
	lightType3->setMenu(lightMenu3);
	lightX3->setMinimum(-1000.0);
	lightX3->setMaximum(1000.0);
	lightX3->setSingleStep(10.0);
	lightY3->setMinimum(-1000.0);
	lightY3->setMaximum(1000.0);
	lightY3->setSingleStep(10.0);
	lightZ3->setMinimum(-1000.0);
	lightZ3->setMaximum(1000.0);
	lightZ3->setSingleStep(10.0);
	angleLabel3->setDisabled(true);
	spotAngle3->setMinimum(0.0);
	spotAngle3->setMaximum(90.0);
	spotAngle3->setSingleStep(1.0);
	spotAngle3->setDisabled(true);

	/* light 3 signals */
	connect(switch3, SIGNAL(stateChanged(int)), lights, SLOT(lightSwitch3(int)));
	connect(lightMenu3, SIGNAL(triggered(QAction *)), this, SLOT(menu3(QAction *)));
	connect(lightX3, SIGNAL(valueChanged(double)), lights, SLOT(setX3(double)));
	connect(lightY3, SIGNAL(valueChanged(double)), lights, SLOT(setY3(double)));
	connect(lightZ3, SIGNAL(valueChanged(double)), lights, SLOT(setZ3(double)));
	connect(spotAngle3, SIGNAL(valueChanged(double)), lights, SLOT(setAngle3(double)));
	connect(colorButton3, SIGNAL(clicked()), lights, SLOT(setColor3()));
}

//! Light 4 Helper Function
/*! Sets options for the widgets that control light 4. */
void RobotWindow::light4()
{
	/* light 4 control options */
	lightMenu4->addAction(directional4);
	lightMenu4->addAction(positional4);
	lightMenu4->addAction(spotlight4);
	typeGroup4->addAction(directional4);
	typeGroup4->addAction(positional4);
	typeGroup4->addAction(spotlight4);
	typeGroup4->setExclusive(true);
	directional4->setCheckable(true);
	positional4->setCheckable(true);
	spotlight4->setCheckable(true);
	directional4->setChecked(true);
	switch4->setCheckState(Qt::Unchecked);
	lightType4->setMenu(lightMenu4);
	lightX4->setMinimum(-1000.0);
	lightX4->setMaximum(1000.0);
	lightX4->setSingleStep(10.0);
	lightY4->setMinimum(-1000.0);
	lightY4->setMaximum(1000.0);
	lightY4->setSingleStep(10.0);
	lightZ4->setMinimum(-1000.0);
	lightZ4->setMaximum(1000.0);
	lightZ4->setSingleStep(10.0);
	angleLabel4->setDisabled(true);
	spotAngle4->setMinimum(0.0);
	spotAngle4->setMaximum(90.0);
	spotAngle4->setSingleStep(1.0);
	spotAngle4->setDisabled(true);

	/* light 4 signals */
	connect(switch4, SIGNAL(stateChanged(int)), lights, SLOT(lightSwitch4(int)));
	connect(lightMenu4, SIGNAL(triggered(QAction *)), this, SLOT(menu4(QAction *)));
	connect(lightX4, SIGNAL(valueChanged(double)), lights, SLOT(setX4(double)));
	connect(lightY4, SIGNAL(valueChanged(double)), lights, SLOT(setY4(double)));
	connect(lightZ4, SIGNAL(valueChanged(double)), lights, SLOT(setZ4(double)));
	connect(spotAngle4, SIGNAL(valueChanged(double)), lights, SLOT(setAngle4(double)));
	connect(colorButton4, SIGNAL(clicked()), lights, SLOT(setColor4()));
}

//! Light 5 Helper Function
/*! Sets options for the widgets that control light 5. */
void RobotWindow::light5()
{
	/* light 5 control options */
	lightMenu5->addAction(directional5);
	lightMenu5->addAction(positional5);
	lightMenu5->addAction(spotlight5);
	typeGroup5->addAction(directional5);
	typeGroup5->addAction(positional5);
	typeGroup5->addAction(spotlight5);
	typeGroup5->setExclusive(true);
	directional5->setCheckable(true);
	positional5->setCheckable(true);
	spotlight5->setCheckable(true);
	directional5->setChecked(true);
	switch5->setCheckState(Qt::Unchecked);
	lightType5->setMenu(lightMenu5);
	lightX5->setMinimum(-1000.0);
	lightX5->setMaximum(1000.0);
	lightX5->setSingleStep(10.0);
	lightY5->setMinimum(-1000.0);
	lightY5->setMaximum(1000.0);
	lightY5->setSingleStep(10.0);
	lightZ5->setMinimum(-1000.0);
	lightZ5->setMaximum(1000.0);
	lightZ5->setSingleStep(10.0);
	angleLabel5->setDisabled(true);
	spotAngle5->setMinimum(0.0);
	spotAngle5->setMaximum(90.0);
	spotAngle5->setSingleStep(1.0);
	spotAngle5->setDisabled(true);

	/* light 5 signals */
	connect(switch5, SIGNAL(stateChanged(int)), lights, SLOT(lightSwitch5(int)));
	connect(lightMenu5, SIGNAL(triggered(QAction *)), this, SLOT(menu4(QAction *)));
	connect(lightX5, SIGNAL(valueChanged(double)), lights, SLOT(setX5(double)));
	connect(lightY5, SIGNAL(valueChanged(double)), lights, SLOT(setY5(double)));
	connect(lightZ5, SIGNAL(valueChanged(double)), lights, SLOT(setZ5(double)));
	connect(spotAngle5, SIGNAL(valueChanged(double)), lights, SLOT(setAngle5(double)));
	connect(colorButton5, SIGNAL(clicked()), lights, SLOT(setColor5()));
}

//! Light Widget Pack Function
/*! Packs up all the Lighting widgets. */
void RobotWindow::packLights()
{
	/* pack up the lighting conrols */
	lightGrid->addWidget(lightType1, 0, 0, 1, 2);
	lightGrid->addWidget(lightType2, 0, 2, 1, 2);
	lightGrid->addWidget(lightType3, 0, 4, 1, 2);
	lightGrid->addWidget(lightType4, 0, 6, 1, 2);
	lightGrid->addWidget(lightType5, 0, 8, 1, 2);
	lightGrid->addWidget(switch1, 1, 0, 1, 2);
	lightGrid->addWidget(switch2, 1, 2, 1, 2);
	lightGrid->addWidget(switch3, 1, 4, 1, 2);
	lightGrid->addWidget(switch4, 1, 6, 1, 2);
	lightGrid->addWidget(switch5, 1, 8, 1, 2);
	lightGrid->addWidget(xLabel1, 2, 0);
	lightGrid->addWidget(xLabel2, 2, 2);
	lightGrid->addWidget(xLabel3, 2, 4);
	lightGrid->addWidget(xLabel4, 2, 6);
	lightGrid->addWidget(xLabel5, 2, 8);
	lightGrid->addWidget(lightX1, 2, 1);
	lightGrid->addWidget(lightX2, 2, 3);
	lightGrid->addWidget(lightX3, 2, 5);
	lightGrid->addWidget(lightX4, 2, 7);
	lightGrid->addWidget(lightX5, 2, 9);
	lightGrid->addWidget(yLabel1, 3, 0);
	lightGrid->addWidget(yLabel2, 3, 2);
	lightGrid->addWidget(yLabel3, 3, 4);
	lightGrid->addWidget(yLabel4, 3, 6);
	lightGrid->addWidget(yLabel5, 3, 8);
	lightGrid->addWidget(lightY1, 3, 1);
	lightGrid->addWidget(lightY2, 3, 3);
	lightGrid->addWidget(lightY3, 3, 5);
	lightGrid->addWidget(lightY4, 3, 7);
	lightGrid->addWidget(lightY5, 3, 9);
	lightGrid->addWidget(zLabel1, 4, 0);
	lightGrid->addWidget(zLabel2, 4, 2);
	lightGrid->addWidget(zLabel3, 4, 4);
	lightGrid->addWidget(zLabel4, 4, 6);
	lightGrid->addWidget(zLabel5, 4, 8);
	lightGrid->addWidget(lightZ1, 4, 1);
	lightGrid->addWidget(lightZ2, 4, 3);
	lightGrid->addWidget(lightZ3, 4, 5);
	lightGrid->addWidget(lightZ4, 4, 7);
	lightGrid->addWidget(lightZ5, 4, 9);
	lightGrid->addWidget(angleLabel1, 5, 0);
	lightGrid->addWidget(angleLabel2, 5, 2);
	lightGrid->addWidget(angleLabel3, 5, 4);
	lightGrid->addWidget(angleLabel4, 5, 6);
	lightGrid->addWidget(angleLabel5, 5, 8);
	lightGrid->addWidget(spotAngle1, 5, 1);
	lightGrid->addWidget(spotAngle2, 5, 3);
	lightGrid->addWidget(spotAngle3, 5, 5);
	lightGrid->addWidget(spotAngle4, 5, 7);
	lightGrid->addWidget(spotAngle5, 5, 9);
	lightGrid->addWidget(colorButton1, 6, 0, 1, 2);
	lightGrid->addWidget(colorButton2, 6, 2, 1, 2);
	lightGrid->addWidget(colorButton3, 6, 4, 1, 2);
	lightGrid->addWidget(colorButton4, 6, 6, 1, 2);
	lightGrid->addWidget(colorButton5, 6, 8, 1, 2);
	lightBox->setLayout(lightGrid);
}

//! Main Pack Function
/*! Packs up the main interface. */
void RobotWindow::packMain()
{
	/* additional controls */
	additionalLayout->addWidget(controlBox);
	additionalLayout->addWidget(visualBox);
	additionalBox->setLayout(additionalLayout);

	/* overall container for controls */
	controlContainerBox->addWidget(viewControls, 0, 0);
	controlContainerBox->addWidget(lightBox, 1, 0, 1, 3);
	controlContainer->setLayout(controlContainerBox);

	/* main layout */
	mainLayout->addWidget(qrobot, 0, 0);
	mainLayout->addWidget(additionalBox, 0, 1);
	mainLayout->addWidget(controlContainer, 1, 0, 1, 2);
	setLayout(mainLayout);
	setWindowTitle(tr("QT Robot Arm"));
}

//! Physics Helper Function
/*! Sets the options for the physics widgets. */
void RobotWindow::physics()
{
	/* spin box options */
	tempSpin->setMinimum(-40.0);
	tempSpin->setMaximum(100.0);
	tempSpin->setValue(qrobot->getTemperature());
	massSpin->setMinimum(10.0);
	massSpin->setMaximum(1000.0);
	massSpin->setValue(qrobot->getMass());
	dragSpin->setMinimum(0.1);
	dragSpin->setMaximum(3.0);
	dragSpin->setSingleStep(0.1);
	dragSpin->setValue(qrobot->getDrag());

	/* spin box signals */
	connect(tempSpin, SIGNAL(valueChanged(double)), qrobot, SLOT(setTemperature(double)));
	connect(massSpin, SIGNAL(valueChanged(double)), qrobot, SLOT(setMass(double)));
	connect(dragSpin, SIGNAL(valueChanged(double)), qrobot, SLOT(setDrag(double)));

	/* pack up the physics controls */
	controlGrid->addWidget(tempLabel, 0, 0);
    	controlGrid->addWidget(tempSpin, 0, 1);
	controlGrid->addWidget(massLabel, 1, 0);
	controlGrid->addWidget(massSpin, 1, 1);
	controlGrid->addWidget(dragLabel, 2, 0);
	controlGrid->addWidget(dragSpin, 2, 1);
    	controlBox->setLayout(controlGrid);
}

//! View Control Helper Function
/*! Sets the options for the robot movement/viewing control widgets. */
void RobotWindow::viewControl()
{
	/* view control options */
	viewRadio->setChecked(true);
	zoomSlider->setMinimum(100);
	zoomSlider->setMaximum(500);
	zoomSlider->setTickInterval(25);
	zoomSlider->setTickPosition(QSlider::TicksBelow);
	zoomSlider->setValue((int) qrobot->getZoom());
	viewRadio->setChecked(true);
	fingerSlider->setMinimum(0);
	fingerSlider->setMaximum(600);
	fingerSlider->setTickInterval(50);
	fingerSlider->setTickPosition(QSlider::TicksBelow);
	fingerSlider->setValue(0);

	/* pack up the view control objects */
	viewControlsBox->addWidget(viewRadio);
	viewControlsBox->addWidget(controlRadio);
	viewControlsBox->addWidget(zoomLabel);
	viewControlsBox->addWidget(zoomSlider);
	viewControlsBox->addWidget(fingerLabel);
	viewControlsBox->addWidget(fingerSlider);
	viewControlsBox->addWidget(cubeStatus);
	viewControlsBox->addStretch(1);
	viewControls->setLayout(viewControlsBox);

	/* view control signals */
	connect(viewRadio, SIGNAL(toggled(bool)), qrobot, SLOT(setMode(bool)));
	connect(zoomSlider, SIGNAL(valueChanged(int)), qrobot, SLOT(setZoom(int)));
	connect(fingerSlider, SIGNAL(valueChanged(int)), qrobot, SLOT(setFingerAngle(int)));
	connect(qrobot, SIGNAL(cubeGrabbed(QString)), cubeStatus, SLOT(setText(QString)));
}

//! Visual Helper Function
/*! Sets options for the visual effects widgets. */
void RobotWindow::visual()
{
	/* current light menu */
	currLightMenu->addAction(noLight);
	currLightMenu->addAction(currLight1);
	currLightMenu->addAction(currLight2);
	currLightMenu->addAction(currLight3);
	currLightMenu->addAction(currLight4);
	currLightMenu->addAction(currLight5);
	currLightGroup->addAction(noLight);
	currLightGroup->addAction(currLight1);
	currLightGroup->addAction(currLight2);
	currLightGroup->addAction(currLight3);
	currLightGroup->addAction(currLight4);
	currLightGroup->addAction(currLight5);
	currLightGroup->setExclusive(true);
	currLight1->setCheckable(true);
	currLight2->setCheckable(true);
	currLight3->setCheckable(true);
	currLight4->setCheckable(true);
	currLight5->setCheckable(true);
	currLight->setMenu(currLightMenu);
	noLight->setCheckable(true);
	noLight->setChecked(true);

	/* material menu */
	materialMenu->addAction(cartoon);
	materialMenu->addAction(gold);
	materialMenu->addAction(obsidian);
	materialMenu->addAction(silver);
	materialGroup->addAction(cartoon);
	materialGroup->addAction(gold);
	materialGroup->addAction(obsidian);
	materialGroup->addAction(silver);
	materialGroup->setExclusive(true);
	cartoon->setCheckable(true);
	gold->setCheckable(true);
	obsidian->setCheckable(true);
	silver->setCheckable(true);
	cartoon->setChecked(true);
	material->setMenu(materialMenu);

	/* texture menu */
	textureMenu->addAction(classic);
	textureMenu->addAction(dice);
	textureGroup->addAction(classic);
	textureGroup->addAction(dice);
	textureGroup->setExclusive(true);
	classic->setCheckable(true);
	dice->setCheckable(true);
	if (qrobot->texturesLoaded())
		dice->setChecked(true);
	else
		classic->setChecked(true);
	texture->setMenu(textureMenu);

	/* pack the menus */
	visualLayout->addWidget(currLight);
	visualLayout->addWidget(material);
	visualLayout->addWidget(texture);
	visualBox->setLayout(visualLayout);

	/* visual signals */
	connect(currLightMenu, SIGNAL(triggered(QAction *)), this, SLOT(currentLight(QAction *)));
	connect(materialMenu, SIGNAL(triggered(QAction *)), this, SLOT(robotMaterial(QAction *)));
	connect(textureMenu, SIGNAL(triggered(QAction *)), this, SLOT(cubeTexture(QAction *)));
}

//! Visual Effects Widget Helper Function */
/*! Allocates all the widgets in the visual effects group. */
void RobotWindow::visualWidgets()
{
	/* main layout */
	visualBox = new QGroupBox(tr("Other Visual Effects"));
	visualLayout = new QVBoxLayout;

	/* buttons and menus */
	currLight = new QPushButton(tr("Current Light"));
	material = new QPushButton(tr("Robot Material"));
	texture = new QPushButton(tr("Cube Style"));
	currLightMenu = new QMenu(this);
	materialMenu = new QMenu(this);
	textureMenu = new QMenu(this);
	currLightGroup = new QActionGroup(currLight);
	materialGroup = new QActionGroup(material);
	textureGroup = new QActionGroup(texture);

	/* menu options */
	currLight1 = new QAction(tr("Light 1"), currLight);
	currLight2 = new QAction(tr("Light 2"), currLight);
	currLight3 = new QAction(tr("Light 3"), currLight);
	currLight4 = new QAction(tr("Light 4"), currLight);
	currLight5 = new QAction(tr("Light 5"), currLight);
	noLight = new QAction(tr("(None)"), currLight);
	cartoon = new QAction(tr("Cartoon"), materialMenu);
	gold = new QAction(tr("Gold"), materialMenu);
	obsidian = new QAction(tr("Obsidian"), materialMenu);
	silver = new QAction(tr("Silver"), materialMenu);
	classic = new QAction(tr("Classic"), textureMenu);
	dice = new QAction(tr("Dice"), textureMenu);
}
