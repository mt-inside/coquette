.DEFAULT: coquette
.PHONY: clean

# Source directory
SRC_ROOT := ../src

# Front end
FE := ${SRC_ROOT}/fe/fe_avg_stream.c
# "Serial" implimentation
COM := ${SRC_ROOT}/com/com_replay.c

SRC_DIRS := ${SRC_ROOT} ${SRC_ROOT}/observers
SRC_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.[ch])) ${COM}

CFLAGS := -pg -g3 -O0 -Wall -Wextra -pedantic -Werror

coquette: ${SRC_FILES} ${FE}
	${CC} -o $@ ${CFLAGS} $(foreach dir,$(SRC_DIRS),-I$(dir)) $^ -lm

#need a shared compile stage to build objects (where lib target sets -fPIC), then two link stages
lib: ${SRC_FILES}
	${CC} -shared -Wl,-soname,libcoquette.so -fPIC -o libcoquette.so ${CFLAGS} $(foreach dir,$(SRC_DIRS),-I$(dir)) $^ -lm

clean:
	rm -f coquette
