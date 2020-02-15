
###################################################
# Sources

###################################################
# include path
CFLAGS += -DCIS_EMBED -DHAVE_STRUCT_TIMESPEC
CFLAGS += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/include
CFLAGS += -I$(SOURCE_DIR)/driver/chip/inc
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/cis_onenet/ciscore
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/er-coap-13
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/cis_onenet/ciscore/std_object
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/cis_onenet/porting


