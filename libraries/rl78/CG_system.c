/*
*******************************************************************************
* Copyright(C) 2011 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation. 
*
* This device driver was created by Applilet3 for RL78/G13
* 16-Bit Single-Chip Microcontrollers
* Filename: CG_system.c
* Abstract: This file implements device driver for System module.
* APIlib: Applilet3 for RL78/G13 E1.00b [14 Jan 2011]
* Device: R5F100LE
* Compiler: IAR Systems iccrl78
* Creation date:  25/10/2014
*******************************************************************************
*/

/*
*******************************************************************************
** Include files
*******************************************************************************
*/

#include "CG_macrodriver.h"
#include "CG_system.h"

/*
*******************************************************************************
**  Global define
*******************************************************************************
*/

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function initializes the clock generator.
**
**  Parameters:
**  None
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/
void CLOCK_Init(void)
{
  /* Set fMX */
  CMC = _00_CG_HISYS_PORT | _00_CG_SUB_PORT | _00_CG_SYSOSC_DEFAULT | _00_CG_SUBMODE_DEFAULT;   //Set all pins for crystal as normal I/O
  
  //MSTOP = 1U;   //Stop X1
  /* Set fSUB */
  //XTSTOP = 1U;  //Stop fSUB
  /* Set fIH */
  //HIOSTOP = 0U; //Run On-Chip Oscillator
  CSC = _80_CG_HISYS_STOP | _40_CG_SUB_STOP | _00_CG_HIO_OPER;

  /* Set fMAIN */
  //MCM0 = 0U;    //Select HOCO as fMAIN
  /* Set fCLK */
  //CSS = 0U;     //Select fMAIN as CPU and peripheral clock
  CKC = _00_CG_CPUCLK_SELMAIN | _00_CG_MAINCLK_SELHIO;

  OSMC = _10_CG_RTC_CLK_FIL;      //Select Low-speed on-chip oscillator for RTC and Interval timer
}