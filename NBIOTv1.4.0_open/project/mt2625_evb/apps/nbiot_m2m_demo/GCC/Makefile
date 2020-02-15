# Copyright Statement:
#
# (C) 2005-2016  MediaTek Inc. All rights reserved.
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. ("MediaTek") and/or its licensors.
# Without the prior written permission of MediaTek and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.
# You may only use, reproduce, modify, or distribute (as applicable) MediaTek Software
# if you have agreed to and been bound by the applicable license agreement with
# MediaTek ("License Agreement") and been granted explicit permission to do so within
# the License Agreement ("Permitted User").  If you are not a Permitted User,
# please cease any access or use of MediaTek Software immediately.
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT MEDIATEK SOFTWARE RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES
# ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
# WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
#
 
#######################################################
# Project vairables assignment

SOURCE_DIR = ../../../../..

PWD= $(shell pwd)
SDK_PATH    = $(abspath $(PWD)/$(SOURCE_DIR))
FEATURE ?= feature.mk

DEBUG = 0
FLOAT_TYPE = hard
BUILD_DIR = $(PWD)/Build

# Project name
PROJ_NAME = $(shell basename $(dir $(PWD)))
PROJ_PATH = $(abspath $(dir $(PWD)))/GCC
OUTPATH = $(PWD)/Build

ifndef FLAG_STORE_FOLDER
FLAG_STORE_FOLDER = $(OUTPATH)
endif

include $(FEATURE)

# LD Flags
LDFLAGS = $(ALLFLAGS) $(FPUFLAGS) --specs=nano.specs -lnosys -nostartfiles -u _printf_float -u _scanf_float
LDFLAGS += -Wl,-wrap=malloc -Wl,-wrap=calloc -Wl,-wrap=realloc -Wl,-wrap=free
LDFLAGS += -Wl,-wrap=memcpy -Wl,-wrap=memset -Wl,-wrap=memmove -Wl,-wrap=memcmp -Wl,-wrap=strcpy -Wl,-wrap=strcmp -Wl,-wrap=strlen -Wl,-wrap=fprintf
LDFLAGS += -Wl,-wrap=_printf_i

ifeq ($(ULS_FUNCTION_PROFILER),y)
LDFLAGS += -Wl,-Tmt2625_flash_profiler.ld -Wl,--gc-sections
else
LDFLAGS += -Wl,-Tmt2625_flash_bring_up.ld -Wl,--gc-sections
endif

ifeq ($(DEBUG_IO), 1)
LDFLAGS += --specs=rdimon.specs -lrdimon
CFLAGS += -DDEBUG_IO
endif

ifeq ($(ULS_FUNCTION_PROFILER), y)
CFLAGS += -DULS_FUNCTION_PROFILER
CFLAGS += -DULS_FUNCTION_PROFILER_EXIT
endif
#######################################################
# Include Module

ifneq ($(MTK_SYSTEM_CLOCK_SET), )
CFLAGS += -DMTK_SYSTEM_CLOCK_SET=$(MTK_SYSTEM_CLOCK_SET)
endif

#include $(FEATURE)
# Gloabl Config
-include $(SOURCE_DIR)/.config
# IC Config
-include $(SOURCE_DIR)/config/chip/$(IC_CONFIG)/chip.mk
# Board Config
#-include $(SOURCE_DIR)/config/board/$(BOARD_CONFIG)/board.mk

# EPT Config
-include $(SOURCE_DIR)/driver/board/mt2625_hdk/ept/module.mk

ifeq ($(MTK_USB_DEMO_ENABLED), y)
# add USB CLASS files
include $(SOURCE_DIR)/middleware/MTK/usb/module.mk
endif


# Sources or Lib
# RTOS source files
include $(SOURCE_DIR)/kernel/rtos/FreeRTOS/module.mk

# kernel service files
include $(SOURCE_DIR)/kernel/service/module.mk

# HAL driver files
include $(SOURCE_DIR)/driver/chip/mt2625/module.mk


# Httpclient files
include $(SOURCE_DIR)/middleware/third_party/httpclient/module.mk

# lwip files
include $(SOURCE_DIR)/middleware/third_party/lwip/module.mk

# ping files
include $(SOURCE_DIR)/middleware/third_party/ping/module.mk

# iperf files
include $(SOURCE_DIR)/middleware/MTK/iperf/module.mk

