ifeq ($(PROJ_PATH),)
include feature.mk
else
include $(PROJ_PATH)/feature.mk
endif

MTK_N301_PRJ_NAME = y
MTK_RF_PA_RF3515 = y
MTK_NB_CUSTOM_DIR = mt2625_nbiot_custom_rf3515
