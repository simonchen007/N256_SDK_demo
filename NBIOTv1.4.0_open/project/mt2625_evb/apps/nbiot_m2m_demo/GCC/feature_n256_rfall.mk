ifeq ($(PROJ_PATH),)
include feature.mk
else
include $(PROJ_PATH)/feature.mk
endif

MTK_N256_PRJ_NAME = y
MTK_RF_PA_RF3515 = n

#for saving power ,auto register can not retry
LL_AUTO_REG_NOT_RETRY = y

