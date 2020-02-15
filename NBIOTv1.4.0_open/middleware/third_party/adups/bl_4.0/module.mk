ADUPS_SRC = middleware/third_party/adups


#C_FILES += $(ADUPS_SRC)/bl_4.0/wosun/src/adups_bl_main.c
#C_FILES += $(ADUPS_SRC)/bl_4.0/wosun/src/adups_bl_flash.c	
#C_FILES += $(ADUPS_SRC)/bl_4.0/wosun/src/adups_file.c



CFLAGS +=  -DWOSUN_LZMA_ENABLE



CFLAGS += -I$(SOURCE_DIR)/$(ADUPS_SRC)/bl_4.0/wosun/inc
CFLAGS += -I$(SOURCE_DIR)/driver/chip/mt2625/inc
CFLAGS += -I$(SOURCE_DIR)/project/mt2625_evb/apps/bootloader/inc
CFLAGS += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/include
