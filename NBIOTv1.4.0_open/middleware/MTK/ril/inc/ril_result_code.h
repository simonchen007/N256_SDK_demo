/* Copyright Statement:
 *
 * (C) 2005-2016  MediaTek Inc. All rights reserved.
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. ("MediaTek") and/or its licensors.
 * Without the prior written permission of MediaTek and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 * You may only use, reproduce, modify, or distribute (as applicable) MediaTek Software
 * if you have agreed to and been bound by the applicable license agreement with
 * MediaTek ("License Agreement") and been granted explicit permission to do so within
 * the License Agreement ("Permitted User").  If you are not a Permitted User,
 * please cease any access or use of MediaTek Software immediately.
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT MEDIATEK SOFTWARE RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

#ifndef __RIL_RESULT_CODE_H__
#define __RIL_RESULT_CODE_H__

#if defined (RIL_RESULT_CODE_NO_VERBOSE)
#define RESULT_CODE_ENTRY(_enumerate, _numeric, _text) \
{ _enumerate, _numeric }

#else
#define RESULT_CODE_ENTRY(_enumerate, _numeric, _text) \
{ _enumerate, _numeric, _text }
#endif

/* result code */
RESULT_CODE_ENTRY(  RIL_RESULT_CODE_OK,         0, "OK"          ),
                    RESULT_CODE_ENTRY(  RIL_RESULT_CODE_CONNECT,    1, "CONNECT"     ),
                    RESULT_CODE_ENTRY(  RIL_RESULT_CODE_RING,       2, "RING"        ),
                    RESULT_CODE_ENTRY(  RIL_RESULT_CODE_NO_CARRIER, 3, "NO CARRIER"  ),
                    RESULT_CODE_ENTRY(  RIL_RESULT_CODE_ERROR,      4, "ERROR"       ),
                    RESULT_CODE_ENTRY(  RIL_RESULT_CODE_INV,        5, "CONNECT 1200"),
                    RESULT_CODE_ENTRY(  RIL_RESULT_CODE_NO_DIALTONE, 6, "NO DIALTONE" ),
                    RESULT_CODE_ENTRY(  RIL_RESULT_CODE_BUSY,       7, "BUSY"        ),
                    RESULT_CODE_ENTRY(  RIL_RESULT_CODE_NO_ANSWER,  8, "NO ANSWER"   ),
                    RESULT_CODE_ENTRY(  RIL_RESULT_CODE_PROCEEDING, 9, "PROCEEDING"  ),
                    RESULT_CODE_ENTRY(  RIL_RESULT_CODE_FCERROR,    10, "+FCERROR"    ),
                    RESULT_CODE_ENTRY(  RIL_RESULT_CODE_CME_ERROR,  11, "+CME ERROR"    ),
                    RESULT_CODE_ENTRY(  RIL_RESULT_CODE_CMS_ERROR,  12, "+CMS ERROR"    ),
                    RESULT_CODE_ENTRY(  RIL_RESULT_CODE_NULL,       0, ""            ),

                    /* CME error codes */

                    /* general errors */

                    RESULT_CODE_ENTRY(  RIL_CME_PHONE_FAILURE,                   0, "phone failure"                                 ),
                    RESULT_CODE_ENTRY(  RIL_CME_NO_MS_CONNECTION,                1, "no connection to phone"                        ),
                    RESULT_CODE_ENTRY(  RIL_CME_PHONE_LINK_RESERVED,             2, "phone-adaptor link reserved"                   ),
                    RESULT_CODE_ENTRY(  RIL_CME_OPERATION_NOT_ALLOWED,           3, "operation not allowed"                         ),
                    RESULT_CODE_ENTRY(  RIL_CME_OPERATION_NOT_SUPPORTED,         4, "operation not supported"                       ),
                    RESULT_CODE_ENTRY(  RIL_CME_PH_SIM_PIN_REQUIRED,             5, "PH-SIM PIN required"                           ),
                    RESULT_CODE_ENTRY(  RIL_CME_FSIM_PIN_REQUIRED,               6, "PH-FSIM PIN required"                          ),
                    RESULT_CODE_ENTRY(  RIL_CME_FSIM_PUK_REQUIRED,               7, "PH-FSIM PUK required"                          ),
                    RESULT_CODE_ENTRY(  RIL_CME_SIM_NOT_INSERTED,               10, "SIM not inserted"                              ),
                    RESULT_CODE_ENTRY(  RIL_CME_SIM_PIN_REQUIRED,               11, "SIM PIN required"                              ),
                    RESULT_CODE_ENTRY(  RIL_CME_SIM_PUK_REQUIRED,               12, "SIM PUK required"                              ),
                    RESULT_CODE_ENTRY(  RIL_CME_SIM_FAILURE,                    13, "SIM failure"                                   ),
                    RESULT_CODE_ENTRY(  RIL_CME_SIM_BUSY,                       14, "SIM busy"                                      ),
                    RESULT_CODE_ENTRY(  RIL_CME_SIM_WRONG,                      15, "SIM wrong"                                     ),
                    RESULT_CODE_ENTRY(  RIL_CME_INCORRECT_PASSWORD,             16, "incorrect password"                            ),
                    RESULT_CODE_ENTRY(  RIL_CME_SIM_PIN2_REQUIRED,              17, "SIM PIN2 required"                             ),
                    RESULT_CODE_ENTRY(  RIL_CME_SIM_PUK2_REQUIRED,              18, "SIM PUK2 required"                             ),
                    RESULT_CODE_ENTRY(  RIL_CME_MEMORY_FULL,                    20, "memory full"                                   ),
                    RESULT_CODE_ENTRY(  RIL_CME_INVALID_INDEX,                  21, "invalid index"                                 ),
                    RESULT_CODE_ENTRY(  RIL_CME_NOT_FOUND,                      22, "not found"                                     ),
                    RESULT_CODE_ENTRY(  RIL_CME_MEMORY_FAILURE,                 23, "memory failure"                                ),
                    RESULT_CODE_ENTRY(  RIL_CME_LONG_TEXT,                      24, "text string too long"                          ),
                    RESULT_CODE_ENTRY(  RIL_CME_INVALID_TEXT_CHARS,             25, "invalid characters in text string"             ),
                    RESULT_CODE_ENTRY(  RIL_CME_LONG_DIALSTRING,                26, "dial string too long"                          ),
                    RESULT_CODE_ENTRY(  RIL_CME_INVALID_DIALSTRING_CHARS,       27, "invalid characters in dial string"             ),
                    RESULT_CODE_ENTRY(  RIL_CME_NO_NETWORK_SERVICE,             30, "no network service"                            ),
                    RESULT_CODE_ENTRY(  RIL_CME_NETWORK_TIMEOUT,                31, "network timeout"                               ),
                    RESULT_CODE_ENTRY(  RIL_CME_EMERGENCY_ONLY,                 32, "network not allowed - emergency calls only"    ),
                    RESULT_CODE_ENTRY(  RIL_CME_NET_PIN_REQUIRED,               40, "network personalisation PIN required"          ),
                    RESULT_CODE_ENTRY(  RIL_CME_NET_PUK_REQUIRED,               41, "network personalisation PUK required"          ),
                    RESULT_CODE_ENTRY(  RIL_CME_NETSUB_PIN_REQUIRED,            42, "network subset personalisation PIN required"   ),
                    RESULT_CODE_ENTRY(  RIL_CME_NETSUB_PUK_REQUIRED,            43, "network subset personalisation PUK required"   ),
                    RESULT_CODE_ENTRY(  RIL_CME_NETPROV_PIN_REQUIRED,           44, "service provider personalisation PIN required" ),
                    RESULT_CODE_ENTRY(  RIL_CME_NETPROV_PUK_REQUIRED,           45, "service provider personalisation PUK required" ),
                    RESULT_CODE_ENTRY(  RIL_CME_CORPORATE_PIN_REQUIRED,         46, "corporate personalisation PIN required"        ),
                    RESULT_CODE_ENTRY(  RIL_CME_CORPORATE_PUK_REQUIRED,         47, "corporate personalisation PUK required"        ),
                    RESULT_CODE_ENTRY(  RIL_CME_HIDDEN_KEY_REQUIRED,            48, "hidden key required"                           ),
                    RESULT_CODE_ENTRY(  RIL_CME_UNKNOWN,                       100, "unknown"                                       ),

                    /* PSD errors */
                    RESULT_CODE_ENTRY(  RIL_CME_ILLEGAL_MS,                    103, "illegal MS"                                    ),

                    RESULT_CODE_ENTRY(  RIL_CME_ILLEGAL_ME,                    106, "illegal ME"                                    ),
                    RESULT_CODE_ENTRY(  RIL_CME_PSD_SERVICES_NOT_ALLOWED,      107, "psd services not allowed"                      ),

                    RESULT_CODE_ENTRY(  RIL_CME_PLMN_NOT_ALLOWED,              111, "plmn not allowed"                              ),
                    RESULT_CODE_ENTRY(  RIL_CME_LOCATION_AREA_NOT_ALLOWED,     112, "location area not allowed"                     ),
                    RESULT_CODE_ENTRY(  RIL_CME_ROAMING_NOT_ALLOWED,           113, "roaming not allowed in this location area"     ),

                    RESULT_CODE_ENTRY(  RIL_CME_SERVICE_OPTION_NOT_SUPPORTED,  132, "service option not supported"                  ),
                    RESULT_CODE_ENTRY(  RIL_CME_SERVICE_OPTION_NOT_SUBSCRIBED, 133, "requested service option not subscribed"       ),
                    RESULT_CODE_ENTRY(  RIL_CME_SERVICE_OPTION_OUT_OF_ORDER,   134, "service option temporarily out of order"       ),

                    RESULT_CODE_ENTRY(  RIL_CME_UNSPECIFIED_PSD_ERROR,         148, "unspecified psd error"                         ),
                    RESULT_CODE_ENTRY(  RIL_CME_PDP_AUTHENTIFICATION_ERROR,    149, "PDP authentication failure"                    ),
                    RESULT_CODE_ENTRY(  RIL_CME_INVALID_MOBILE_CLASS,          150, "invalid mobile class"                          ),

                    /* LTE Specific errors - ESM */
                    RESULT_CODE_ENTRY(  RIL_CME_LAST_PDN_DISCONNECTION_NOT_ALLOWED,
                                        151, "PSD - last PDN disconnection not allowed"      ),
