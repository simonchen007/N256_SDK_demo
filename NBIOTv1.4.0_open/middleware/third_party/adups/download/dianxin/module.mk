ABUP_APP_PATH_SRC=middleware/third_party/adups/download/dianxin/source/src
ABUP_APP_PATH_INC=middleware/third_party/adups/download/dianxin/source/inc
ABUP_APP_PATH_LIB=middleware/third_party/adups/download/dianxin/lib

C_FILES += $(ABUP_APP_PATH_SRC)/abup_flash.c
#C_FILES += $(ABUP_APP_PATH_SRC)/abup_firmware.c

LIBS += $(SOURCE_DIR)/$(ABUP_APP_PATH_LIB)/abup_dianxin.lib

CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/lwm2m/core
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/lwm2m/core/shared
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/lwm2m/core/er-coap-13
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/adups/bl/inc
CFLAGS += -I$(SOURCE_DIR)/$(ABUP_APP_PATH_INC)
