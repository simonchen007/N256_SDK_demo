
###############################################################################
# feature option dependency
###############################################################################

ifneq ($(APB_PROXY_UNIT_TEST), Y)
include $(SOURCE_DIR)/middleware/MTK/mux_ap/module.mk
endif

#################################################################################
# source files
#################################################################################

#################################################################################
# include path
#################################################################################

CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/inc
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/inc_private
ifeq ($(APB_PROXY_UNIT_TEST), Y)
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/unit_test
endif
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/apb_proxy_cmd_hdlr/ril
ifeq ($(MTK_ATCI_APB_PROXY_NETWORK_ENABLE), y)
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/apb_proxy_cmd_hdlr/network
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/apb_proxy_cmd_hdlr/network/lwm2m
ifeq ($(MTK_COAP_SUPPORT), y)
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/apb_proxy_cmd_hdlr/network/coap
endif
endif
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/apb_proxy_cmd_hdlr/apb_test
ifeq ($(MTK_FOTA_ENABLE), y)
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/apb_proxy_cmd_hdlr/fota
endif
ifeq ($(MTK_ATCI_APB_PROXY_ADAPTER_ENABLE), y)
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/apb_proxy_cmd_hdlr/atci_adapter
endif

#################################################################################
# Define Macros
#################################################################################

ifeq ($(APB_PROXY_UNIT_TEST), Y)
CFLAGS += -DENABLE_APB_PROXY_UNIT_TEST
endif

ifeq ($(MTK_APB_PROXY_PROJECT_COMMAND_ENABLE), y)
CFLAGS += -DAPB_PROXY_PROJECT_COMMAND_ENABLE
endif

ifeq ($(MTK_FOTA_ENABLE), y)
CFLAGS += -DAPB_PROXY_FOTA_CMD_ENABLE
endif

ifeq ($(MTK_ATCI_APB_PROXY_NETWORK_ENABLE), y)
CFLAGS += -DMTK_ATCI_APB_PROXY_NETWORK_ENABLE
endif

ifeq ($(MTK_COAP_SUPPORT), y)
CFLAGS += -DMTK_COAP_SUPPORT
endif

ifeq ($(MTK_LWM2M_AT_CMD_SUPPORT), y)
CFLAGS += -DMTK_LWM2M_AT_CMD_SUPPORT
endif