#include mbedtls
include $(SOURCE_DIR)/middleware/third_party/mbedtls/module.mk

#include er-coap-13
include $(SOURCE_DIR)/middleware/third_party/er-coap-13/module.mk

# lwm2m files
ifeq ($(MTK_LWM2M_SUPPORT), y)
CFLAGS += -DMTK_LWM2M_SUPPORT
CFLAGS += -DMTK_SENSOR_APP_LWM2M
include $(SOURCE_DIR)/middleware/third_party/lwm2m/module.mk

ifeq ($(MTK_LWM2M_CT_SUPPORT), y)
CFLAGS += -DMTK_LWM2M_CT_SUPPORT
endif

ifeq ($(MTK_LWM2M_WITH_DTLS_SUPPORT), y)
CFLAGS += -DMTK_LWM2M_WITH_DTLS_SUPPORT
endif

#include tinydtls
include $(SOURCE_DIR)/middleware/third_party/tinydtls/module.mk
endif #MTK_LWM2M_SUPPORT

# mqtt files
include $(SOURCE_DIR)/middleware/third_party/mqtt/module.mk

# sntp files
include $(SOURCE_DIR)/middleware/third_party/sntp/module.mk

# MUX AP files
include $(SOURCE_DIR)/middleware/MTK/mux_ap/module.mk

# KeyPad customer files
include $(SOURCE_DIR)/driver/board/mt2625_hdk/keypad/module.mk

# add ATCI files
include $(SOURCE_DIR)/middleware/MTK/atci/module.mk

# add FOTA files
include $(SOURCE_DIR)/middleware/MTK/fota/module.mk

#include lwip
include $(SOURCE_DIR)/middleware/third_party/lwip/module.mk

#include sntp
include $(SOURCE_DIR)/middleware/third_party/sntp/module.mk

# SIMAT Proxy
include $(SOURCE_DIR)/middleware/MTK/simat_proxy/module.mk

#include nidd
include $(SOURCE_DIR)/middleware/MTK/nidd/module.mk

#include ril
include $(SOURCE_DIR)/middleware/MTK/ril/module.mk

#include AP Bridge Proxy
include $(SOURCE_DIR)/middleware/MTK/apb_proxy/module.mk

#include tel_conn_mgr
include $(SOURCE_DIR)/middleware/MTK/tel_conn_mgr/module.mk

include $(SOURCE_DIR)/middleware/third_party/cjson/module.mk

ifeq ($(RS_FOTA_SUPPORT), y)
include $(SOURCE_DIR)/middleware/third_party/rsfota/rsdl/module.mk
CFLAGS += -D__RS_FOTA_SUPPORT__
endif

ifeq ($(MTK_ONENET_SUPPORT), y)
#include cis_onenet
CFLAGS += -DMTK_ONENET_SUPPORT 
include $(SOURCE_DIR)/middleware/third_party/cis_onenet/module.mk

#include cis_onenet_example
include $(SOURCE_DIR)/middleware/third_party/cis_onenet/sample/module.mk
endif #MTK_ONENET_SUPPORT

#include libcoap
ifeq ($(MTK_COAP_SUPPORT), y)
include $(SOURCE_DIR)/middleware/third_party/libcoap/module.mk
endif

# NVDM files
ifeq ($(MTK_NVDM_ENABLE), y)
CFLAGS += -DMTK_NVDM_ENABLE
include $(SOURCE_DIR)/middleware/MTK/nvdm/module.mk
endif

# NVDM MODEM files
ifeq ($(MTK_NVDM_MODEM_ENABLE), y)
CFLAGS += -DMTK_NVDM_MODEM_ENABLE
include $(SOURCE_DIR)/middleware/MTK/nvdm_modem/module.mk
endif

# MD files
ifeq ($(MTK_NB_MODEM_ENABLE), y)
CFLAGS += -DMTK_NB_MODEM_ENABLE
include $(SOURCE_DIR)/middleware/MTK/nbiot_custom/module.mk
ifneq ($(wildcard $(strip $(SOURCE_DIR))/middleware/MTK/nbiot/),)
include $(SOURCE_DIR)/middleware/MTK/nbiot/module.mk
else
include $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/module.mk
endif
endif

ifeq ($(MTK_PORT_SERVICE_ENABLE), y)
include $(SOURCE_DIR)/middleware/MTK/port_service/module.mk
endif

