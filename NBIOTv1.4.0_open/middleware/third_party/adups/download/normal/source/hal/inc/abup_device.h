/*
 * This is an OpenSSL-compatible implementation of the RSA Data Security, Inc.
 * MD5 Message-Digest Algorithm (RFC 1321).
 *
 * Homepage:
 * http://openwall.info/wiki/people/solar/software/public-domain-source-code/md5
 *
 * Author:
 * Alexander Peslyak, better known as Solar Designer <solar at openwall.com>
 *
 * This software was written by Alexander Peslyak in 2001.  No copyright is
 * claimed, and the software is hereby placed in the public domain.
 * In case this attempt to disclaim copyright and place the software in the
 * public domain is deemed null and void, then the software is
 * Copyright (c) 2001 Alexander Peslyak and it is hereby released to the
 * general public under the following terms:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 *
 * See md5.c for more information.
 */

#ifndef _OBJECT_DEVICE_H
#define _OBJECT_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#define ABUP_PRV_POWER_SOURCE_1    1
#define ABUP_PRV_POWER_SOURCE_2    5
#define ABUP_PRV_POWER_VOLTAGE_1   3800
#define ABUP_PRV_POWER_VOLTAGE_2   5000
#define ABUP_PRV_POWER_CURRENT_1   125
#define ABUP_PRV_POWER_CURRENT_2   900
#define ABUP_PRV_BATTERY_LEVEL     100
#define ABUP_PRV_MEMORY_FREE       15
#define ABUP_PRV_ERROR_CODE        0
#define ABUP_PRV_TIME_ZONE         "Europe/Berlin"
#define ABUP_PRV_BINDING_MODE      "U"

#define ABUP_PRV_OFFSET_MAXLEN   7 //+HH:MM\0 at max
#define ABUP_PRV_TLV_BUFFER_SIZE 128

// Resource Id's:
#define ABUP_RES_O_MANUFACTURER          0
#define ABUP_RES_O_MODEL_NUMBER          1
#define ABUP_RES_O_SERIAL_NUMBER         2
#define ABUP_RES_O_FIRMWARE_VERSION      3
#define ABUP_RES_M_REBOOT                4
#define ABUP_RES_O_FACTORY_RESET         5
#define ABUP_RES_O_AVL_POWER_SOURCES     6
#define ABUP_RES_O_POWER_SOURCE_VOLTAGE  7
#define ABUP_RES_O_POWER_SOURCE_CURRENT  8
#define ABUP_RES_O_BATTERY_LEVEL         9
#define ABUP_RES_O_MEMORY_FREE           10
#define ABUP_RES_M_ERROR_CODE            11
#define ABUP_RES_O_RESET_ERROR_CODE      12
#define ABUP_RES_O_CURRENT_TIME          13
#define ABUP_RES_O_UTC_OFFSET            14
#define ABUP_RES_O_TIMEZONE              15
#define ABUP_RES_M_BINDING_MODES         16
// since TS 20141126-C:
#define ABUP_RES_O_DEVICE_TYPE           17
#define ABUP_RES_O_HARDWARE_VERSION      18
#define ABUP_RES_O_SOFTWARE_VERSION      19
#define ABUP_RES_O_BATTERY_STATUS        20
#define ABUP_RES_O_MEMORY_TOTAL          21

#define ABUP_R_PRODUCT_ID			100
#define ABUP_R_DEVICE_ID			101
#define ABUP_R_PLATFORM				102
#define ABUP_R_TIME_STAMP			103
#define ABUP_R_SIGN					104
#define ABUP_R_SDK_VERSION			105
#define ABUP_R_APK_VERSION			106
#define ABUP_R_NETWORK_TYPE			107
#define ABUP_O_MAC					108
#define ABUP_O_LAC					109
#define ABUP_O_CID					110
#define ABUP_0_MCC					111
#define ABUP_0_MNC					112
#define ABUP_O_RXLEV				113
#define ABUP_DEVICE_STATUS			200



#ifdef __cplusplus
}
#endif

#endif
