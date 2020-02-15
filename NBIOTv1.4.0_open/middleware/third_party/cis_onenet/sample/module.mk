
###################################################
# Sources
ONENET_SAMPLE_SRC = middleware/third_party/cis_onenet/sample


###################################################
# include path
CFLAGS += -DCIS_EMBED -DHAVE_STRUCT_TIMESPEC
CFLAGS += -I$(SOURCE_DIR)/include/rtos/FreeRTOS/Source/include
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/cis_onenet/sample/asyn_mode
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/cis_onenet/sample/sync_mode
