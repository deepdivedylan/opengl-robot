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
QT += opengl widgets
macx {
	DEFINES = MacOSX
}
unix {
	LIBS += -lGL -lGLU
}
win32 {
	# downloading Windows 10 SDK is necessary in addition to Visual Studio
	# https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk
	DEFINES = Win32
	LIBS += opengl32.lib glu32.lib
}