ifeq ($(MTK_TOOL_AUTHENTICATION_ENABLE), y)
ifneq ($(wildcard $(strip $(SOURCE_DIR))/middleware/MTK/tool_authentication/src_protected/),)
include $(SOURCE_DIR)/middleware/MTK/tool_authentication/src_protected/GCC/module.mk
else
include $(SOURCE_DIR)/prebuilt/middleware/MTK/tool_authentication/module.mk
endif
endif

ifeq ($(MTK_SWLA_ENABLE), y)
CFLAGS += -DMTK_FW_VERSION=\"$(MTK_FW_VERSION)\"
CFLAGS += -DPRODUCT_VERSION_STR=\"$(PRODUCT_VERSION)\"
endif

ifeq ($(MTK_AUTO_BAUD_RATE_ENABLE), y)
CFLAGS += -DMTK_AUTO_BAUD_RATE_ENABLE
endif

ifeq ($(MTK_RF_PA_RF3515), y)
CFLAGS += -DMTK_RF_PA_RF3515
endif

ifeq ($(MTK_RF_PA_QUAR), y)
CFLAGS += -DMTK_RF_PA_QUAR
endif

ifeq ($(USE_HAIWEI_RAWDATA_PLUGIN), y)
CFLAGS += -DUSE_HAIWEI_RAWDATA_PLUGIN
endif

#tangfeng.add
ifeq ($(MTK_N256_PRJ_NAME), y)
CFLAGS += -DMTK_N256_PRJ_NAME
endif

ifeq ($(MTK_N102_PRJ_NAME), y)
CFLAGS += -DMTK_N102_PRJ_NAME
endif

ifeq ($(MTK_N301_PRJ_NAME), y)
CFLAGS += -DMTK_N301_PRJ_NAME
APP_FILES += $(APP_PATH_SRC)/gps/apb_proxy_gps_cmd.c
APP_FILES += $(APP_PATH_SRC)/gps/td1030.c
CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/src/gps
endif

ifeq ($(RINLINK_SOCKET_SEND_RAW_CHAR_SUPPORT), y)
CFLAGS += -DRINLINK_SOCKET_SEND_RAW_CHAR_SUPPORT
endif

ifeq ($(MTK_GN5100_GIZWITS_PRJ_NAME), y)
CFLAGS += -DMTK_GN5100_GIZWITS_PRJ_NAME
endif

ifeq ($(LLLWM2M_DEMO_SUPPORT), y)
CFLAGS += -DLLLWM2M_DEMO_SUPPORT
APP_FILES += $(APP_PATH_SRC)/demo_lwm2m/LLDeviceTask.c
endif
ifeq ($(LLUDP_DEMO_SUPPORT), y)
CFLAGS += -DLLUDP_DEMO_SUPPORT
endif
ifeq ($(LLTCP_DEMO_SUPPORT), y)
CFLAGS += -DLLTCP_DEMO_SUPPORT
endif

ifeq ($(LL_AUTO_REG_NOT_RETRY), y)
CFLAGS += -DLL_AUTO_REG_NOT_RETRY
endif

#tangfeng.add
APP_FILES += $(APP_PATH_SRC)/rinlinkat/apb_proxy_rinlinkat_cmd.c
APP_FILES += $(APP_PATH_SRC)/common/LLCommon.c

#add for holley
APP_FILES += $(APP_PATH_SRC)/holley/HLWatchTask.c
APP_FILES += $(APP_PATH_SRC)/holley/HLCopyTask.c
APP_FILES += $(APP_PATH_SRC)/holley/HLLinkTask.c
APP_FILES += $(APP_PATH_SRC)/holley/HLSendTask.c
APP_FILES += $(APP_PATH_SRC)/holley/HLRecvTask.c
APP_FILES += $(APP_PATH_SRC)/holley/HLQueryTask.c
APP_FILES += $(APP_PATH_SRC)/holley/LLTcpNetworkTask.c
APP_FILES += $(APP_PATH_SRC)/holley/LLUdpNetworkTask.c


CFLAGS += -DMTK_QUERY_SDK_VERSION

OS_VERSION := $(shell uname)

#######################################################
# Main APP files

APP_PATH        = $(patsubst $(SDK_PATH)/%,%,$(abspath $(dir $(PWD))))

$(info $(SDK_PATH))
$(info $(PROJ_PATH))
$(info $(APP_PATH))

