OLED_DRIVER_ENABLE = yes

ifndef QUANTUM_DIR
	include ../../../../Makefile
endif

SRC += taphold.c smoothled.c
