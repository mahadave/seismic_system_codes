/*******************************************************************************
* File Name: adc_INT.c  
* Version 2.20
*
* Description:
*  This file contains the code that operates during the ADC_DelSig interrupt 
*  service routine.  
*
* Note:
*
*******************************************************************************
* Copyright 2008-2011, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "adc.h"  


/*******************************************************************************
* Custom Declarations and Variables
* - add user inlcude files, prototypes and variables between the following 
*   #START and #END tags
*******************************************************************************/
/* `#START ADC_SYS_VAR`  */

/* `#END`  */


/*****************************************************************************
* Function Name: adc_ISR1
******************************************************************************
*
* Summary:
*  Handle Interrupt Service Routine.  
*
* Parameters:  
*  void
*
* Return: 
*  void 
*
* Reentrant: 
*  No
*
*****************************************************************************/
CY_ISR( adc_ISR1)
{
    /***************************************************************************
    *  Custom Code
    *  - add user ISR code between the following #START and #END tags
    **************************************************************************/
    /* `#START MAIN_ADC_ISR1`  */


#include "lcd.h"
#include "uart.h"

            uint16 output; // do sth for 32 bit
            int8  upper;
            int8  lower;

            output = adc_GetResult16();
            
                  
            upper = (uint8) ((output>>8)&(0x00FF)); 
            lower = (uint8) ((output)&(0x00FF)); 
            
            uart_WriteTxData(upper); // upper byte
            uart_WriteTxData(lower); // lower byte
            
            lcd_Position(0, 0);            
            lcd_PrintString("        ");      
            lcd_Position(0, 0);       
            lcd_PrintNumber(output);      
                  
                  
    /* `#END`  */
    
    /* Stop the conversion if conversion mode is single sample and resolution
       is above 16 bits. 
    */
    #if(adc_CFG1_RESOLUTION > 16 && \
        adc_CFG1_CONV_MODE == adc_MODE_SINGLE_SAMPLE) 
        adc_StopConvert(); 
    #endif /* Single sample conversion mode with resolution above 16 bits */         
    
    /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)
        #if((CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2) && (adc_IRQ__ES2_PATCH ))      
            adc_ISR_PATCH();
        #endif /* Silicon revision check */
    #endif /* Chip member check */   
    
}


/*****************************************************************************
* Function Name: adc_ISR2
******************************************************************************
*
* Summary:
*  Handle Interrupt Service Routine.  
*
* Parameters:  
*  void
*
* Return: 
*  void 
*
* Reentrant: 
*  No
*
*****************************************************************************/
CY_ISR( adc_ISR2)
{
    /***************************************************************************
    *  Custom Code
    *  - add user ISR code between the following #START and #END tags
    **************************************************************************/
    /* `#START MAIN_ADC_ISR2`  */

    /* `#END`  */
    
    /* Stop the conversion conversion mode is single sample and resolution
       is above 16 bits.
    */
    #if(adc_CFG2_RESOLUTION > 16 && \
        adc_CFG2_CONVMODE == adc_MODE_SINGLE_SAMPLE) 
        adc_StopConvert(); 
    #endif /* Single sample conversion mode with resolution above 16 bits */
    
    /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)
        #if((CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2) && (adc_IRQ__ES2_PATCH ))      
            adc_ISR_PATCH();
        #endif /* Silicon revision check */
    #endif /* Chip member check */   
    
}


/*****************************************************************************
* Function Name: adc_ISR3
******************************************************************************
*
* Summary:
*  Handle Interrupt Service Routine.  
*
* Parameters:  
*  void
*
* Return: 
*  void 
*
* Reentrant: 
*  No
*
*****************************************************************************/
CY_ISR( adc_ISR3)
{
    /***************************************************************************
    *  Custom Code
    *  - add user ISR code between the following #START and #END tags
    **************************************************************************/
    /* `#START MAIN_ADC_ISR3`  */

    /* `#END`  */

    /* Stop the conversion if conversion mode is set to single sample and 
       resolution is above 16 bits. 
    */
    #if(adc_CFG3_RESOLUTION > 16 && \
        adc_CFG3_CONVMODE == adc_MODE_SINGLE_SAMPLE) 
        adc_StopConvert(); 
    #endif /* Single sample conversion mode with resolution above 16 bits */  
    
    /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)
        #if((CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2) && (adc_IRQ__ES2_PATCH ))      
            adc_ISR_PATCH();
        #endif /* Silicon revision check */
    #endif /* Chip member check */       
}


/*****************************************************************************
* Function Name: adc_ISR4
******************************************************************************
*
* Summary:
*  Handle Interrupt Service Routine.  
*
* Parameters:  
*  void
*
* Return: 
*  void 
*
* Reentrant: 
*  No
*
*****************************************************************************/
CY_ISR( adc_ISR4)
{
    /***************************************************************************
    *  Custom Code
    *  - add user ISR code between the following #START and #END tags
    **************************************************************************/
    /* `#START MAIN_ADC_ISR4`  */

    /* `#END`  */
    
    /* Stop the conversion if conversion mode is set to single sample and 
       resolution is above 16 bits. 
    */
    #if(adc_CFG4_RESOLUTION > 16 && \
        adc_CFG4_CONVMODE == adc_MODE_SINGLE_SAMPLE) 
        adc_StopConvert(); 
    #endif /* Single sample conversion mode with resolution above 16 bits */
    
    /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)
        #if((CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2) && (adc_IRQ__ES2_PATCH ))      
            adc_ISR_PATCH();
        #endif /* Silicon revision check */
    #endif /* Chip member check */            
}


/* [] END OF FILE */
