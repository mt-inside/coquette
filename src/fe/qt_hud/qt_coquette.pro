HEADERS = main_form.h   widgets/dial.h
SOURCES = main_form.cpp widgets/dial.cpp qt_coquette.cpp
FORMS   = main_form.ui
QMAKE_CXXFLAGS += -Wall -Wextra -Werror -pg -g3 -O0
LIBS           += -Wl,-rpath,../../../build -L../../../build -lcoquette
INCLUDEPATH     = ../../../src
