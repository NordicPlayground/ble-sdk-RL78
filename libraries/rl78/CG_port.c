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
  /*For the LED2*/
  P7 = _80_Pn7_OUTPUT_1;      
  PM7 = _01_PMn0_NOT_USE | _02_PMn1_NOT_USE | _04_PMn2_NOT_USE | _08_PMn3_NOT_USE | _10_PMn4_NOT_USE | _20_PMn5_NOT_USE | _40_PMn6_NOT_USE | _00_PMn7_MODE_OUTPUT;
  /*For SPI lines of nRF8001*/
  P14 = _01_Pn0_OUTPUT_1 | _02_Pn1_OUTPUT_1;    
  PM14 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _40_PMn6_NOT_USE | _80_PMn7_MODE_INPUT | _3C_PM14_DEFAULT;
  PU14 = _80_PUn7_PULLUP_ON;
  PMC14 = _00_PMCn7_DI_ON | _7F_PMC14_DEFAULT;
  
  /*The pin organization for the ACI is:
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
  
  /*The only pin that will be use is ACI_RDYN*/
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
  /*The only pin that will be used is ACI_REQN.
    MOSI, MISO and Clk have to be set to 1 so SPI work properly.
    If any if these three pins are set to 0, that pin will not
    output the value coming from the SPI module.
  */
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