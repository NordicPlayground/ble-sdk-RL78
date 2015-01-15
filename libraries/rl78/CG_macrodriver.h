/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/


#ifndef _MDSTATUS_
#define _MDSTATUS_
/*
*******************************************************************************
**  Include files
*******************************************************************************
*/
#include "ior5f100le.h"
#include "ior5f100le_ext.h"
#include "intrinsics.h"
/*
*******************************************************************************
**  Register bit define
*******************************************************************************
*/
/*
*******************************************************************************
**  Macro define
*******************************************************************************
*/
#define DI    __disable_interrupt
#define EI    __enable_interrupt
#define HALT  __halt
#define NOP   __no_operation
#define STOP  __stop

/* Data type definition */
typedef unsigned long  ULONG;
typedef signed long    SLONG;

typedef unsigned int   UINT;
typedef signed int     SINT;

typedef unsigned short USHORT;
typedef signed short   SHORT;

typedef unsigned char  UCHAR;
typedef signed char    SCHAR;

typedef unsigned char  BOOL;
typedef unsigned short MD_STATUS;

#define MD_ON     1U
#define MD_OFF    0U

#define MD_TRUE   1U
#define MD_FALSE  0U

#define MD_SET    1U
#define MD_CLEAR  0U

/* Status list definition */
#define MD_STATUSBASE      0x00U
#define MD_OK              (MD_STATUSBASE + 0x00U) /* register setting OK */
#define MD_RESET           (MD_STATUSBASE + 0x01U) /* reset input */
#define MD_SENDCOMPLETE    (MD_STATUSBASE + 0x02U) /* send data complete */
#define MD_OVF             (MD_STATUSBASE + 0x03U) /* timer count overflow */
#define MD_SPT             (MD_STATUSBASE + 0x04U) /* IIC stop */
#define MD_NACK            (MD_STATUSBASE + 0x05U) /* IIC no ACK */
#define MD_SLAVE_SEND_END  (MD_STATUSBASE + 0x06U) /* IIC slave send end */
#define MD_SLAVE_RCV_END   (MD_STATUSBASE + 0x07U) /* IIC slave receive end */
#define MD_MASTER_SEND_END (MD_STATUSBASE + 0x08U) /* IIC master send end */
#define MD_MASTER_RCV_END  (MD_STATUSBASE + 0x09U) /* IIC master receive end */
#define MD_BUSY            (MD_STATUSBASE + 0x0AU) /* IIC is busy */
#define MD_UNDEREXEC       (MD_STATUSBASE + 0x0BU) /* DMA transfer under execute */
#define MD_COMPLETED       (MD_STATUSBASE + 0x0CU) /* DMA transfer completed */
#define MD_BUSY1           (MD_STATUSBASE + 0x0DU) /* busy 1 */
#define MD_BUSY2           (MD_STATUSBASE + 0x0EU) /* busy 2 */

/* Error list definition */
#define MD_ERRORBASE       0x80U
#define MD_ERROR           (MD_ERRORBASE + 0x00U)  /* error */
#define MD_RESOURCEERROR   (MD_ERRORBASE + 0x01U)  /* no resource available */
#define MD_PARITYERROR     (MD_ERRORBASE + 0x02U)  /* UARTn parity error */
#define MD_OVERRUNERROR    (MD_ERRORBASE + 0x03U)  /* UARTn overrun error */
#define MD_FRAMEERROR      (MD_ERRORBASE + 0x04U)  /* UARTn frame error */
#define MD_ARGERROR        (MD_ERRORBASE + 0x05U)  /* error agrument input error */
#define MD_TIMINGERROR     (MD_ERRORBASE + 0x06U)  /* error timing operation error */
#define MD_SETPROHIBITED   (MD_ERRORBASE + 0x07U)  /* setting prohibited */
#define MD_DATAEXISTS      (MD_ERRORBASE + 0x08U)  /* data to be transferred next exists in TXBn register */
#define MD_ERROR1          (MD_ERRORBASE + 0x09U)  /* error 1 */
#define MD_ERROR2          (MD_ERRORBASE + 0x0AU)  /* error 2 */
#define MD_ERROR3          (MD_ERRORBASE + 0x0BU)  /* error 3 */
#define MD_ERROR4          (MD_ERRORBASE + 0x0CU)  /* error 4 */

/*
*******************************************************************************
**  Function define
*******************************************************************************
*/

#endif
