
NBIOT_L4_CUSTOM_SRC = middleware/MTK/nbiot_custom/l4


##############################################################################
# Include paths for this module
##############################################################################

CFLAGS += -I$(SOURCE_DIR)/$(NBIOT_L4_CUSTOM_SRC)/atci.mod/inc
CFLAGS += -I$(SOURCE_DIR)/$(NBIOT_L4_CUSTOM_SRC)/mux_gpio/inc
CFLAGS += -I$(SOURCE_DIR)/$(NBIOT_L4_CUSTOM_SRC)/port/inc
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/simat_proxy/inc
CFLAGS += -I$(SOURCE_DIR)/$(NBIOT_L4_CUSTOM_SRC)/sim/inc

ifneq ($(wildcard $(strip $(SOURCE_DIR))/middleware/MTK/tool_authentication/src_protected/),)
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/tool_authentication/inc
else
CFLAGS += -I$(SOURCE_DIR)/prebuilt/middleware/MTK/tool_authentication/inc
endif
