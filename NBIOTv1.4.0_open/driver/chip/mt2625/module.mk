
MT2625_CHIP = driver/chip/mt2625
COMPONENT = driver/board/component

CFLAGS   += $(FPUFLAGS) -DUSE_HAL_DRIVER -DPRODUCT_VERSION=2625


#################################################################################
#include path
CFLAGS  += -I../inc
CFLAGS  += -Isrc/common/include
CFLAGS  += -Isrc/sdio_gen3/include
CFLAGS  += -Iinc
CFLAGS  += -I$(SOURCE_DIR)/driver/chip/inc
CFLAGS  += -I$(SOURCE_DIR)/driver/chip/mt2625/inc
CFLAGS 	+= -I$(SOURCE_DIR)/kernel/service/inc
CFLAGS  += -I$(SOURCE_DIR)/driver/board/component/lcm/ST7789H2
CFLAGS += -I$(SOURCE_DIR)/driver/CMSIS/Device/MTK/mt2625/Include
CFLAGS += -I$(SOURCE_DIR)/driver/CMSIS/Include
CFLAGS += -I$(SOURCE_DIR)/driver/board/component/lcm/ST7789H2
CFLAGS += -I$(SOURCE_DIR)/driver/board/mt25x3_hdk/lcd
CFLAGS += -I$(SOURCE_DIR)/driver/board/component/common
CFLAGS += -I$(SOURCE_DIR)/$(MT2625_CHIP)/src/common/include

ifeq ($(MTK_DCXO_CAPID_LOAD_FROM_EFUSE), y)
    CFLAGS += -DDCXO_CAPID_LOAD_FROM_EFUSE
endif

ifeq ($(MTK_DCXO_CAPID_CAL), y)
    CFLAGS += -DDCXO_CAPID_CAL
    ifeq ($(MTK_DCXO_CAPID_STORE_TO_EFUSE), 0)
        CFLAGS += -DDCXO_CAPID_STORE_TO_EFUSE0
    else ifeq ($(MTK_DCXO_CAPID_STORE_TO_EFUSE), 1)
        CFLAGS += -DDCXO_CAPID_STORE_TO_EFUSE1
    else ifeq ($(MTK_DCXO_CAPID_STORE_TO_EFUSE), 2)
        CFLAGS += -DDCXO_CAPID_STORE_TO_EFUSE2
    endif
endif

#ifeq ($(MTK_EXTERNAL_PMIC), y)
#	CFLAGS += -I$(SOURCE_DIR)/driver/board/component/pmic/
#endif


ifneq ($(wildcard $(strip $(SOURCE_DIR))/driver/chip/mt2625/src_core/),)
include $(SOURCE_DIR)/driver/chip/mt2625/src_core/GCC/module.mk
else
LIBS += $(SOURCE_DIR)/prebuilt/driver/chip/mt2625/lib/libhal_core_CM4_GCC.a
endif

ifneq ($(wildcard $(strip $(SOURCE_DIR))/driver/chip/mt2625/src_protected/),)
include $(SOURCE_DIR)/driver/chip/mt2625/src_protected/GCC/module.mk
else
LIBS += $(SOURCE_DIR)/prebuilt/driver/chip/mt2625/lib/libhal_protected_CM4_GCC.a
endif

#LIBS += $(SOURCE_DIR)/driver/chip/mt2625/lib/libcryptos_CM4_GCC.a
