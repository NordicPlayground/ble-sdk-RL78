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
* Filename: CG_serial.c
* Abstract: This file implements device driver for Serial module.
* APIlib: Applilet3 for RL78/G13 E1.00b [14 Jan 2011]
* Device: R5F100LE
* Compiler: IAR Systems iccrl78
* Creation date:  29/10/2014
*******************************************************************************
*/

/*
*******************************************************************************
** Include files
*******************************************************************************
*/
#include "CG_macrodriver.h"
#include "hal_platform.h"
#include "CG_serial.h"

#define BUFFER_SIZE 128

/*Define for using or not SPI interrupt*/
#define SPI_NO_INTERRUPT
//#define SPI_INTERRUPT

/*
*******************************************************************************
**  Global define
*******************************************************************************
*/
volatile uint8_t * addByte;    /*Address of the next byte to be transmitted*/
volatile uint8_t TXcnt;        /*Number of pending bytes to be transmitted*/
volatile uint8_t RXcnt;        /*Number of received bytes*/

uint8_t bufferUARTtx[BUFFER_SIZE];     /*Can transmit up to 128 bytes*/
uint8_t bufferUARTrx[BUFFER_SIZE];     /*Can receive up to 128 bytes*/

SerialAdapter Serial;          /*Structure used to convert the Arduino Serial commands into plain C*/

