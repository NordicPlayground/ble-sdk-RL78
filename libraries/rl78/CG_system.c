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
  /*Set all pins for crystal as normal I/O */
  CMC = _00_CG_HISYS_PORT | _00_CG_SUB_PORT | _00_CG_SYSOSC_DEFAULT | _00_CG_SUBMODE_DEFAULT;
  
  // MSTOP = 1U;   /*Stop X1 */
  /* Set fSUB */
  //XTSTOP = 1U;  /*Stop fSUB */
  /* Set fIH */
  //HIOSTOP = 0U; /*Run On-Chip Oscillator */
  CSC = _80_CG_HISYS_STOP | _40_CG_SUB_STOP | _00_CG_HIO_OPER;

  /* Set fMAIN */
  //MCM0 = 0U;    /*Select HOCO as fMAIN */
  /* Set fCLK */
  //CSS = 0U;     /*Select fMAIN as CPU and peripheral clock */
  CKC = _00_CG_CPUCLK_SELMAIN | _00_CG_MAINCLK_SELHIO;

  OSMC = _10_CG_RTC_CLK_FIL;      /*Select Low-speed on-chip oscillator for RTC and Interval timer */
}