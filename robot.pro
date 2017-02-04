SOURCES = lighting.cpp \
	  main.cpp \
	  robot.cpp \
	  shapes.cpp \
	  matrix.cpp \
	  vector.cpp \
	  qrobot.cpp \
	  robotwindow.cpp
HEADERS = robot.h \
	  linalg.h
TARGET = robot
CONFIG += qt debug
LIBS += -lGL -lGLU
QT += opengl widgets
macx {
	DEFINES = MacOSX
}
