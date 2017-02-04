SOURCES = lighting.cpp \
	  main.cpp \
	  robot.cpp \
	  shapes.cpp \
	  matrix.cpp \
	  vector.cpp \
	  qrobot.cpp \
	  qwindow.cpp
HEADERS = robot.h \
	  linalg.h
TARGET = robot
CONFIG += qt debug
QT += opengl
macx {
	DEFINES = MacOSX
}