const uint8_t tableAscii[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

#if defined (SPI_INTERRUPT)
  volatile bool flagSPIdone;
#endif

volatile uint8_t RxSPIByte;

void UART1_Init(void);
void UART1_Start(void);
void UART1_Stop(void);
__interrupt void UARTtxInterrupt(void);
__interrupt void UARTrxInterrupt(void);
void CSI00_Init(void);
void CSI00_Start(void);
void CSI00_Stop(void);
#if defined (SPI_INTERRUPT)
  __interrupt void MD_INTCSI00(void);
#endif

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function initializes the SAU0 module.
**
**  Parameters:
**  None
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/
void SAU0_Init(void)
{
  SAU0EN = 1U;  /* supply SAU0 clock */
  NOP();
  NOP();
  NOP();
  NOP();
  SPS0 = _0003_SAU_CK00_FCLK_3 | _0010_SAU_CK01_FCLK_1;
  /*CK0 set to 4Mhz due fCLK 32Mhz and divisor 2^3 (8).
    This clock is for SPI. Final SPI freq 2Mhz
    CK1 set to 16Mhz due fCLK 32Mhz and divisor 2^1 (2).
    This clock is for UART. Final UART freq 115 200bps
  */
  UART1_Init();
  UART1_Start();
  CSI00_Init();
  CSI00_Start();
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function initializes the UART1 module.
**
**  Parameters:
**  None
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/
void UART1_Init(void)
{
  ST0 |= _0008_SAU_CH3_STOP_TRG_ON | _0004_SAU_CH2_STOP_TRG_ON; /* disable UART1 receive and transmit */
  STMK1 = 1U; /* disable INTST1 interrupt */
  STIF1 = 0U; /* clear INTST1 interrupt flag */
  SRMK1 = 1U; /* disable INTSR1 interrupt */
  SRIF1 = 0U; /* clear INTSR1 interrupt flag */
  /* Set INTST1 low priority */
  STPR11 = 1U;
  STPR01 = 1U;
  /* Set INTSR1 low priority */
  SRPR11 = 1U;
  SRPR01 = 1U;
  /*Start configuration of UART TX */
  SMR02 = _0020_SAU_SMRMN_INITIALVALUE | _8000_SAU_CLOCK_SELECT_CK01 | _0000_SAU_TRIGGER_SOFTWARE | _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
  SCR02 = _8000_SAU_TRANSMISSION | _0000_SAU_INTSRE_MASK | _0000_SAU_PARITY_NONE | _0080_SAU_LSB | _0010_SAU_STOP_1 | _0007_SAU_LENGTH_8;
  SDR02 = _8800_UART1_TRANSMIT_DIVISOR; /*Set baud rate frequency for 115 200bps */
  /*16 000 000Mhz/115 200bps = 138.8 = 138 counts ==> transforming into register number 138-2=136. Shifting LSB to zero = 0x88
    Start configuration of UART RX
  */
  NFEN0 |= _04_SAU_RXD1_FILTER_ON;
  SMR03 = _0020_SAU_SMRMN_INITIALVALUE | _8000_SAU_CLOCK_SELECT_CK01 | _0100_SAU_TRIGGER_RXD | _0000_SAU_EDGE_FALL | _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
  SCR03 = _4000_SAU_RECEPTION | _0000_SAU_INTSRE_MASK | _0000_SAU_PARITY_NONE | _0080_SAU_LSB | _0010_SAU_STOP_1 | _0007_SAU_LENGTH_8;
  SDR03 = _8800_UART1_RECEIVE_DIVISOR;  /*Set baud rate frequency for 115 200bps*/
  SO0 |= _0004_SAU_CH2_DATA_OUTPUT_1;
  SOL0 |= _0000_SAU_CHANNEL2_NORMAL;  /* output level normal */
  SOE0 |= _0004_SAU_CH2_OUTPUT_ENABLE;  /* enable UART1 output */
  /* Set RxD1 pin */
  PMC0 &= 0xF7U;
  PM0 |= 0x08U;
  /* Set TxD1 pin */
  P0 |= 0x04U;
  PMC0 &= 0xFBU;
  PM0 &= 0xFBU;
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function starts the UART1 module operation.
**
**  Parameters:
**  None
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/
void UART1_Start(void)
{
  STIF1 = 0U; /* clear INTST1 interrupt flag */
  STMK1 = 0U; /* enable INTST1 interrupt */
  SRIF1 = 0U; /* clear INTSR1 interrupt flag */
  SRMK1 = 0U; /* enable INTSR1 interrupt */
  SO0 |= _0004_SAU_CH2_DATA_OUTPUT_1; /* output level normal */
  SOE0 |= _0004_SAU_CH2_OUTPUT_ENABLE;  /* enable UART1 output */
  SS0 |= _0008_SAU_CH3_START_TRG_ON | _0004_SAU_CH2_START_TRG_ON; /* enable UART1 receive and transmit */
  
  /*Reset counters for UART reception*/
  RXcnt = 0;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function stops the UART1 module operation.
**
**  Parameters:
**  None
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/
void UART1_Stop(void)
{
  ST0 |= _0008_SAU_CH3_STOP_TRG_ON | _0004_SAU_CH2_STOP_TRG_ON; /* disable UART1 receive and transmit */
  SOE0 &= ~_0004_SAU_CH2_OUTPUT_ENABLE; /* disable UART1 output */
  STMK1 = 1U; /* disable INTST1 interrupt */
  STIF1 = 0U; /* clear INTST1 interrupt flag */
  SRMK1 = 1U; /* disable INTSR1 interrupt */
  SRIF1 = 0U; /* clear INTSR1 interrupt flag */
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function sends through UART a string.
**
**  Parameters:
**  string: pointer to the string that will be send
**  
**
**  Returns:
**  True:  Successful UART transmission
**  False: Unsuccessful UART transmission
**
**-----------------------------------------------------------------------------
*/
bool printString(uint8_t * string, uint8_t lineOption)
{
  uint8_t maxSize = 0;
  uint8_t sizeString = 0;
  
  if(TXcnt == 0)   /*There are no more characters to send. The buffer is empty and ready to receive more data */
  {
    sizeString = strlen(string);
    if(lineOption == NO_LINE){
      if (sizeString > BUFFER_SIZE)
      {
        memcpy(bufferUARTtx, string, BUFFER_SIZE);
        TXcnt = BUFFER_SIZE;
      }
      else
      {
        memcpy(bufferUARTtx, string, sizeString);
        TXcnt = sizeString;
      }
      
    }else if(lineOption == WITH_LINE){
      if (sizeString > (BUFFER_SIZE - 1))    /*127 bytes as still needs to include the '\n' character */
      {
        memcpy(bufferUARTtx, string, (BUFFER_SIZE - 1));
        /*Copy the '\n' character to the 128 location. 129 location for '\0' */
        bufferUARTtx[BUFFER_SIZE - 1] = '\n';
        TXcnt = BUFFER_SIZE;
      }
      else
      {
        memcpy(bufferUARTtx , string, sizeString);
        /*Counter is the pending chars on buffer and the new string */
        TXcnt = sizeString;
        /*Add the '\n' char */
        bufferUARTtx[TXcnt++] = '\n';
      }
    }
    
    /*Write to the UART register */
    STMK1 = 1U; /*Disable UART Tx Interrupt */
    addByte = bufferUARTtx;
    TXD1 = *addByte;
    addByte++;
    TXcnt--;
    STMK1 = 0U; /*Enable UART Tx Interrupt */
    
    return true;
  }
  else
  {
    STMK1 = 1U; /*Disable UART Tx Interrupt */
    
    /*First move the pending characters to the first position of the UARTbuffer*/
    memmove(bufferUARTtx, (void *)addByte, TXcnt);
    /*Obtain the maximum free spaces still in the buffer. */
    maxSize = BUFFER_SIZE - TXcnt;
    sizeString = strlen(string);
    
    if(lineOption == NO_LINE){
      if (sizeString > maxSize)
      {
        memcpy((bufferUARTtx + TXcnt), string, maxSize);
        TXcnt = BUFFER_SIZE;
      }
      else
      {
        memcpy((bufferUARTtx + TXcnt), string, sizeString);
        TXcnt = TXcnt + sizeString;
      }
      
    }else if(lineOption == WITH_LINE){
      /*The string is bigger than the available space */
      /*maxSize - 1 because we still need to add the '\n' character */
      if (sizeString > (maxSize - 1))
      {
        /*Copy the string */
        memcpy((bufferUARTtx + TXcnt), string, (maxSize - 1));
        /*Copy the '\n' character to the 128 location. 129 location for '\0' */
        bufferUARTtx[BUFFER_SIZE - 1] = '\n';
        TXcnt = BUFFER_SIZE;
      }
      else
      {
        memcpy((bufferUARTtx + TXcnt), string, sizeString);
        /*Counter is the pending chars on buffer and the new string */
        TXcnt = TXcnt + sizeString;
        /*Add the '\n' char */
        bufferUARTtx[TXcnt++] = '\n';
      }
    }
    
    /*Set the character pointer to location 0 of buffer */
    addByte = bufferUARTtx;
    
    STMK1 = 0U; /*Enable UART Tx Interrupt */
    
    /*If the buffer is almost full, wait until it gets emptied a little */
    if(TXcnt > 64){
      delay(5);
    }

    return false;
  }
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function sends a number in hexadecimal format.
**
**  Parameters:
**  hexNumber: number to be converted into ASCII in hexadecimal format
**  
**
**  Returns:
**  True:  Successful UART transmission
**  False: Unsuccessful UART transmission
**
**-----------------------------------------------------------------------------
*/
bool printHex(uint8_t hexNumber, uint8_t lineOption)
{
  uint8_t residueNumber = 0;

  if(TXcnt == 0)   /*There are no more characters to send. The buffer is empty and ready to receive more data. */
  {
    bufferUARTtx[TXcnt++] = '0';
    bufferUARTtx[TXcnt++] = 'x';
    
    if(hexNumber > 0x0F)
    {
      residueNumber = hexNumber % 16;
      hexNumber = hexNumber / 16;
      bufferUARTtx[TXcnt++] = tableAscii[hexNumber];
      hexNumber = residueNumber;
    }
    
    bufferUARTtx[TXcnt++] = tableAscii[hexNumber];
    
    if(lineOption == WITH_LINE){
      bufferUARTtx[TXcnt++] = '\n';
    }

    /*Write to the UART register */
    STMK1 = 1U; /*Disable UART Tx Interrupt */
    addByte = bufferUARTtx;
    TXD1 = *addByte;
    addByte++;
    TXcnt--;
    STMK1 = 0U; /*Enable UART Tx Interrupt */
    
    return true;
    
  }
  else
  {
    STMK1 = 1U; /*Disable INTST1 interrupt */
    /*First move the pending characters to the first position of the UARTbuffer */
    memmove(bufferUARTtx, (void *)addByte, TXcnt);   
    
    bufferUARTtx[TXcnt++] = '0';
    bufferUARTtx[TXcnt++] = 'x';
    
    if(hexNumber > 0x0F)
    {
      residueNumber = hexNumber % 16;
      hexNumber = hexNumber / 16;
      bufferUARTtx[TXcnt++] = tableAscii[hexNumber];
      hexNumber = residueNumber;
    }
    
    bufferUARTtx[TXcnt++] = tableAscii[hexNumber];
    
     if(lineOption == WITH_LINE){
      bufferUARTtx[TXcnt++] = '\n';
    }
    
    /*Set the character pointer and counter to the new values for continuing UART transmission */
    addByte = bufferUARTtx;
    
    STMK1 = 0U; /* enable INTST1 interrupt */
    
    /*If the buffer is almost full, wait until it gets emptied a little */
    if(TXcnt > 100){
      delay(5);
    }

    return false;
  }
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function sends a number in decimal format.
**
**  Parameters:
**  decNumber: number to be converted into ASCII in decimal format
**  
**
**  Returns:
**  True:  Successful UART transmission
**  False: Unsuccessful UART transmission
**
**-----------------------------------------------------------------------------
*/
bool printDec(uint8_t decNumber, uint8_t lineOption)
{
  uint8_t residueNumber = 0;
  
  if(TXcnt == 0)
  {
    if(decNumber > 99)
    {
      residueNumber = decNumber % 100;
      decNumber = decNumber / 100;
      bufferUARTtx[TXcnt++] = tableAscii[decNumber];
      decNumber = residueNumber;
    }
    
    if(decNumber > 9)
    {
      residueNumber = decNumber % 10;
      decNumber = decNumber / 10;
      bufferUARTtx[TXcnt++] = tableAscii[decNumber];
      decNumber = residueNumber;
    }
    
    bufferUARTtx[TXcnt++] = tableAscii[decNumber];
    
    if(lineOption == WITH_LINE){
      bufferUARTtx[TXcnt++] = '\n';
    }
    
    /*Write to the UART register */
    STMK1 = 1U; /*Disable UART Tx Interrupt */
    addByte = bufferUARTtx;
    TXD1 = *addByte;
    addByte++;
    TXcnt--;
    STMK1 = 0U; /*Enable UART Tx Interrupt */
    
    return true;
    
  }
  else
  {
    STMK1 = 1U; /*Disable INTST1 interrupt */
    /*First move the pending characters to the first position of the UARTbuffer */
    memmove(bufferUARTtx, (void *)addByte, TXcnt);
    
    if(decNumber > 99)
    {
      residueNumber = decNumber % 100;
      decNumber = decNumber / 100;
      bufferUARTtx[TXcnt++] = tableAscii[decNumber];
      decNumber = residueNumber;
    }
    
    if(decNumber > 9)
    {
      residueNumber = decNumber % 10;
      decNumber = decNumber / 10;
      bufferUARTtx[TXcnt++] = tableAscii[decNumber];
      decNumber = residueNumber;
    }
    
    bufferUARTtx[TXcnt++] = tableAscii[decNumber];
    
    if(lineOption == WITH_LINE){
      bufferUARTtx[TXcnt++] = '\n';
    }
    
    /*Set the character pointer and counter to the new values for continuing UART transmission */
    addByte = bufferUARTtx;
    STMK1 = 0U; /* enable INTST1 interrupt */

    /*If the buffer is almost full, wait until it gets emptied a little */
    if(TXcnt > 100){
      delay(5);
    }

    return false;
  }
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function selects according to which type of element to print
**  the function to send the data via UART. No new line character added
**
**  Parameters:
**  string: data to be sent via UART
**  typeString: selects whether it is string, decimal or hex
**
**  Returns:
**  True:  Successful UART transmission
**  False: Unsuccessful UART transmission
**
**-----------------------------------------------------------------------------
*/
bool selectPrint(uint8_t * string, uint8_t typeString)
{
  bool result = false;
  
  switch(typeString){
    case TYPE_STRING:
      result = printString(string, NO_LINE);
      break;
    case TYPE_HEX:
      result = printHex((uint8_t)string, NO_LINE);
      break;
    case TYPE_DEC:
      result = printDec((uint8_t)string, NO_LINE);
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
**  This function selects according to which type of element to print
**  the function to send the data via UART. A new line character is added
**
**  Parameters:
**  string: data to be sent via UART
**  typeString: selects whether it is string, decimal or hex
**
**  Returns:
**  True:  Successful UART transmission
**  False: Unsuccessful UART transmission
**
**-----------------------------------------------------------------------------
*/
bool selectPrintln(uint8_t * string, uint8_t typeString)
{
  bool result = false;

  switch(typeString){
    case TYPE_STRING:
      result = printString(string, WITH_LINE);
      break;
    case TYPE_HEX:
      result = printHex((uint8_t)string, WITH_LINE);
      break;
    case TYPE_DEC:
      result = printDec((uint8_t)string, WITH_LINE);
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
**  This function sends a raw byte through UART. This function can
**  send a 0 through UART.
**
**  Parameters:
**  value: data to be sent via UART
**
**  Returns:
**  True:  Successful UART transmission
**  False: Unsuccessful UART transmission
**
**-----------------------------------------------------------------------------
*/
bool writeByte(uint8_t value){
  bool status = false;

  if(TXcnt == 0){
    bufferUARTtx[TXcnt++] = value;
    status =  true;
    
    /*Write to the UART register */
    STMK1 = 1U; /*Disable UART Tx Interrupt */
    addByte = bufferUARTtx;
    TXD1 = *addByte;
    addByte++;
    TXcnt--;
    STMK1 = 0U; /*Enable UART Tx Interrupt */
    
  }else{
    STMK1 = 1U; /*Disable UART Tx Interrupt */
    bufferUARTtx[TXcnt++] = value;
    STMK1 = 0U; /*Enable UART Tx Interrupt */
  }
  
  return status; 
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function initializes the structure used to convert Arduino code into 
**  plain C code
**
**  Parameters:
**  * initSerial: Pointer to the Serial structure 
**  
**
**  Returns:
**  None
**  
**-----------------------------------------------------------------------------
*/
void initializeSerialMonitor(SerialAdapter * initSerial)
{
    initSerial->print = selectPrint;
    initSerial->println = selectPrintln;
    initSerial->write = writeByte;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function is INTST1 interrupt service routine.
**
**  Parameters:
**  None
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/
#pragma vector = INTST1_vect
__interrupt void UARTtxInterrupt(void)
{
  if (TXcnt > 0U)
  {
    TXD1 = *addByte;
    addByte++;
    TXcnt--;
  }
  else
  {
    TXcnt = 0;         /*Be sure to set the counter to 0 */
  }
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function checks if there is a new line character between the first 20 characters of the UART Rx buffer
**
**  Parameters:
**  targetChar: pointer that gets the index of the new line character
**
**  Returns:
**  true
**  false
**
**-----------------------------------------------------------------------------
*/
bool existsNewLine(uint8_t * targetChar)
{
  uint8_t index = 0;
  bool status = false;
  
  for(index = 0; index < 20; index++)
  {
    if(bufferUARTrx[index] == '\n'){      
      *targetChar = index;
      status = true;
      break;
    }
  }
  
  return status;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function receives copies data from the UART Rx buffer to another specific buffer.
**
**  Parameters:
**  uartBuffer: receive buffer pointer where to copy data
**  uartLenght: number of characters copied
**
**  Returns:
**  true
**  false
**
**-----------------------------------------------------------------------------
*/
bool getUART(uint8_t * uartBuffer, uint8_t * uartLenght)
{
  uint8_t newLinePosition = 0;
  uint8_t i = 0;
  bool status = true;
  
  SRMK1 = 1U; /* disable INTSR1 interrupt */
  
  if(RXcnt > 0){
    if(existsNewLine(&newLinePosition))        /*Copy data if there is a '\n' character between the first 20 characters */
    {
      memcpy(uartBuffer, bufferUARTrx, newLinePosition);
      *uartLenght = newLinePosition;
      
      if(RXcnt == (newLinePosition + 1))   /*Check if the '\n' was the last character */
      {
        RXcnt = 0;
        /*Clean the buffer */
        for(i = 0; i < BUFFER_SIZE; i++){
          bufferUARTrx[i] = '\0';
        }
      }
      else                  /*Otherwise move forward the values of the Rx UART buffer, except the '\n' */
      {
        newLinePosition++;
        memmove(bufferUARTrx, (bufferUARTrx + newLinePosition), RXcnt - newLinePosition);
        RXcnt = RXcnt - newLinePosition;
        /*Clear the rest of the buffer */
        for(i = RXcnt; i < BUFFER_SIZE; i++){
          bufferUARTrx[i] = '\0';
        }
      }
      
    }
    else if(RXcnt > 20)                 /*Copy data if there are more than 20 characters */
    {
      memcpy(uartBuffer, bufferUARTrx, 20);
      *uartLenght = 20;
      memmove(bufferUARTrx, (bufferUARTrx + 20), RXcnt - 20);
      RXcnt = RXcnt - 20;
      Serial.println("Serial input truncated");
    }
    else
    {
      status = false;
    }
  }
  else
  {
      status = false;
  }
  
  SRMK1 = 0U; /* enable INTSR1 interrupt */
  
  return status;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function is INTSR1 interrupt service routine.
**
**  Parameters:
**  None
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/
#pragma vector = INTSR1_vect
__interrupt void UARTrxInterrupt(void)
{
  uint8_t rx_data;

  rx_data = RXD1;
  
  /*While you have space in the buffer just copy the data received via UART */
  if (RXcnt < BUFFER_SIZE)
  {
    bufferUARTrx[RXcnt++] = rx_data;
  }
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function initializes the CSI00 module.
**
**  Parameters:
**  None
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/
void CSI00_Init(void)
{
  ST0 |= _0001_SAU_CH0_STOP_TRG_ON; /* disable CSI00 */
  CSIMK00 = 1U; /* disable INTCSI00 interrupt */
  CSIIF00 = 0U; /* clear INTCSI00 interrupt flag */
  /* Set INTCSI00 low priority */
  CSIPR100 = 1U;
  CSIPR000 = 1U;
  SMR00 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_CLOCK_MODE_CKS | _0000_SAU_TRIGGER_SOFTWARE | _0000_SAU_MODE_CSI | _0000_SAU_TRANSFER_END;
  SCR00 = _C000_SAU_RECEPTION_TRANSMISSION | _3000_SAU_TIMING_4 | _0080_SAU_LSB | _0007_SAU_LENGTH_8;
  SDR00 = _0000_CSI00_DIVISOR;
  /*CK0 is 4Mhz and divisor by default is 2, final freq is 2Mhz*/
  SO0 |= _0100_SAU_CH0_CLOCK_OUTPUT_1;  /* CSI00 clock initial level */
  SO0 &= ~_0001_SAU_CH0_DATA_OUTPUT_1;  /* CSI00 SO initial level */
  SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;  /* enable CSI00 output */
  /* Set SI00 pin */
  PM1 |= 0x02U;
  /* Set SO00 pin */
  P1 |= 0x04U;
  PM1 &= 0xFBU;
  /* Set SCK00 pin */
  P1 |= 0x01U;
  PM1 &= 0xFEU;
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function starts the CSI00 module operation.
**
**  Parameters:
**  None
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/
void CSI00_Start(void)
{
  CSIIF00 = 0U; /* clear INTCSI00 interrupt flag */
  #if defined (SPI_INTERRUPT)
  CSIMK00 = 0U; /* enable INTCSI00 */
  #endif
  SO0 |= _0100_SAU_CH0_CLOCK_OUTPUT_1;  /* CSI00 clock initial level */
  SO0 &= ~_0001_SAU_CH0_DATA_OUTPUT_1;  /* CSI00 SO initial level */
  SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;  /* enable CSI00 output */
  SS0 |= _0001_SAU_CH0_START_TRG_ON;  /* enable CSI00 */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function stops the CSI00 module operation.
**
**  Parameters:
**  None
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/
void CSI00_Stop(void)
{
  ST0 |= _0001_SAU_CH0_STOP_TRG_ON; /* disable CSI00 */
  SOE0 &= ~_0001_SAU_CH0_OUTPUT_ENABLE; /* disable CSI00 output */
  CSIMK00 = 1U; /* disable INTCSI00 interrupt */
  CSIIF00 = 0U; /* clear INTCSI00 interrupt flag */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function sends and receives CSI00 data.
**
**  Parameters:
**  txbuf: transfer buffer pointer
**  txnum: buffer size
**  rxbuf: receive buffer pointer
**
**  Returns:
**  MD_OK
**  MD_ARGERROR
**
**-----------------------------------------------------------------------------
*/
uint8_t CSI00_SendReceiveData(UCHAR txByte)
{
  
  #if defined (SPI_NO_INTERRUPT)
    /*SPI With no interruption */
    CSIIF00 = 0;     /*Clear flag of SPI transmission/reception end (interruption flag) */
    SIO00 = txByte;  /*Started by writing data to SDR[7:0] */

    while(CSIIF00 == 0);
    RxSPIByte = SIO00;
    
  #elif defined (SPI_INTERRUPT)
    /*SPI with interruption */
    
    flagSPIdone = false;
    uint8_t dummy = 0;
    
    CSIMK00 = 1U;    /* disable INTCSI00 interrupt */
    SIO00 = txByte;  /* started by writing data to SDR[7:0] */
    CSIMK00 = 0U;    /* enable INTCSI00 interrupt */

    while(flagSPIdone);
    
    /*Small delay for the interrupt to work properly */
    /*Refer to RL78 web forum for more details */
    for(dummy = 0 ; dummy < 10; dummy++);
  #endif
  
  return RxSPIByte;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**  This function is INTCSI00 interrupt service routine.
**
**  Parameters:
**  None
**
**  Returns:
**  None
**
**-----------------------------------------------------------------------------
*/

#if defined (SPI_INTERRUPT)

#pragma vector = INTCSI00_vect
__interrupt void MD_INTCSI00(void)
{
   
  RxSPIByte = SIO00;
  flagSPIdone = true;
  
}

#endif