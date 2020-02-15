
###################################################
# Sources

###################################################
# include path
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/tel_conn_mgr/app/inc
ifeq ($(MTK_CREATE_DEFAULT_APN),y)
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/tel_conn_mgr/apn/inc
endif
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/tel_conn_mgr/bearer/inc
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/tel_conn_mgr/common/inc
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/ril/inc
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/lwip/ports/include
CFLAGS += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/include
