NVDM_MODEM_SRC = middleware/MTK/nvdm_modem


#################################################################################
#include path
CFLAGS 	+= -I$(SOURCE_DIR)/$(NVDM_MODEM_SRC)/inc
CFLAGS 	+= -I$(SOURCE_DIR)/$(NVDM_MODEM_SRC)/inc_core
CFLAGS  += -I$(SOURCE_DIR)/middleware/util/include
CFLAGS  += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/include
CFLAGS  += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/include
CFLAGS  += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/include/portable/GCC/ARM_CM4F
CFLAGS  += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source
CFLAGS  += -I$(SOURCE_DIR)/middleware/mlog/include
CFLAGS  += -I$(SOURCE_DIR)/driver/chip/inc

#################################################################################