APP_PATH_SRC = $(APP_PATH)/src
APP_LWM2M_EXAMPLE_SRC = $(APP_PATH_SRC)/examples
APP_LWM2M_EXAMPLE_INC = $(SOURCE_DIR)/$(APP_LWM2M_EXAMPLE_SRC)

APP_FILES += $(APP_PATH_SRC)/main.c
APP_FILES += $(APP_PATH_SRC)/ept_gpio_var.c
APP_FILES += $(APP_PATH_SRC)/ept_eint_var.c
APP_FILES += $(APP_PATH_SRC)/sys_init.c
APP_FILES += $(APP_PATH_SRC)/system_mt2625.c
APP_FILES += $(APP_PATH_SRC)/apb_proxy_onenet_client_cmd.c
APP_FILES += $(APP_PATH_SRC)/apb_proxy_lwm2m_client_cmd.c

APP_FILES += $(APP_PATH_SRC)/md_init.c
APP_FILES += $(APP_PATH_SRC)/auto_register.c
APP_FILES += $(APP_PATH)/GCC/syscalls.c

C_FILES += $(APP_FILES)
C_FILES += $(APP_PATH_SRC)/app_state_machine.c
C_FILES += $(APP_PATH_SRC)/util.c

###########################################################


LIBS += $(SOURCE_DIR)/libs/adups.a
LIBS += $(SOURCE_DIR)/libs/cjson.a
LIBS += $(SOURCE_DIR)/libs/iperf.a
LIBS += $(SOURCE_DIR)/libs/mux_ap.a
LIBS += $(SOURCE_DIR)/libs/nvdm_core.a
LIBS += $(SOURCE_DIR)/libs/ril.a
LIBS += $(SOURCE_DIR)/libs/tinydtls.a
LIBS += $(SOURCE_DIR)/libs/apb_proxy.a
LIBS += $(SOURCE_DIR)/libs/er-coap-13.a
LIBS += $(SOURCE_DIR)/libs/libcoap.a
LIBS += $(SOURCE_DIR)/libs/nbiot_custom.a
LIBS += $(SOURCE_DIR)/libs/nvdm_modem.a
LIBS += $(SOURCE_DIR)/libs/simat_proxy.a
LIBS += $(SOURCE_DIR)/libs/usb.a
LIBS += $(SOURCE_DIR)/libs/atci.a
LIBS += $(SOURCE_DIR)/libs/fota.a
LIBS += $(SOURCE_DIR)/libs/mbedtls.a
LIBS += $(SOURCE_DIR)/libs/nidd.a
LIBS += $(SOURCE_DIR)/libs/ping.a
LIBS += $(SOURCE_DIR)/libs/sntp.a
LIBS += $(SOURCE_DIR)/libs/cis_onenet.a
LIBS += $(SOURCE_DIR)/libs/httpclient.a
LIBS += $(SOURCE_DIR)/libs/mqtt.a
LIBS += $(SOURCE_DIR)/libs/nvdm.a
LIBS += $(SOURCE_DIR)/libs/port_service.a
LIBS += $(SOURCE_DIR)/libs/tel_conn_mgr.a
LIBS += $(SOURCE_DIR)/libs/lwm2m.a

LIBS += $(SOURCE_DIR)/libs/service.a
LIBS += $(SOURCE_DIR)/libs/driver.a


###########################################################
#lwm2m client source code files

CFLAGS += -I$(APP_LWM2M_EXAMPLE_INC)
CFLAGS += -I$(APP_LWM2M_EXAMPLE_INC)/bootstrap_server
CFLAGS += -I$(APP_LWM2M_EXAMPLE_INC)/client
CFLAGS += -I$(APP_LWM2M_EXAMPLE_INC)/server
CFLAGS += -I$(APP_LWM2M_EXAMPLE_INC)/shared

