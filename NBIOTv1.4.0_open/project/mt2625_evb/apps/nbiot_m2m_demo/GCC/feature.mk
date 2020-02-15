IC_CONFIG                             = mt2625
BOARD_CONFIG                          = mt2625_evb

MTK_FW_VERSION                        = SDK_VER_IOT_SDK_DEMO
# HAL LOG
#MTK_HAL_PLAIN_LOG_ENABLE = y
#MTK_SUPPORT_SFC_DVT_ENABLE = y

# debug level: none, error, warning, and info
MTK_DEBUG_LEVEL = info

# customer parameter
MTK_NB_CUSTOM_DIR = mt2625_nbiot
ifeq ($(wildcard $(strip $(SOURCE_DIR))/middleware/MTK/nbiot_custom_rf/$(MTK_NB_CUSTOM_DIR)),)
MTK_NB_CUSTOM_DIR = mt2625_nbiot_custom
endif
# MD NBIOT flags
MTK_NB_MODEM_ENABLE = y
MTK_NBIOT_AP_BRIDGE_ENABLE = y
MTK_PPP_ENABLE = y

# temp ULS simulation
ULS_SIMULATOR_ENABLED = n

# Interface to Genie tool
FEATURE_EMMI_IN_USE = y

# SDK version
MTK_FW_VERSION                      = nbiot_m2m_demo

# NVDM
MTK_NVDM_ENABLE = y

ifeq ($(MTK_NVDM_ENABLE),y)
# Modem NVDM
MTK_NVDM_MODEM_ENABLE = y
MTK_MINI_DUMP_ENABLE = y
MTK_TOOL_AUTHENTICATION_ENABLE = y
MTK_NVDM_AUTOGEN_DATABASE = y
endif
# USB
MTK_USB_DEMO_ENABLED = y
MTK_PORT_SERVICE_ENABLE = y
MTK_PORT_SERVICE_CONFIGURABLE_BUFFER = y

#DCXO Calibration
MTK_DCXO_CAPID_CAL                     = n
MTK_DCXO_CAPID_LOAD_FROM_EFUSE         = n
MTK_DCXO_CAPID_STORE_TO_EFUSE          = n

#Base64 configration
MTK_MBEDTLS_CONFIG_FILE             = config-mtk-nbiot.h


# Include the AT Commands in ATCI.
MTK_ATCI_APB_PROXY_ADAPTER_ENABLE = y

#Port service for conn layer
MTK_CONNL_VIA_PORT_SERVICE_ENABLE = y

# Indicate if this is a dual mode project or not
MT2503_2625_DUAL = n

#SMT load support acti
MTK_BUILD_SMT_LOAD = y

# Support system at command
MTK_SYSTEM_AT_COMMAND_ENABLE = y

# Support CPU utilization through at command
MTK_OS_CPU_UTILIZATION_ENABLE = y

#swla enable
MTK_SWLA_ENABLE = n

#GNSS FOTA
MTK_GNSS_FOTA_ENABLE = n

#Project related customed AT command
MTK_APB_PROXY_PROJECT_COMMAND_ENABLE = y

#Enable Network AT command
MTK_ATCI_APB_PROXY_NETWORK_ENABLE = y

#L4mm enabling
ENABLE_L4MM_ALLOC = y

#GNSS Support
MTK_GNSS_ENABLE = n

#FOTA Support
MTK_FOTA_ENABLE = y

#ONENET support
MTK_ONENET_SUPPORT = y

#LWM2M support
MTK_LWM2M_SUPPORT = y
MTK_LWM2M_AT_CMD_SUPPORT = y
MTK_LWM2M_CT_SUPPORT = y

ULS_FUNCTION_PROFILER = n

MTK_HTTPCLIENT_SSL_ENABLE = y

#CoAP support
MTK_COAP_SUPPORT = y

#Secure boot
MTK_SECURE_BOOT_ENABLE  = n
MTK_ANTI_CLONE_ENABLE = n

#create default pnd data link
MTK_CREATE_DEFAULT_APN = y

#adups fota support
ADUPS_FOTA_ENABLE = y
ADUPS_FOTA_ENABLE_CT = n

#rsfota support
RS_FOTA_SUPPORT = n

#RIL test cases
MTK_RIL_UT_TEST_CASE_ENABLE = y

#LL ADD
MTK_RF_PA_RF3515 = n

USE_HAIWEI_RAWDATA_PLUGIN = y

#tangfeng.add from feature_lwm2m_ct.mk
MTK_LWM2M_CT_SUPPORT = y
MTK_LWM2M_WITH_DTLS_SUPPORT = n
MTK_RIL_UT_TEST_CASE_ENABLE = n 

#for saving power ,auto register can not retry
LL_AUTO_REG_NOT_RETRY = n

LLLWM2M_DEMO_SUPPORT = n
LLUDP_DEMO_SUPPORT = n
LLTCP_DEMO_SUPPORT = n
