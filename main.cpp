#include "robot.h"
#include <QApplication>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	QWindow window;
	window.show();
	return app.exec();	
}
