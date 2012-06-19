HEADERS = main_form.h                       \
          proxies/proxy_base.h              \
          proxies/proxy_derivative_label.h  \
          proxies/proxy_flag_label.h        \
          proxies/proxy_integral_label.h    \
          proxies/proxy_ratio_dial.h        \
          proxies/proxy_ratio_label.h       \
          proxies/proxy_shift_label.h       \
          proxies/proxy_value_label.h       \
          proxies/proxy_zerosixty_label.h   \
          widgets/dial.h
SOURCES = main_form.cpp                      \
          proxies/proxy_base.cpp             \
          proxies/proxy_derivative_label.cpp \
          proxies/proxy_flag_label.cpp       \
          proxies/proxy_integral_label.cpp   \
          proxies/proxy_ratio_dial.cpp       \
          proxies/proxy_ratio_label.cpp      \
          proxies/proxy_shift_label.cpp      \
          proxies/proxy_value_label.cpp      \
          proxies/proxy_zerosixty_label.cpp  \
          widgets/dial.cpp                   \
          qt_coquette.cpp
FORMS   = main_form.ui
QMAKE_CXXFLAGS += -Wall -Wextra -Werror -pg -g3 -O0
LIBS           += -Wl,-rpath,../../../build -L../../../build -lcoquette
INCLUDEPATH     = ../../../src
