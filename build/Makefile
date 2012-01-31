.DEFAULT: coquette

# Source directory
SRC := ../src

# Front end
FE := ${SRC}/fe/fe_avg.c
# "Serial" implimentation
COM := ${SRC}/com/com_stdio.c

coquette: ${SRC}/*.c ${FE} ${COM}
	${CC} -o $@ -pg -g3 -O0 -Wall -Wextra -pedantic -Werror -I${SRC} $^ -lm
