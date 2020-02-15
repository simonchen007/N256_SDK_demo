
BOARD_SRC = driver/board/mt2625_hdk
COMPONENT_SRC = driver/board/component


#################################################################################
#include path
CFLAGS 	+= -Iinclude
CFLAGS  += -I$(SOURCE_DIR)/kernel/service/inc
CFLAGS  += -I$(SOURCE_DIR)/driver/chip/inc
CFLAGS  += -I$(SOURCE_DIR)/driver/chip/mt2625/inc


#display driver include path
CFLAGS  += -I$(SOURCE_DIR)/driver/board/component/common
CFLAGS  += -I$(BOARD_SRC)/lcd
CFLAGS  += -I$(BOARD_SRC)/backlight
