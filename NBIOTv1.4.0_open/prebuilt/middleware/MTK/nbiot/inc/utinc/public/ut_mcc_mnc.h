/* Copyright Statement:
*
* (C) 2005-2017  MediaTek Inc. All rights reserved.
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
/*
 **************************************************************************/
/** \file
 *
 * Utility library for helper functions related to MCCs and MNCs.
 **************************************************************************/

#ifndef UT_MCC_MNC_H
#define UT_MCC_MNC_H

/***************************************************************************
 * Nested Include Files
 ***************************************************************************/

#if !defined (SYSTEM_H)
#include <system.h>
#endif

#if defined (__cplusplus)
extern "C" {
#endif

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/** Max Length of IMSI 
 */
#define  MAX_IMSI_LENGTH  8

/** USIM IMSI parity bit (see 3GPP TS 31.102)
 */
#define  IMSI_ODD_PARITY_FLAG   0x08

/** Position of the MCC1 in IMSI
 */
#define MCC_OFFSET      0
/** Position of the MNC2-MNC1 in IMSI
 */
#define MNC_OFFSET      2
/** Length of full PLMN name.
 */
#define SIM_PLMN_NAME_FULL_LENGTH   (26)
/** Length of short PLMN name. - 8+1 for trailing 0
 */
#define SIM_PLMN_NAME_ABBR_LENGTH   (9)
/** Length of country initials.
 */
#define COUNTRY_INITIALS_LENGTH     (4)
/** Length of full PLMN name.
 */
#define PLMN_NAME_FULL_LENGTH       (SIM_PLMN_NAME_FULL_LENGTH)
/** Length of short PLMN name.
 */
#define PLMN_NAME_ABBR_LENGTH       (SIM_PLMN_NAME_ABBR_LENGTH)
/** Length of format specifier.
 */
#define FORMAT_SPECIFIER_LENGTH     (14)
/** Number of user defined Plmns in the list stored in Nvram.
 */
#define NVRAM_PLMN_LIST_SIZE        (5)
/** Min Length of MNC 
 */
#define SIM_MIN_MNC_LENGTH             2
/** Max Length of MNC 
 */
#define SIM_MAX_MNC_LENGTH             3
/** UNKWW Length of MNC 
 */
#define SIM_UNKW_MNC_LENGTH            0

/** Defined operator PLMN, struct type was  struct Plmn, 
 *  {mcc,     mnc,   is_three_digit_mnc,      accessTechnology}
 */
 /* CMCC PLMN*/
#define CMCC_PLMN_46000 {0x0460, 0x0000,     FALSE,          EUTRAN_ACCESS_TECHNOLOGY}
#define CMCC_PLMN_46002 {0x0460, 0x0002,     FALSE,          EUTRAN_ACCESS_TECHNOLOGY}
#define CMCC_PLMN_46004 {0x0460, 0x0004,     FALSE,          EUTRAN_ACCESS_TECHNOLOGY}
#define CMCC_PLMN_46007 {0x0460, 0x0007,     FALSE,          EUTRAN_ACCESS_TECHNOLOGY}
#define CMCC_PLMN_46008 {0x0460, 0x0008,     FALSE,          EUTRAN_ACCESS_TECHNOLOGY}
#define CMCC_PLMN_46013 {0x0460, 0x0013,     FALSE,          EUTRAN_ACCESS_TECHNOLOGY}

/* CT PLMN*/
#define CT_PLMN_46003   {0x0460, 0x0003,     FALSE,          EUTRAN_ACCESS_TECHNOLOGY}
#define CT_PLMN_46005   {0x0460, 0x0005,     FALSE,          EUTRAN_ACCESS_TECHNOLOGY}
#define CT_PLMN_46011   {0x0460, 0x0011,     FALSE,          EUTRAN_ACCESS_TECHNOLOGY}


/* CU PLMN*/
#define CU_PLMN_46001   {0x0460, 0x0001,     FALSE,          EUTRAN_ACCESS_TECHNOLOGY}
#define CU_PLMN_46006   {0x0460, 0x0006,     FALSE,          EUTRAN_ACCESS_TECHNOLOGY}
#define CU_PLMN_46009   {0x0460, 0x0009,     FALSE,          EUTRAN_ACCESS_TECHNOLOGY}
/*SoftBank PLMN*/
#define SOFT_BANK_PLMN_44020 {0x0440, 0x0020,     FALSE,          EUTRAN_ACCESS_TECHNOLOGY}
/*Deutsche Telekom PLMN*/
#define DEUTSCHE_TELEKOM_PLMN_901040 {0x0901, 0x0040,     FALSE,   EUTRAN_ACCESS_TECHNOLOGY}
/* Thai operator */
#define THAI_TRUE_PLMN_52004   {0x0520, 0x0004,     FALSE,          EUTRAN_ACCESS_TECHNOLOGY}
#define THAI_AIS_PLMN_52003    {0x0520, 0x0003,     FALSE,          EUTRAN_ACCESS_TECHNOLOGY}

#define INVALID_OP_PLMN {0xFFFF, 0xFFFF,     FALSE,   EUTRAN_ACCESS_TECHNOLOGY}

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/** Type for Mobile Network Code
 */
typedef Int16 Mnc;

/** Type for Mobile Country Code
 */
typedef Int16 Mcc;

/** Type for access technology Id.
 * For coding of the access technology field, see ETSI TS 131 102.
 * A bitmap is used. the meaning of each bit is as follows
 * b=1 access technology selected
 * b=0 accesstechnology not selected
 *
 * first byte : bit 8 specifies whether UTRAN is selected.
 *              bit 7 specifies whether EUTRAN is selected.
 * second byte: bit 8 specifies whether GSM is selected
 *              bit 7 specifies whether GSM COMPACT is selected.
 */
typedef Int16 AccessTechnologyId;

/** Type for Public Land Mobile Network definition.
 */
typedef struct PlmnTag
{
  Mcc                mcc;                /**< Mobile Country Code. */
  Mnc                mnc;                /**< Mobile Network Code. */
  Boolean            is_three_digit_mnc; /**< Is MNC 3 digit */

  /* TODO: Remove rest of the structure elements */
  AccessTechnologyId accessTechnology; /**< Access Technology.   */
}
Plmn;

/** Type for PLMN with MNC length.
 */
typedef struct SimPlmnTag
{
   Plmn    plmn;                   /**< PLMN.                         */
   Boolean mncThreeDigitsDecoding; /**< Is three digit encoding used. */
}
SimPlmn;

/** PLMN name coding schemes.
 */
typedef enum PlmnNameCodingSchemeTag
{
  PLMN_CODING_DEFAULT, /**< Default coding scheme. */
  PLMN_CODING_UCS2     /**< UCS2 coding scheme.    */
}
PlmnNameCodingScheme;

/** Type for full network name.
 */
typedef Int8  FullName[ PLMN_NAME_FULL_LENGTH ];
/** Type for abbreviated network name.
 */
typedef Int8  AbbrName[ PLMN_NAME_ABBR_LENGTH ];
/** Type for country initials.
 */
typedef Int8  Initials[ COUNTRY_INITIALS_LENGTH ];
/** Type for format specifier.
 */
typedef Int8  FormatSpecifier[ FORMAT_SPECIFIER_LENGTH ];

/** Type for PLMN name.
 */
typedef struct PlmnNameTag
{
  PlmnNameCodingScheme plmnCoding; /**< Coding scheme for 'full', 'abbr' & 'initials'. */
  FullName             full;       /**< Full network name (long).                      */
  AbbrName             abbr;       /**< Abbreviated network name (short).              */
  Initials             initials;   /**< Country initials.                              */
  FormatSpecifier      format;     /**< Format specifier.                              */
}
PlmnName;

/** Type for PLMN Id.
 */
typedef struct PlmnIdTag
{
  Boolean       present;       /**< Is present.              
\assoc PRESENT \ref plmn
\assoc PRESENT \ref threeDigitMnc
\assoc PRESENT \ref plmnName */
  Plmn          plmn;          /**< PLMN.                    */
  Boolean       threeDigitMnc; /**< Is the MNC three digit.  */
  PlmnName      plmnName;      /**< PLMN Name.               */
}
PlmnId;

/** Type for country initials.
 */
typedef struct UtmmCountryInitialsTag
{
  Mcc       mcc;        /**< Mobile Country Code.  */
  Initials  initials;   /**< Country initials.     */
}
UtmmCountryInitials;

/** Type for network names.
 */
typedef struct UtmmNetworkNamesTag
{
  Plmn              plmn;       /**< PLMN = MNC + MCC.                 */
  AbbrName          abbr;       /**< Abbreviated network name (short). */
  FormatSpecifier   format;     /**< Format specifier.                 */
  FullName          full;       /**< Full network name (long).         */
}
UtmmNetworkNames;

/** Type for user defined PLMN data.
 */
typedef struct UserDefPlmnDataTag
{
  PlmnId  userDefPlmnDataList[NVRAM_PLMN_LIST_SIZE]; /**< User defined PLMN values. */
  Boolean userDefPlmnDataIsValid;                    /**< Is this store valid?      */
}
UserDefPlmnData;

/** IMSI data element. See 3GPP TS 31.102 for format */
typedef struct ImsiTag
{
  /** Length of data 
\assoc ARRAY \ref contents */
    Int8                            length;

  /** Holds IMSI data */
    Int8                            contents [MAX_IMSI_LENGTH];
}
Imsi;

/**************************************************************************
 * Function Prototypes
 **************************************************************************/

/** \defgroup ApiUtMccMnc MCC, MNC Utility Functions
 * \ingroup PrdGSMProtoStack
 * \ingroup Api
 * Common utility functions for MCC and MNC handling/translation.
 * Mobile Country Code (MCC) and Mobile Network Code (MNC) may need to be
 * translated into country initials or network alias. This functionality is
 * required by applications as well as protocol stack. This group of APIs
 * provide related functionality to applications.
 * @{
 */

/** Look up a Network PLMN Name and country initials in the
 * constant look-up table (LUT). This table is doubly sorted:
 * firstly on MCC then on MNC.
 *
 * \param plmn_p [in]Pointer which specifies the MCC and MNC.
 * \param name_p [out]The memory pointed by this param is populated with the
 *                      output strings.
 * \retval None.
 */
void    utmmLookupPlmnName        (SimPlmn *plmn_p, PlmnName *name_p);

/** Look up a countrys' initials in the constant look-up table
 * (LUT). The initials are stored in the passed string. The
 * table is sorted on MCC.
 *
 * \param plmn_p     [in]Pointer which specifies the MCC.
 * \param initials_p [out]The memory pointed by this param is populated with the
 *                   output string.
 * \retval None.
 */
void    utmmLookupCountryInitials (Plmn *plmn_p, Char *initials_p);

 /** Compares PLMN received over air (may be 2/3 digit MNC) with
 * PLMN stored in MS.
 *
 * \param plmn1_p        [in]Pointer which specifies the first PLMN.
 * \param threeDigitMnc1 [in]Is three digit coding used?.
 * \param plmn2_p        [in]Pointer which specifies the second PLMN.
 * \param threeDigitMnc2 [in]Is three digit coding used?.
 * \retval Boolean TRUE if PLMNs match.
 */
Boolean utmmPlPlmnsMatch          (Plmn *plmn1_p, Boolean threeDigitMnc1,
                                   Plmn *plmn2_p, Boolean threeDigitMnc2);


/** Check whether Mcc is North American (USA or Canada)
 *
 * \param mcc        [in]Mobile Country Code.
 * \retval Boolean TRUE if MCC is a North American MCC.
 */
Boolean utmmPlIsNorthAmericanMcc  (Mcc mcc);

/** Get the HPLMN from the IMSI. The length of MNC is known according to 
 * mncLength (if known) or MCC
 * The MCC list is updated according to 
 * "MOBILE NETWORK CODES (MNC) FOR THE INTERNATIONAL IDENTIFICATION PLAN FOR
 * PUBLIC NETWORKS AND SUBSCRIPTIONS" 
 * (http://www.itu.int/ITU-T/inr/forms/mnc.html)
 *
 * \param imsi        [in]  (Imsi)      International Mobile Subscriber Identity
 * \param mncLength   [in]  (Int8)      From EFad field. If not present, the length depends of the MCC
 * \param hPlmn       [out] (SimPlmn *) Field (Mnc) mnc, (Mcc) mcc and (Boolean) mncThreeDigitsDecoding
 * \retval none
 */
void utmmGetHplmnFromSim(Imsi imsi, Int8 mncLength, SimPlmn * hPlmn);

/** @} */ /* End of ApiUtMccMnc */

#if defined (__cplusplus)
}
#endif
#endif /* UT_MCC_MNC_H */

/* END OF FILE */


