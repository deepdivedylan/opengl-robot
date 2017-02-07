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
win32 {
	# downloading Windows 10 SDK is necessary in addition to Visual Studio
	# https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk
	DEFINES = Win32
	INCLUDEPATH += "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Include"
	INCLUDEPATH += "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include"
	INCLUDEPATH += "C:\Program Files (x86)\Windows Kits\10\Include\10.0.10240.0\ucrt"
}
