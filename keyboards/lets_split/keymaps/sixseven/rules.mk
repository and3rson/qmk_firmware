ifndef QUANTUM_DIR
	include ../../../../Makefile
endif

RGBLIGHT_ENABLE = yes
POINTING_DEVICE_ENABLE = yes
PS2_MOUSE_ENABLE = yes
PS2_USE_USART = yes

SRC += smoothled.c knob_v2.c taphold.c
