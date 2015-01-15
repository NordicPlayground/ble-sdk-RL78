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
#include "CG_timer.h"

/*
*******************************************************************************
**  Global define
*******************************************************************************
*/
volatile uint16_t delay_counter;


void TAU0_Channel0_Start(void);
void TAU0_Channel0_Stop(void);
void TAU0_Channel0_ChangeTimerCondition(USHORT regvalue);
__interrupt void MD_INTTM00(void);


/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function initializes the TAU0 module.
**
**  Parameters:
**  None
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/
void TAU0_Init(void)
{
  TAU0EN = 1U;    /* supplies input clock */
  TPS0 = _0005_TAU_CKM0_FCLK_5 | _0000_TAU_CKM1_FCLK_0 | _0000_TAU_CKM2_FCLK_1 | _0000_TAU_CKM3_FCLK_8;   
  /*Due to fCLK 32MHz. Pre-scaler selected fCKL/2^5 = 1 MHz for CKM0. Other CKMx ignored */
  /* Stop all channels */
  TT0 = _0001_TAU_CH0_STOP_TRG_ON | _0002_TAU_CH1_STOP_TRG_ON | _0004_TAU_CH2_STOP_TRG_ON | _0008_TAU_CH3_STOP_TRG_ON | _0010_TAU_CH4_STOP_TRG_ON | _0020_TAU_CH5_STOP_TRG_ON | _0040_TAU_CH6_STOP_TRG_ON | _0080_TAU_CH7_STOP_TRG_ON | _0200_TAU_CH1_H8_STOP_TRG_ON | _0800_TAU_CH3_H8_STOP_TRG_ON;
  /* Mask channel 0 interrupt */
  TMMK00 = 1U;  /* disable INTTM00 interrupt */
  TMIF00 = 0U;  /* clear INTTM00 interrupt flag */
  /* Mask channel 1 interrupt */
  TMMK01 = 1U;  /* disable INTTM01 interrupt */
  TMIF01 = 0U;  /* clear INTTM01 interrupt flag */
  /* Mask channel 1 higher 8 bits interrupt */
  TMMK01H = 1U; /* disable INTTM01H interrupt */
  TMIF01H = 0U; /* clear INTTM01H interrupt flag */
  /* Mask channel 2 interrupt */
  TMMK02 = 1U;  /* disable INTTM02 interrupt */
  TMIF02 = 0U;  /* clear INTTM02 interrupt flag */
  /* Mask channel 3 interrupt */
  TMMK03 = 1U;  /* disable INTTM03 interrupt */
  TMIF03 = 0U;  /* clear INTTM03 interrupt flag */
  /* Mask channel 3 higher 8 bits interrupt */
  TMMK03H = 1U; /* disable INTTM03H interrupt */
  TMIF03H = 0U; /* clear INTTM03H interrupt flag */
  /* Mask channel 4 interrupt */
  TMMK04 = 1U;  /* disable INTTM04 interrupt */
  TMIF04 = 0U;  /* clear INTTM04 interrupt flag */
  /* Mask channel 5 interrupt */
  TMMK05 = 1U;  /* disable INTTM05 interrupt */
  TMIF05 = 0U;  /* clear INTTM05 interrupt flag */
  /* Mask channel 6 interrupt */
  TMMK06 = 1U;  /* disable INTTM06 interrupt */
  TMIF06 = 0U;  /* clear INTTM06 interrupt flag */
  /* Mask channel 7 interrupt */
  TMMK07 = 1U;  /* disable INTTM07 interrupt */
  TMIF07 = 0U;  /* clear INTTM07 interrupt flag */
  /* Set INTTM00 low priority */
  TMPR100 = 1U;
  TMPR000 = 1U;
  /* Channel 0 used as interval timer */
  TMR00 = _0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_TRIGGER_SOFTWARE | _0000_TAU_MODE_INTERVAL_TIMER | _0000_TAU_START_INT_UNUSED;
  TDR00 = 0x03E7U;   /*999 counts plus 1. Each count es every 1us = 1ms */
  TO0 = _0000_TAU_CH0_OUTPUT_VALUE_0;
  TOE0 = _0000_TAU_CH0_OUTPUT_DISABLE;
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function starts TAU0 channel 0 counter.
**
**  Parameters:
**  None
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/
void TAU0_Channel0_Start(void)
{
  TMIF00 = 0U;  /* clear INTTM00 interrupt flag */
  TMMK00 = 0U;  /* enable INTTM00 interrupt */
  TS0 = _0001_TAU_CH0_START_TRG_ON;   /*Start Channel 0 from Timer Array 0 */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function stops TAU0 channel 0 counter.
**
**  Parameters:
**  None
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/
void TAU0_Channel0_Stop(void)
{
  TT0 |= _0001_TAU_CH0_STOP_TRG_ON;  /*Stop Channel 0 from Timer Array 0 */
  /* Mask channel 0 interrupt */
  TMMK00 = 1U;  /* disable INTTM00 interrupt */
  TMIF00 = 0U;  /* clear INTTM00 interrupt flag */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function changes TAU0 channel 0 register value.
**
**  Parameters:
**  regvalue: value for data register
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/
void TAU0_Channel0_ChangeTimerCondition(USHORT regvalue)
{
  TDR00 = regvalue;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function performs a delay based on the number of milliseconds as input.
**
**  Parameters:
**  None
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/
void delay(uint16_t number_ms)
{
  delay_counter = 0;
  TAU0_Channel0_Start();
  while(delay_counter < number_ms);
  TAU0_Channel0_Stop();
}


/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function is INTTM00 interrupt service routine.
**
**  Parameters:
**  None
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/

#pragma vector = INTTM00_vect
__interrupt void MD_INTTM00(void)
{
  delay_counter++;
}

