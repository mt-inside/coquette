.DEFAULT: coquette
.PHONY: clean

# Source directory
SRC_ROOT := ../src

# Front end
FE := ${SRC_ROOT}/fe/fe_avg_stream.c
FE_OBJECT_FILE := $(patsubst %.c,%.o,$(FE))

# "Serial" implementation
COM := ${SRC_ROOT}/com/com_replay.c

SRC_DIRS := ${SRC_ROOT} ${SRC_ROOT}/observers
SRC_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c)) ${COM}
LIB_OBJECT_FILES := $(patsubst %.c,%.o,$(SRC_FILES))

CFLAGS := -pg -g3 -O0 -Wall -Wextra -pedantic -Werror
LDFLAGS := -pg -g3

INCLUDES := $(foreach dir,$(SRC_DIRS),-I$(dir)) 
LIBS := -lm -lpthread

%.o: %.c
	${CC} ${CFLAGS} ${INCLUDES}-c -o $@ $^

coquette: ${LIB_OBJECT_FILES} ${FE_OBJECT_FILE}
	${CC} ${LDFLAGS} -o $@ $^ ${LIBS}

libcoquette.so: ${LIB_OBJECT_FILES}
	${CC} ${LDFLAGS} -shared -Wl,-soname,$@ -fPIC -o $@ $^ ${LIBS}

clean:
	rm -f ${LIB_OBJECT_FILES} ${FE_OBJECT_FILE} coquette libcoquette.so