C_FILES  += ${APP_LWM2M_EXAMPLE_SRC}/bootstrap_server/bootstrap_server.c     \
                  ${APP_LWM2M_EXAMPLE_SRC}/bootstrap_server/bootstrap_info.c       \
                  ${APP_LWM2M_EXAMPLE_SRC}/client/lwm2mclient.c                    \
                  ${APP_LWM2M_EXAMPLE_SRC}/client/system_api.c                     \
                  ${APP_LWM2M_EXAMPLE_SRC}/client/object_security.c                \
                  ${APP_LWM2M_EXAMPLE_SRC}/client/object_server.c                  \
                  ${APP_LWM2M_EXAMPLE_SRC}/client/object_device.c                  \
                  ${APP_LWM2M_EXAMPLE_SRC}/client/object_firmware.c                \
                  ${APP_LWM2M_EXAMPLE_SRC}/client/object_location.c                \
                  ${APP_LWM2M_EXAMPLE_SRC}/client/object_connectivity_moni.c       \
                  ${APP_LWM2M_EXAMPLE_SRC}/client/object_connectivity_stat.c       \
                  ${APP_LWM2M_EXAMPLE_SRC}/client/object_access_control.c          \
                  ${APP_LWM2M_EXAMPLE_SRC}/client/object_barometer.c               \
                  ${APP_LWM2M_EXAMPLE_SRC}/client/object_accelerometer.c           \
                  ${APP_LWM2M_EXAMPLE_SRC}/client/object_watermeter.c              \
                  ${APP_LWM2M_EXAMPLE_SRC}/client/test_object.c

C_FILES  += ${APP_LWM2M_EXAMPLE_SRC}/shared/commandline.c                    \
            ${APP_LWM2M_EXAMPLE_SRC}/lwm2m_cli.c

C_FILES  += ${APP_LWM2M_EXAMPLE_SRC}/server/lwm2mserver.c

#######################################################
# add startup file to build
S_FILES += $(APP_PATH)/GCC/startup_mt2625.s
S_FILES += $(APP_PATH)/GCC/startup_md.s

C_OBJS = $(C_FILES:%.c=$(BUILD_DIR)/%.o)
CXX_OBJS        = $(CXX_FILES:%.cpp=$(BUILD_DIR)/%.o)
S_OBJS = $(S_FILES:%.s=$(BUILD_DIR)/%.o)

#######################################################
# Include path

CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/inc
CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/psram/inc
CFLAGS += -I$(SOURCE_DIR)/driver/CMSIS/Include
CFLAGS += -I$(SOURCE_DIR)/driver/CMSIS/Device/MTK/mt2625/Include
CFLAGS += -I$(SOURCE_DIR)/driver/chip/mt2625/inc
CFLAGS += -I$(SOURCE_DIR)/driver/chip/inc
CFLAGS += -I$(SOURCE_DIR)/kernel/service/inc
#CFLAGS += -I$(SOURCE_DIR)/driver/board/mt25x3_hdk/ept/inc
CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/src/common

#######################################################



###########################################################
ifeq ($(ADUPS_FOTA_ENABLE),y)
CFLAGS         += -DADUPS_FOTA_ENABLE
include $(SOURCE_DIR)/middleware/third_party/adups/download/normal/module.mk
LIBS += $(SOURCE_DIR)/middleware/third_party/adups/download/normal/lib/adups_hal.a
endif

ifeq ($(ADUPS_FOTA_ENABLE_CT),y)
CFLAGS         += -DADUPS_FOTA_ENABLE_CT
CFLAGS         += -DADUPS_FOTA_ENABLE_CT_PLUGIN
include $(SOURCE_DIR)/middleware/third_party/adups/download/dianxin/module.mk
endif


###########################################################


# Rules

.PHONY: proj clean $(MODULE_PATH)

all: cleanlog proj nvdm_database
	@mkdir -p $(BUILD_DIR)
	@$(SIZE) $(OUTPATH)/$(PROJ_NAME).elf
	@$(SOURCE_DIR)/tools/scripts/build/copy_firmware.sh $(SOURCE_DIR) $(OUTPATH) $(IC_CONFIG) $(BOARD_CONFIG) $(PROJ_NAME).bin $(PWD) $(MTK_SECURE_BOOT_ENABLE) $(MTK_ANTI_CLONE_ENABLE)

ifeq ($(MTK_SECURE_BOOT_ENABLE),y)
	@$(SOURCE_DIR)/tools/scripts/build/build_sboot_header.sh $(SOURCE_DIR) $(OUTPATH) $(BOARD_CONFIG) $(PROJ_NAME).bin
endif

MOD_EXTRA = BUILD_DIR=$(BUILD_DIR) OUTPATH=$(OUTPATH) PROJ_PATH=$(PROJ_PATH)

$(LIBS): $(MODULE_PATH)

