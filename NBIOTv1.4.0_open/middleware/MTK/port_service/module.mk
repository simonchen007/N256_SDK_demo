
FEATURE ?= feature.mk
include $(FEATURE)

###################################################
# Sources

###################################################
# include path
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/port_service/inc
CFLAGS += -I$(SOURCE_DIR)/driver/chip/inc
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/usb/inc
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/nbiot_custom/l4/port/src
CFLAGS += -D__MTK_TARGET__
