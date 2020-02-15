
###################################################
# Sources
COAP_SRC = middleware/third_party/libcoap/src



###################################################
# include path
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/libcoap/include
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/lwip/ports/include
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/lwip/src/include
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/lwip/src/include/lwip
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/lwip/src/include/netif


