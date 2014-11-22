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
* Filename: CG_port.c
* Abstract: This file implements device driver for PORT module.
* APIlib: Applilet3 for RL78/G13 E1.00b [14 Jan 2011]
* Device: R5F100LE
* Compiler: IAR Systems iccrl78
* Creation date:  26/10/2014
*******************************************************************************
*/

/*
*******************************************************************************
** Include files
*******************************************************************************
*/
#include "CG_macrodriver.h"
#include "CG_port.h"


/*
*******************************************************************************
**  Global define
*******************************************************************************
*/

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function initializes the Port I/O.
**
**  Parameters:
**  None
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/
void PORT_Init(void)
{
  //For the LED2
  P7 = _80_Pn7_OUTPUT_1;      
  PM7 = _01_PMn0_NOT_USE | _02_PMn1_NOT_USE | _04_PMn2_NOT_USE | _08_PMn3_NOT_USE | _10_PMn4_NOT_USE | _20_PMn5_NOT_USE | _40_PMn6_NOT_USE | _00_PMn7_MODE_OUTPUT;
  //For SPI lines of nRF8001
  P14 = _01_Pn0_OUTPUT_1 | _02_Pn1_OUTPUT_1;    
  PM14 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _40_PMn6_NOT_USE | _80_PMn7_MODE_INPUT | _3C_PM14_DEFAULT;
  PU14 = _80_PUn7_PULLUP_ON;
  PMC14 = _00_PMCn7_DI_ON | _7F_PMC14_DEFAULT;
  
  /*
  MOSI  Port1.2
  MISO  Port1.1
  SCLK  Port1.0
  REQN  Port14.1
  RDYN  Port14.7
  RESET Port14.0
  */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function implements a reading from an input pin
**
**  Parameters:
**  pin: pin to be read
**
**  Returns:
**  value read from the pin. Either 0 or 1
**
**-----------------------------------------------------------------------------
*/
uint8_t digitalRead(uint8_t pin)
{
  uint8_t result = 0xFF;
  
  //The only pin that will be use is ACI_RDYN
  switch (pin){
    case ACI_MOSI:
      result = (uint8_t)P1_bit.no2;
      break;
    case ACI_MISO:
      result = (uint8_t)P1_bit.no1;
      break;
    case ACI_SCLK:
      result = (uint8_t)P1_bit.no0;
      break;
    case ACI_REQN:
      result = (uint8_t)P14_bit.no1;
      break;
    case ACI_RDYN:
      result = (uint8_t)P14_bit.no7;
      break;
    case ACI_RESET:
      result = (uint8_t)P14_bit.no0;
      break;
    default:
      break;
  }
  return result;
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function implements a writing to an output pin
**
**  Parameters:
**  pin:   pin to be read
**  value: the value to be written to the pin
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/
void digitalWrite(uint8_t pin, uint8_t value)
{
  //The only pin that will be use is ACI_REQN
  //MOSI, MISO and Clk have to be  in 1 for the SPI to work properly
  switch (pin){
    case ACI_MOSI:
      P1_bit.no2 = value;
      break;
    case ACI_MISO:
      P1_bit.no1 = value;
      break;
    case ACI_SCLK:
      P1_bit.no0 = value;
      break;
    case ACI_REQN:
      P14_bit.no1 = value;
      break;
    case ACI_RDYN:
      //P14_bit.no7 = value;
      break;
    case ACI_RESET:
      P14_bit.no0 = value;
      break;
    default:
      break;
  }
}