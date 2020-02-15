/*******************************************************************************
 *
 * Copyright (c) 2014 Bosch Software Innovations GmbH, Germany.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Bosch Software Innovations GmbH - Please refer to git log
 *
 *******************************************************************************/
/*
 * abup_lwm2mclient.h
 *
 *  General functions of lwm2m test client.
 *
 *  Created on: 22.01.2015
 *  Author: Achim Kraus
 *  Copyright (c) 2015 Bosch Software Innovations GmbH, Germany. All rights reserved.
 */

#ifndef LWM2MCLIENT_H_
#define LWM2MCLIENT_H_

#include "liblwm2m.h"
#include "connection.h"
extern int abup_greboot;

typedef struct
{
    lwm2m_object_t * securityObjP;
    lwm2m_object_t * serverObject;
    int sock;
#ifdef ADUPS_WITH_TINYDTLS
    dtls_connection_t * connList;
    lwm2m_context_t * lwm2mH;
#else
    connection_t * connList;
#endif
    int addressFamily;
} abup_client_data_t;


/*
 * object_device.c
 */
lwm2m_object_t * abup_get_object_device(void);
void abup_free_object_device(lwm2m_object_t * objectP);
uint8_t abup_device_change(lwm2m_data_t * dataArray, lwm2m_object_t * objectP);
void abup_display_device_object(lwm2m_object_t * objectP);
/*
 * object_firmware.c
 */
lwm2m_object_t * abup_get_object_firmware(void);
void abup_free_object_firmware(lwm2m_object_t * objectP);
void abup_display_firmware_object(lwm2m_object_t * objectP);
/*
 * object_location.c
 */
lwm2m_object_t * abup_get_object_location(void);
void abup_free_object_location(lwm2m_object_t * object);
void abup_display_location_object(lwm2m_object_t * objectP);
/*
 * object_test.c
 */
//#define TEST_OBJECT_ID 31024
//lwm2m_object_t * get_test_object(void);
//void free_test_object(lwm2m_object_t * object);
//void display_test_object(lwm2m_object_t * objectP);
/*
 * object_server.c
 */
lwm2m_object_t * abup_get_server_object(int serverId, const char* binding, int lifetime, bool storing);
void abup_clean_server_object(lwm2m_object_t * object);
void abup_display_server_object(lwm2m_object_t * objectP);
void abup_copy_server_object(lwm2m_object_t * objectDest, lwm2m_object_t * objectSrc);

/*
 * object_connectivity_moni.c
 */
//lwm2m_object_t * get_object_conn_m(void);
//void free_object_conn_m(lwm2m_object_t * objectP);
//uint8_t connectivity_moni_change(lwm2m_data_t * dataArray, lwm2m_object_t * objectP);

/*
 * object_connectivity_stat.c
 */
//extern lwm2m_object_t * get_object_conn_s(void);
//void free_object_conn_s(lwm2m_object_t * objectP);
//extern void conn_s_updateTxStatistic(lwm2m_object_t * objectP, uint16_t txDataByte, bool smsBased);
//extern void conn_s_updateRxStatistic(lwm2m_object_t * objectP, uint16_t rxDataByte, bool smsBased);

/*
 * object_access_control.c
 */
//lwm2m_object_t* acc_ctrl_create_object(void);
//void acl_ctrl_free_object(lwm2m_object_t * objectP);
//bool  acc_ctrl_obj_add_inst (lwm2m_object_t* accCtrlObjP, uint16_t instId,
//                 uint16_t acObjectId, uint16_t acObjInstId, uint16_t acOwner);
//bool  acc_ctrl_oi_add_ac_val(lwm2m_object_t* accCtrlObjP, uint16_t instId,
//                 uint16_t aclResId, uint16_t acValue);
/*
 * lwm2mclient.c
 */
void abup_handle_value_changed(lwm2m_context_t* lwm2mH, lwm2m_uri_t* uri, const char * value, size_t valueLength);
/*
 * system_api.c
 */
void abup_init_value_change(lwm2m_context_t * lwm2m);
void abup_system_reboot(void);

/*
 * object_security.c
 */
lwm2m_object_t * abup_get_security_object(int serverId, const char* serverUri, char * bsPskId, char * psk, uint16_t pskLen, bool isBootstrap);
void abup_clean_security_object(lwm2m_object_t * objectP);
void abup_display_security_object(lwm2m_object_t * objectP);
void abup_copy_security_object(lwm2m_object_t * objectDest, lwm2m_object_t * objectSrc);
int abup_lwm2mclient(void);
#endif /* LWM2MCLIENT_H_ */
