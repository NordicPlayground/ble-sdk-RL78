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
* Filename: CG_systeminit.c
* Abstract: This file implements system initializing function.
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
#include "CG_serial.h"
#include "CG_timer.h"
#include "CG_port.h"

/*
*******************************************************************************
**  Global define
*******************************************************************************
*/
int __low_level_init(void);
void systeminit(void);

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function initializes every macro.
**
**  Parameters:
**  None
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/
void systeminit(void)
{
  PIOR = 0x00U;
  PORT_Init();
  CLOCK_Init();
  TAU0_Init();
  SAU0_Init();
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function initializes hardware setting.
**
**  Parameters:
**  None
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/
int __low_level_init(void)
{
  
  DI();
  systeminit();
  EI();
  
  return MD_TRUE;
}
