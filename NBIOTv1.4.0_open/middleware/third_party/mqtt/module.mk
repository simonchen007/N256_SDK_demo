
MQTT_SRC = middleware/third_party/mqtt


#################################################################################
#include path
CFLAGS 	+= -I./include
CFLAGS	+= -I$(SOURCE_DIR)/middleware/third_party/lwip/src/include
CFLAGS  += -I$(SOURCE_DIR)/middleware/third_party/lwip/src/include/lwip
CFLAGS  += -I$(SOURCE_DIR)/middleware/third_party/lwip/ports/include
CFLAGS  += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/include
CFLAGS  += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/portable/GCC/ARM_CM4F
CFLAGS  += -I$(SOURCE_DIR)/middleware/third_party/mqtt/MQTTPacket/src
CFLAGS  += -I$(SOURCE_DIR)/middleware/third_party/mqtt/MQTTClient-C/src
CFLAGS  += -I$(SOURCE_DIR)/middleware/third_party/mqtt/MQTTClient-C/src/mediatek
CFLAGS  += -I$(SOURCE_DIR)/middleware/third_party/mbedtls/include
