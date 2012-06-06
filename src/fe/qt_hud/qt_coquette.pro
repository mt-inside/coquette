HEADERS = main_form.h               \
          proxy_base.h              \
          proxy_derivative_label.h  \
          proxy_flag_label.h        \
          proxy_integral_label.h    \
          proxy_ratio_dial.h        \
          proxy_ratio_label.h       \
          proxy_shift_label.h       \
          proxy_value_label.h       \
          proxy_zerosixty_label.h   \
          widgets/dial.h
SOURCES = main_form.cpp              \
          proxy_base.cpp             \
          proxy_derivative_label.cpp \
          proxy_flag_label.cpp       \
          proxy_integral_label.cpp   \
          proxy_ratio_dial.cpp       \
          proxy_ratio_label.cpp      \
          proxy_shift_label.cpp      \
          proxy_value_label.cpp      \
          proxy_zerosixty_label.cpp  \
          widgets/dial.cpp           \
          qt_coquette.cpp
FORMS   = main_form.ui
QMAKE_CXXFLAGS += -Wall -Wextra -Werror -pg -g3 -O0
LIBS           += -Wl,-rpath,../../../build -L../../../build -lcoquette
INCLUDEPATH     = ../../../src boost_1_49_0
