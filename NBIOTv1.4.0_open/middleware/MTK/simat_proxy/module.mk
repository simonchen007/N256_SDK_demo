
FEATURE ?= feature.mk
include $(FEATURE)

###################################################
# Sources
SIMAT_PROXY_SRC = middleware/MTK/simat_proxy

###################################################
# include path
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/simat_proxy/inc
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/mux_ap/inc
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/tel_conn_mgr/app/inc
