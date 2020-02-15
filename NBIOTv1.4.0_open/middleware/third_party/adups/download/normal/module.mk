APP_PATH_SRC_HAL=middleware/third_party/adups/download/normal/source/hal/src
APP_PATH_INC_HAL=middleware/third_party/adups/download/normal/source/hal/inc
APP_PATH_SRC_CUSTOM=middleware/third_party/adups/download/normal/source/custom/src
APP_PATH_INC_CUSTOM=middleware/third_party/adups/download/normal/source/custom/inc
#CFLAGS += -DLWM2M_CLIENT_MODE
#CFLAGS += -DLWM2M_BOOTSTRAP

#ifneq ($(ADUPS_FOTA_LIB_ENABLE),y)
#C_FILES += $(APP_PATH_SRC_HAL)/abup_lwm2mclient.c
#C_FILES += $(APP_PATH_SRC_HAL)/abup_device.c
#C_FILES += $(APP_PATH_SRC_HAL)/abup_firmware.c
#C_FILES += $(APP_PATH_SRC_HAL)/abup_security.c
#C_FILES += $(APP_PATH_SRC_HAL)/abup_server.c

#C_FILES += $(APP_PATH_SRC_HAL)/abup_md5.c
#C_FILES += $(APP_PATH_SRC_HAL)/abup_HMD5.c
#C_FILES += $(APP_PATH_SRC_HAL)/abup_stdlib.c
#C_FILES += $(APP_PATH_SRC_HAL)/abup_timer.c
#C_FILES += $(APP_PATH_SRC_HAL)/abup_socket.c
#endif

LIBS += $(SOURCE_DIR)/middleware/third_party/adups/download/normal/lib/adups_hal.a

#C_FILES += $(APP_PATH_SRC_HAL)/abup_flash.c
#C_FILES += $(APP_PATH_SRC_HAL)/abup_thread.c

#C_FILES += $(APP_PATH_SRC_CUSTOM)/abup_config.c
#C_FILES += $(APP_PATH_SRC_CUSTOM)/abup_system_api.c
#C_FILES += $(APP_PATH_SRC_CUSTOM)/abup_lwm2m_api.c
#C_FILES += $(APP_PATH_SRC_CUSTOM)/abup_user.c


CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/lwm2m/core
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/lwm2m/core/shared
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/lwm2m/core/er-coap-13
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/adups/bl_4.0/wosun/inc
CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH_INC_HAL)
CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH_INC_CUSTOM)