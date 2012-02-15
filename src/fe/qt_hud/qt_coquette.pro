HEADERS = main_form.h
SOURCES = main_form.cpp qt_coquette.cpp
FORMS   = main_form.ui
unix:LIBS   += -Wl,-rpath,../../../build -L../../../build -lcoquette
unix:INCLUDEPATH = ../../../src
