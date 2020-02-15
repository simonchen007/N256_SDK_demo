NVDM_SRC = middleware/MTK/nvdm

#################################################################################
#include path
CFLAGS  += -I$(SOURCE_DIR)/$(NVDM_SRC)/inc
CFLAGS  += -I$(SOURCE_DIR)/$(NVDM_SRC)/inc_core
CFLAGS  += -I$(SOURCE_DIR)/middleware/util/include 
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/minicli/include 
CFLAGS  += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/include
CFLAGS  += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/include 
CFLAGS  += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/include/portable/GCC/ARM_CM4F
CFLAGS  += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source
CFLAGS  += -I$(SOURCE_DIR)/middleware/mlog/include
CFLAGS  += -I$(SOURCE_DIR)/driver/chip/mt$(PRODUCT_VERSION)/include
CFLAGS  += -I$(SOURCE_DIR)/driver/chip/inc
CFLAGS  += -I$(OUTPATH)/autogen/conf_xml
CFLAGS  += -I$(SOURCE_DIR)/middleware/port_service/inc
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/nbiot_custom/l4/atci.mod/inc

ifneq ($(wildcard $(strip $(SOURCE_DIR))/middleware/MTK/nvdm_core/),)
include $(SOURCE_DIR)/middleware/MTK/nvdm_core/src_core/GCC/module.mk
else
include $(SOURCE_DIR)/prebuilt/middleware/MTK/nvdm/module.mk
endif

#################################################################################
#Enable the feature by configuring
CFLAGS += -DMTK_NVDM_ENABLE
