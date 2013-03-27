/*******************************************************************************
* File Name: uart_INT.c
* Version 2.10
*
* Description:
*  This file provides all Interrupt Service functionality of the UART component
*
* Note:
*  Any unusual or non-standard behavior should be noted here. Other-
*  wise, this section should remain blank.
*
*******************************************************************************
* Copyright 2008-2011, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "uart.h"
#include "CyLib.h"


/***************************************
* Custom Declratations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if( (uart_RX_ENABLED || uart_HD_ENABLED) && \
     (uart_RXBUFFERSIZE > uart_FIFO_LENGTH))

    extern volatile uint8 uart_rxBuffer[];
    extern volatile uint8 uart_rxBufferRead;
    extern volatile uint8 uart_rxBufferWrite;
    extern volatile uint8 uart_rxBufferLoopDetect;
    extern volatile uint8 uart_rxBufferOverflow;
    #if (uart_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 uart_rxAddressMode;
        extern volatile uint8 uart_rxAddressDetected;
    #endif /* End EnableHWAddress */    

    /*******************************************************************************
    * Function Name: uart_RXISR
    ********************************************************************************
    *
    * Summary:
    *  Interrupt Service Routine for RX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  uart_rxBuffer - RAM buffer pointer for save received data.
    *  uart_rxBufferWrite - cyclic index for write to rxBuffer, 
    *     increments after each byte saved to buffer.
    *  uart_rxBufferRead - cyclic index for read from rxBuffer, 
    *     checked to detect overflow condition.
    *  uart_rxBufferOverflow - software overflow flag. Set to one
    *     when uart_rxBufferWrite index overtakes 
    *     uart_rxBufferRead index.
    *  uart_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when uart_rxBufferWrite is equal to 
    *    uart_rxBufferRead
    *  uart_rxAddressMode - this variable contains the Address mode, 
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  uart_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(uart_RXISR)
    {
        uint8 readData;
        uint8 increment_pointer = 0;

        /* User code required at start of ISR */
        /* `#START uart_RXISR_START` */

        /* `#END` */

        readData = uart_RXSTATUS_REG;

        if((readData & (uart_RX_STS_BREAK | uart_RX_STS_PAR_ERROR |
                        uart_RX_STS_STOP_ERROR | uart_RX_STS_OVERRUN)) != 0)
        {
            /* ERROR handling. */
            /* `#START uart_RXISR_ERROR` */

            /* `#END` */
        }

        while(readData & uart_RX_STS_FIFO_NOTEMPTY)
        {
            
            #if (uart_RXHW_ADDRESS_ENABLED)
                if(uart_rxAddressMode == uart__B_UART__AM_SW_DETECT_TO_BUFFER) 
                {
                    if((readData & uart_RX_STS_MRKSPC) != 0u )
                    {  
                        if ((readData & uart_RX_STS_ADDR_MATCH) != 0)
                        {
                            uart_rxAddressDetected = 1u;
                        }
                        else
                        {
                            uart_rxAddressDetected = 0u;
                        }
                    }

                    readData = uart_RXDATA_REG;
                    if(uart_rxAddressDetected != 0u)
                    {   /* store only addressed data */
                        uart_rxBuffer[uart_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* without software addressing */
                {
                    uart_rxBuffer[uart_rxBufferWrite] = uart_RXDATA_REG;
                    increment_pointer = 1u;
                }
            #else  /* without addressing */
                uart_rxBuffer[uart_rxBufferWrite] = uart_RXDATA_REG;
                increment_pointer = 1u;
            #endif /* End SW_DETECT_TO_BUFFER */
            
            /* do not increment buffer pointer when skip not adderessed data */
            if( increment_pointer != 0u )
            {
                if(uart_rxBufferLoopDetect)
                {   /* Set Software Buffer status Overflow */
                    uart_rxBufferOverflow = 1u;
                }
                /* Set next pointer. */
                uart_rxBufferWrite++;

                /* Check pointer for a loop condition */
                if(uart_rxBufferWrite >= uart_RXBUFFERSIZE)
                {
                    uart_rxBufferWrite = 0u;
                }
                /* Detect pre-overload condition and set flag */
                if(uart_rxBufferWrite == uart_rxBufferRead)
                {
                    uart_rxBufferLoopDetect = 1u;
                    /* When Hardware Flow Control selected */
                    #if(uart_FLOW_CONTROL != 0u)
                    /* Disable RX interrupt mask, it will be enabled when user read data from the buffer using APIs */
                        uart_RXSTATUS_MASK_REG  &= ~uart_RX_STS_FIFO_NOTEMPTY;
                        CyIntClearPending(uart_RX_VECT_NUM); 
                        break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                    #endif /* End uart_FLOW_CONTROL != 0 */    
                }
            }

            /* Check again if there is data. */
            readData = uart_RXSTATUS_REG;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START uart_RXISR_END` */

        /* `#END` */

        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(CY_PSOC3_ES2 && (uart_RXInternalInterrupt__ES2_PATCH))
            uart_ISR_PATCH();
        #endif /* End CY_PSOC3_ES2*/
    }

#endif /* End uart_RX_ENABLED && (uart_RXBUFFERSIZE > uart_FIFO_LENGTH) */


#if(uart_TX_ENABLED && (uart_TXBUFFERSIZE > uart_FIFO_LENGTH))

    extern volatile uint8 uart_txBuffer[];
    extern volatile uint8 uart_txBufferRead;
    extern uint8 uart_txBufferWrite;


    /*******************************************************************************
    * Function Name: uart_TXISR
    ********************************************************************************
    *
    * Summary:
    * Interrupt Service Routine for the TX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  uart_txBuffer - RAM buffer pointer for transmit data from.
    *  uart_txBufferRead - cyclic index for read and transmit data 
    *     from txBuffer, increments after each transmited byte.
    *  uart_rxBufferWrite - cyclic index for write to txBuffer, 
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(uart_TXISR)
    {

        /* User code required at start of ISR */
        /* `#START uart_TXISR_START` */

        /* `#END` */

        while((uart_txBufferRead != uart_txBufferWrite) && \
             !(uart_TXSTATUS_REG & uart_TX_STS_FIFO_FULL))
        {
            /* Check pointer. */
            if(uart_txBufferRead >= uart_TXBUFFERSIZE)
            {
                uart_txBufferRead = 0u;
            }

            uart_TXDATA_REG = uart_txBuffer[uart_txBufferRead];

            /* Set next pointer. */
            uart_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START uart_TXISR_END` */

        /* `#END` */
        
        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(CY_PSOC3_ES2 && (uart_TXInternalInterrupt__ES2_PATCH))
            uart_ISR_PATCH();
        #endif /* End CY_PSOC3_ES2*/
    }

#endif /* End uart_TX_ENABLED && (uart_TXBUFFERSIZE > uart_FIFO_LENGTH) */


/* [] END OF FILE */
