.DEFAULT: coquette
.PHONY: clean

# Source directory
SRC_ROOT := ../src

# Front end
FE := ${SRC_ROOT}/fe/fe_avg_stream.c
# "Serial" implimentation
COM := ${SRC_ROOT}/com/com_win32.c

SRC_DIRS := ${SRC_ROOT} ${SRC_ROOT}/observers
SRC_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.[ch])) ${FE} ${COM}

coquette: ${SRC_FILES}
	${CC} -o $@ -pg -g3 -O0 -Wall -Wextra -pedantic -Werror $(foreach dir,$(SRC_DIRS),-I$(dir)) $^ -lm

clean:
	rm -f coquette