#if 0
                    /* 24.011 E.2 SMS error codes */
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_UNASSIGNED_NO,             1, "unassigned (unallocated) number"               ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_OPER_DETERM_BARRING,       8, "operator determined barring"                   ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_CALL_BARRED,              10, "call barred"                                   ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_REJECTED,                 21, "short message transfer rejected"               ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_DEST_OUT_OF_ORDER,        27, "destination out of service"                    ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_UNIDENTIFIED_SUBSCRIBER,  28, "unidentified subscriber"                       ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_FACILITY_REJECTED,        29, "facility rejected"                             ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_UNKNOWN_SUBSCRIBER,       30, "unknown subscriber"                            ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_NET_OUT_OF_ORDER,         38, "network out of order"                          ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_TEMP_FAILURE,             41, "temporary failure"                             ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_SWITCH_CONGESTION,        42, "congestion"                                    ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_RESOURCES_UNAV,           47, "resources unavailable, unspecified"            ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_REQ_FAC_NOT_SUBSCR,       50, "requested facility not subscribed"             ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_REQ_FACIL_NOT_IMPL,       69, "requested facility not implemented"            ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_INVALID_SM_TRANSFER_REF,  81, "invalid short message transfer reference value" ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_INVALID_MSG_SEMANTIC,     95, "invalid message, unspecified"                  ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_MAND_IE_ERROR,            96, "invalid mandatory information"                 ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_MSG_NONEXISTENT,          97, "message type non-existent or not implemented"  ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_MSG_GEN_ERROR,            98, "message not compatible with short message protocol state" ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_IE_NONEXISTENT,           99, "information element non-existent or not implemented" ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_PROTOCOL_ERROR,          111, "protocol error, unspecified"                   ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_INTERWORKING,            127, "interworking, unspecified"                     ),

                    /* 27.005 error codes */

                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_ME_FAILURE,              300, "ME failure"                                    ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_SMS_ME_RESERVED,         301, "SMS ME reserved"                               ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_OP_NOT_ALLOWED,          302, "operation not allowed"                         ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_OP_NOT_SUPPORTED,        303, "operation not supported"                       ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_INVALID_PDU_MODE_PARM,   304, "invalid PDU mode parameter"                    ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_INVALID_TEXT_MODE_PARM,  305, "invalid text mode parameter"                   ),

                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_SIM_NOT_INSERTED,        310, "SIM not inserted"                              ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_SIM_PIN_NECESSARY,       311, "SIM pin necessary"                             ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_PH_SIM_PIN_NECESSARY,    312, "PH SIM pin necessary"                          ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_SIM_FAILURE,             313, "SIM failure"                                   ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_SIM_BUSY,                314, "SIM busy"                                      ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_SIM_WRONG,               315, "SIM wrong"                                     ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_SIM_PUK_REQUIRED,        316, "SIM PUK required"                              ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_SIM_PIN2_REQUIRED,       317, "SIM PIN2 required"                             ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_SIM_PUK2_REQUIRED,       318, "SIM PUK2 required"                             ),

                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_MEMORY_FAILURE,          320, "memory failure"                                ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_INVALID_MEMORY_INDEX,    321, "invalid memory index"                          ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_MEMORY_FULL,             322, "memory full"                                   ),

                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_SMSC_ADDRESS_UNKNOWN,    330, "SMSC address unknown"                          ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_NO_NETWORK,              331, "no network"                                    ),
                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_NETWORK_TIMEOUT,         332, "network timeout"                               ),

                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_NO_CNMA_ACK_EXPECTED,    340, "no +CNMA acknowledgment expected"              ),

                    RESULT_CODE_ENTRY(  RIL_CMS_ERROR_UNKNOWN,                 500, "unknown"                                       ),
#endif


#endif /* __RIL_RESULT_CODE_H__ */

