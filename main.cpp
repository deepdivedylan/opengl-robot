#include "robot.h"
#include <QApplication>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	RobotWindow window;
	window.show();
	return app.exec();
}
