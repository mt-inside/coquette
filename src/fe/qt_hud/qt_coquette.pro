HEADERS = main_form.h    \
          proxy_base.h  \
          proxy_value_label.h  \
          widgets/dial.h
SOURCES = main_form.cpp    \
          proxy_base.cpp  \
          proxy_value_label.cpp  \
          widgets/dial.cpp \
          qt_coquette.cpp
FORMS   = main_form.ui
QMAKE_CXXFLAGS += -Wall -Wextra -Werror -pg -g3 -O0
LIBS           += -Wl,-rpath,../../../build -L../../../build -lcoquette
INCLUDEPATH     = ../../../src boost_1_49_0
