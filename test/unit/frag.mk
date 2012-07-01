HERE := ${ROOT}/test/unit

vpath %.c ${HERE}

LIB_SRC_FILES += unit_driver.c          \
		 observer_stats_tests.c