$(MODULE_PATH):
	@+make -C $@ $(MOD_EXTRA) $($@_EXTRA)

ifneq ($(wildcard $(strip $(SOURCE_DIR))/middleware/MTK/nbiot/),)
proj: $(OUTPATH)/$(PROJ_NAME).elf
else
proj: $(OUTPATH)/$(PROJ_NAME).elf prebuilt_dec_db
endif
silox_ext_db_gen:
	@echo "[customized silo db] # Copy Customized Silo database from $(SOURCE_DIR)/tools/scripts/silox/silo_db_ext.sox to $(OUTPATH)"
	@cp -f $(SOURCE_DIR)/tools/scripts/silox/silo_db_ext.sox $(OUTPATH) ;\
	if [ "$$?" != "0" ]; then \
		echo "[customized silo db] # Error: Failed to copy Customized Silo database file at $@" | tee -a $(ERR_LOG); exit 1; \
	fi
	@echo "[customized silo db] # Silo Customized database copied."	
	@echo "[customized silo db] # Update Customized Silo database with L4/ATCI src..."
ifneq ($(filter MINGW%,$(OS_VERSION)),)
	$(SOURCE_DIR)/tools/scripts/silox/Silox.exe -v -sox=$(OUTPATH)/silo_db_ext.sox -l "$(SOURCE_DIR)/tools/scripts/silox/Customized_SiloDB_srclist.txt" -lm -ig -modeasmodem -sm "$(OUTPATH)/siloMessage_ext.log" -r >>$(ERR_LOG) ; \
	if [ "$$?" != "0" ]; then \
		echo "[customized silo db] # Error: Failed to Update Customized Silo database with L4/ATCI src at $@" | tee -a $(ERR_LOG); exit 1; \
	fi
else
	$(WINE) $(SOURCE_DIR)/tools/scripts/silox/Silox.exe -v -sox=$(OUTPATH)/silo_db_ext.sox -l "$(SOURCE_DIR)/tools/scripts/silox/Customized_SiloDB_srclist.txt" -lm -ig -modeasmodem -sm "$(OUTPATH)/siloMessage_ext.log" -r -pwd "$(PWD)" >>$(ERR_LOG) ; \
	if [ "$$?" != "0" ]; then \
		echo "[customized silo db] # Error: Failed to Update Customized Silo database with L4/ATCI src at $@" | tee -a $(ERR_LOG); exit 1; \
	fi
endif
	@echo "[customized silo db] # Customized Silo database is updated."
	@echo "[customized silo db] # Remove the backup file of Customized Silo database..."
	@rm -f $(OUTPATH)/silo_db_ext.sox.backup ;\
	if [ "$$?" != "0" ]; then \
		echo "[customized silo db] # Error: Failed to removed the backup file of Customized Silo database at $@" | tee -a $(ERR_LOG); exit 1; \
	fi
	@echo "[customized silo db] # The backup file of Customized Silo database is removed."	

proj: nvdm_database

proj: cust_silo_db

$(OUTPATH)/$(PROJ_NAME).elf: $(C_OBJS) $(CXX_OBJS) $(S_OBJS) $(LIBS)
	@echo "export LDFLAGS"
	@echo $(LDFLAGS) > $(FLAG_STORE_FOLDER)/LDFLAGS.config
	@echo "export CFLAGS"
	@echo $(CFLAGS) > $(FLAG_STORE_FOLDER)/CFLAGS.config
	@echo "export end"
	@echo Linking...
	@if [ -e "$@" ]; then rm -f "$@"; fi
	@if [ -e "$(OUTPATH)/$(PROJ_NAME).map" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).map"; fi
	@if [ -e "$(OUTPATH)/$(PROJ_NAME).dis" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).dis"; fi
	@if [ -e "$(OUTPATH)/$(PROJ_NAME).sym" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).sym"; fi
	@if [ -e "$(OUTPATH)/$(PROJ_NAME).hex" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).hex"; fi
	@if [ -e "$(OUTPATH)/$(PROJ_NAME).bin" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).bin"; fi
	@echo $(patsubst $(SDK_PATH)/%,$(SOURCE_DIR)/%, $(LDFLAGS) -Wl,--whole-archive -Wl,--start-group $^ -Wl,--end-group -Wl,--no-whole-archive -Wl,-Map=$(patsubst $(SDK_PATH)/%,$(SOURCE_DIR)/%,$(OUTPATH)/$(PROJ_NAME).map) -lm -o $@) > $(OUTPATH)/link_option.tmp
	@$(CC) @$(OUTPATH)/link_option.tmp 2>>$(ERR_LOG)
	@rm -f $(OUTPATH)/link_option.tmp
