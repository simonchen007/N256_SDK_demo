
			 

#################################################################################
#include path
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/iperf/inc
CFLAGS	+= -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/include 
CFLAGS  += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/portable/GCC/ARM_CM4F
CFLAGS  += -I$(SOURCE_DIR)/driver/chip/mt$(PRODUCT_VERSION)/inc
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/connsys/inc
