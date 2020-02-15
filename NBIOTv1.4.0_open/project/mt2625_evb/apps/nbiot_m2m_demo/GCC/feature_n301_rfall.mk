ifeq ($(PROJ_PATH),)
include feature.mk
else
include $(PROJ_PATH)/feature.mk
endif

MTK_N301_PRJ_NAME = y
MTK_RF_PA_RF3515 = n