ifeq ($(WITH_DIS), y)
	@$(OBJDUMP) -D $(OUTPATH)/$(PROJ_NAME).elf > $(OUTPATH)/$(PROJ_NAME).dis
endif
	@$(OBJCOPY) -O ihex $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).hex
	@$(OBJDUMP) -x $(OUTPATH)/$(PROJ_NAME).elf > $(OUTPATH)/$(PROJ_NAME).sym
	@$(OBJCOPY) -O binary $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).bin
	@echo Done

prebuilt_dec_db:
	@echo "restore prebuilt database..."
ifeq ($(MTK_AGNSS_SUPL2_ENABLE), y)
	@cp -af $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/db/feature_supl2/*.dec ${OUTPATH}/
else
	@cp -af $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/db/default/*.dec ${OUTPATH}/
endif
	@echo "restore done."

include $(SOURCE_DIR)/.rule.mk

clean:
	rm -rf $(OUTPATH)

NB_VERSION := "NB_VERSION"
#use command argv if exist
CUSTOMER_ID?=MTK_INTERNAL

nvdm_database: $(OUTPATH)/$(PROJ_NAME).elf
ifeq ($(MTK_NVDM_AUTOGEN_DATABASE), y)
	@$(SOURCE_DIR)/tools/scripts/nvdm_database_gen/gen_nvdm_db.sh $(CC) $(FLAG_STORE_FOLDER)/CFLAGS.config $(OUTPATH) $(SOURCE_DIR)/tools/scripts/nvdm_database_gen $(SOURCE_DIR)/middleware/MTK/nbiot/nvdm_internal "nvdm_database.EDB" $(NB_VERSION) $(CUSTOMER_ID)
endif

cust_silo_db: $(OUTPATH)/$(PROJ_NAME).elf
	@echo "[customized flox db] # check if the customized flox database file exists..."
# Delete the Existed Customized flox database
	@echo "## Delete the Existed Customized flox database ##"
	@if [ -f $(OUTPATH)/floggerBaseTransform_ext.flox ]; then \
		echo "[customized flox db] # Delete the Existed Customized flox database from $(OUTPATH)/floggerBaseTransform_ext.flox "; \
		rm -f $(OUTPATH)/floggerBaseTransform_ext.flox; \
		if [ "$$?" != "0" ]; then \
			echo "[customized flox db] # Error: Failed to delete the existed Customized Flox database file at $@" | tee -a $(ERR_LOG); exit 1; \
		fi; \
		echo "[customized flox db] # The Existed Customized Flox database deleted."; \
	fi
# Copy Customized flox database
	@echo "## Copy Customized flox database ##"
	@if [ -f $(OUTPATH)/floggerBaseTransform.flox ]; then \
		echo "[customized flox db] # Copy Customized flox database from $(OUTPATH)/floggerBaseTransform.flox to $(OUTPATH)"; \
		cp -f $(OUTPATH)/floggerBaseTransform.flox $(OUTPATH)/floggerBaseTransform_ext.flox; \
		if [ "$$?" != "0" ]; then \
			echo "[customized flox db] # Error: Failed to copy Customized Flox database file at $@" | tee -a $(ERR_LOG); exit 1; \
		fi; \
		echo "[customized flox db] # Customized Flox database copied."; \
	elif [ -f $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/db/floggerBaseTransform.flox ]; then \
		echo "[customized flox db] # Copy Customized flox database from $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/db/floggerBaseTransform.flox to $(OUTPATH)"; \
		cp -f $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/db/floggerBaseTransform.flox $(OUTPATH)/floggerBaseTransform_ext.flox; \
		if [ "$$?" != "0" ]; then \
			echo "[customized flox db] # Error: Failed to copy Customized Flox database file at $@" | tee -a $(ERR_LOG); exit 1; \
		fi; \
		echo "[customized flox db] # Customized Flox database copied."; \
	else \
		echo "Customized Flox database does not exist in $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/db/."; \
	fi
# [cust_silo_db] completed
	@echo "## cust_dilo_db has completed! ##"
