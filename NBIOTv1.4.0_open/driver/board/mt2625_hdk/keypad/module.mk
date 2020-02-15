
BOARD_SRC = driver/board/mt2625_hdk
COMPONENT_SRC = driver/board/component

#keypad custome file

#################################################################################
#include path
CFLAGS 	+= -Iinclude
CFLAGS  += -I$(SOURCE_DIR)/kernel/service/inc
CFLAGS  += -I$(SOURCE_DIR)/driver/chip/inc
CFLAGS  += -I$(SOURCE_DIR)/driver/chip/mt2625/inc
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/inc


#keypad driver include path
CFLAGS  += -I$(SOURCE_DIR)/driver/board/mt2625_hdk/keypad/inc
