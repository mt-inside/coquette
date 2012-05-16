HEADERS = main_form.h
SOURCES = main_form.cpp qt_coquette.cpp
FORMS   = main_form.ui
LIBS   += -Wl,-rpath,../../../build -L../../../build -lcoquette
INCLUDEPATH = ../../../src
