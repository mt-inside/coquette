.DEFAULT: all
.PHONY: clean

#COM := ${COM_DIR}/com_replay.c

BASENAME := coquette

ifneq (,$(findstring Windows,$(PATH)))
    OS := win32
    LIB_NAME := lib${BASENAME}.dll
    EXE_NAME := ${BASENAME}.exe
    COM ?= ${COM_DIR}/com_win32.c
    DEL := DEL
    CC := gcc
else
    OS := $(shell uname -s)
    LIB_NAME := lib${BASENAME}.so
    EXE_NAME := ${BASENAME}
    COM ?= ${COM_DIR}/com_unix.c
    DEL := rm -f
    CFLAGS += -fPIC
endif

# Source directories
ROOT            ?= ..
SRC_ROOT        := ${ROOT}/src
SRC_SEARCH_DIRS := ${SRC_ROOT} ${SRC_ROOT}/observers
COM_DIR         := ${SRC_ROOT}/com
FE_DIR          := ${SRC_ROOT}/fe
SRC_ALL_DIRS    := ${SRC_SEARCH_DIRS} ${COM_DIR} ${FE_DIR}

# TODO - don't actually need the paths on these src files; they're just taken
# off again. Src files are found by vpath, not these paths.
# Front end
FE ?= ${FE_DIR}/fe_avg_stream.c
FE_OBJECT_FILE := $(notdir $(patsubst %.c,%.o,$(FE)))

LIB_SRC_FILES += $(foreach dir,$(SRC_SEARCH_DIRS),$(wildcard $(dir)/*.c)) ${COM} ${COM_DIR}/com_utils.c
LIB_OBJECT_FILES := $(patsubst %.c,%.o,$(LIB_SRC_FILES))
LIB_OBJECT_FILES := $(notdir $(LIB_OBJECT_FILES))

vpath %.c $(SRC_ALL_DIRS)

CFLAGS += -std=c99 -pg -g3 -O0 -Wall -Wextra -pedantic -Werror -D_BSD_SOURCE
LDFLAGS += -pg -g3

INCLUDES := $(foreach dir,$(SRC_SEARCH_DIRS),-I$(dir))
LIBS := -lm -lpthread

all: ${EXE_NAME} ${LIB_NAME}

%.o: %.c
	${CC} ${CFLAGS} ${INCLUDES} -c -o $@ $<

${EXE_NAME}: ${LIB_OBJECT_FILES} ${FE_OBJECT_FILE}
	${CC} ${LDFLAGS} -o $@ $^ ${LIBS}

${LIB_NAME}: ${LIB_OBJECT_FILES}
	${CC} ${LDFLAGS} -shared -Wl,-soname,$@ -o $@ $^ ${LIBS}

clean:
	${DEL} ${LIB_OBJECT_FILES} ${FE_OBJECT_FILE} ${EXE_NAME} ${LIB_NAME}
