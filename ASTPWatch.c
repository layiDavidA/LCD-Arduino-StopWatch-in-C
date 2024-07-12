/* Name: Olayiwola David Abraham
 * Date: 3/1/24
 * LCD Arduino StopWatch In C
 * Student id: 922303843
 * --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/******************************************************************************
 * MSP432 LCD_F Static Text Display
 *
 * Description: In the code example the LCD_F module is configured to output
 * a static text value of "123ABC". The LCD_F is configured in standard mode
 * and pin assignments are made that map the connected segment/COM pins to the
 * LCD_F registers. A basic map of the pin assignments that generate an
 * elementary alphabet are also provided and a convenience function that
 * displays a character at a position on the LCD is provided.
 *
 *             -----------------
 *            |         (L3)COM3|----------------|
 *            |         (L6)COM2|---------------||
 *            |        (L27)COM1|--------------|||
 *            |        (L26)COM0|-------------||||
 *            |                 |    -------------
 *            |        SEG LINES|---| 1 2 3 4 5 6 |
 *            |                 |    -------------
 *            |                 |       TI LCD
*******************************************************************************/
/* DriverLib Includes */
#include <LcdDriver/Crystalfontz128x128_ST7735.h>
#include <ti/grlib/grlib.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>


/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "lcd_utils.h"


/* Global Variables */
Graphics_Context g_sContext;

/* Set the RGB LED output based on the count value. */

void RGB_Output(uint8_t count){

    switch(count){

           case 0:
           GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 |GPIO_PIN1 |GPIO_PIN2);
           GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0 |GPIO_PIN1 |GPIO_PIN2);//white
           break;

           case 1:
           GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 |GPIO_PIN1 |GPIO_PIN2);//black
           break;

           case 2:
           GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 |GPIO_PIN1 |GPIO_PIN2);
           GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);//red
           break;

           case 3:
           GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 |GPIO_PIN1 |GPIO_PIN2);
           GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1 |GPIO_PIN0);//yellow
           break;

           case 4:
           GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 |GPIO_PIN1 |GPIO_PIN2);
           GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2 |GPIO_PIN0);//magenta
           break;

           case 5:
           GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 |GPIO_PIN1 |GPIO_PIN2);
           GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2 |GPIO_PIN1);//cyan
           break;

           case 6:
           GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 |GPIO_PIN1 |GPIO_PIN2);
           GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);//blue
           break;

           case 7:
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 |GPIO_PIN1 |GPIO_PIN2);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);//green
            break;
           }
}
/* Global variables for time tracking */
int count;
int hundredths=0;
int seconds=0;
char secondStr[3];
char hundredthStr[3];


// SysTick interrupt handler, increments hundredths and toggles an LED.
void SysTick_Handler(void)
{
    ++hundredths;
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);

}

//Delay for approximately 1 second using SysTick timer.
void delay_1second(void){
    SysTick->VAL =0;
    while(!(SysTick->CTRL &0x10000));
}


void Color_LCD_init()
{
   Crystalfontz128x128_Init(); //LCD Screen Size 128x128
   Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
   Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128,&g_sCrystalfontz128x128_funcs);
   Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
   Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
   GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
   Graphics_clearDisplay(&g_sContext);
}

void main(void){
    /* Stop Watchdog  */
    WDT_A_hold(WDT_A_BASE);

    if(hundredths == 100){
                  hundredths=0;
                 ++seconds;
               }

             GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
             GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
             GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2 | GPIO_PIN1 | GPIO_PIN0);
             GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2 | GPIO_PIN1 | GPIO_PIN0);

             GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4);

             SysTick_setPeriod(30000);
                  SysTick_enableModule();
                  SysTick->VAL=0;
                  int i;
                  int S1 = GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1);
                  int S2 = GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4);

    Color_LCD_init();   // Initialize LCD on BoosterPack


    while(1){
     for(i=0;i<200;i++){
         delay_1second();
         }

//Display "StopWatch" message on LCD
            Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
            Graphics_clearDisplay(&g_sContext);

            Graphics_drawString(&g_sContext,
             (int8_t *)"S1TOS2",
             AUTO_STRING_LENGTH,
             50,
             64,
             OPAQUE_TEXT);


                    S2 = GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4);

// Wait for button S1 press to track time
                    while(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) != GPIO_INPUT_PIN_LOW);
                    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
                        delay_1second();
                        Graphics_clearDisplay(&g_sContext);
                        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
                        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2 | GPIO_PIN1 | GPIO_PIN0);
                        SysTick->VAL=0;
                        SysTick_enableModule();
                        SysTick_enableInterrupt();

// Measure time until button S2 is pressed
                    while(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4) == GPIO_INPUT_PIN_HIGH){
                        if(hundredths == 100){
                         hundredths=0;
                          ++seconds;
                          ++count;
                          if(count==8){
                              count=0;
                          }
                           }
// Output RGB color based on count
                          RGB_Output(count);
                    }
// Display measured time on LCD
                        SysTick_disableInterrupt();
                        Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
                        Graphics_clearDisplay(&g_sContext);
                        delay_1second();
                        sprintf(secondStr, "%2d", seconds);
                        sprintf(hundredthStr, "%2d", hundredths);
                        Graphics_drawString(&g_sContext, (int8_t *)secondStr, AUTO_STRING_LENGTH, 30, 64, OPAQUE_TEXT);
                        Graphics_drawString(&g_sContext, (int8_t *)"S", AUTO_STRING_LENGTH, 50, 64, OPAQUE_TEXT);
                        Graphics_drawString(&g_sContext, (int8_t *)hundredthStr, AUTO_STRING_LENGTH, 70, 64, OPAQUE_TEXT);
                        Graphics_drawString(&g_sContext, (int8_t *)"H", AUTO_STRING_LENGTH, 90, 64, OPAQUE_TEXT);
    }
}


