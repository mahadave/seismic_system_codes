/*******************************************************************************
* File Name: main.c
*
* Version: 2.10
*
* Description: 
*   This is a source code for example project of ADC differential mode.
*
* Hardware Connections:
*  Connect analog input from Variable resistor to port P0[0] of DVK1 board.
*
********************************************************************************/

#include <device.h>


/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*  Main function performs following functions:
*  1: Initializes the LCD
*  2: Initializes the VDAC.
*  3: Starts ADC 
*  4: Starts ADC converstion.
*  5: Gets the converted result and displays it in LCD.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/

void main()
{
      
    
   

    uart_isr_Start();      /* Initializing the ISR */
    uart_Start();     /* Enabling the UART */
    lcd_Start(); /* Enabling the LCD */
    adc_Start();
    CyGlobalIntEnable; 
    
    /* Start the ADC conversion */
    adc_StartConvert();
 
    
    for(;;)
    {
        
    }
}


/* [] END OF FILE */
