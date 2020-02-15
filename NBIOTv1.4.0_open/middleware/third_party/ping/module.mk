
PING_SRC = middleware/third_party/ping


#################################################################################
#include path
CFLAGS 	+= -I./include
CFLAGS	+= -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/include
CFLAGS  += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/portable/GCC/ARM_CM4F
CFLAGS  += -I$(SOURCE_DIR)/middleware/third_party/ping/inc
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/minicli/inc
CFLAGS  += -I$(SOURCE_DIR)/middleware/third_party/lwip/src/include
CFLAGS  += -I$(SOURCE_DIR)/middleware/third_party/lwip/ports/include
CFLAGS  += -I$(SOURCE_DIR)/driver/chip/mt$(PRODUCT_VERSION)/inc
