//*****************************************************************************
//
// blinky.c - Simple example to blink the on-board LED.
//
// Copyright (c) 2012-2016 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 2.1.3.156 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "daelttiva.h"
#include "driverlib/interrupt.h"
#include "driverlib/pwm.h"

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Blinky (blinky)</h1>
//!
//! A very simple example that blinks the on-board LED using direct register
//! access.
//
//*****************************************************************************

//*****************************************************************************
//
// Blink the on-board LED.
//
//*****************************************************************************
int


main(void)
{
    double wt = 0;
    PWM1_Init(15000);                    // Initialize PWM1 module at 15KHz
    volatile uint32_t ui32Loop;

    PWM1_Start(); // start PWM1

    PWM1_Set_Duty(50000);

       PWMIntEnable(PWM1_BASE, PWM_INT_GEN_2);
       // IntEnable(1);
       PWMGenIntTrigEnable(PWM1_BASE, PWM_INT_GEN_2, PWM_INT_CNT_ZERO);
       //IntMasterEnable();
       IntEnable(0);


    //  PWMGenIntRegister(PWM1_BASE, PWM_INT_GEN_2, void (*pfnIntHandler)(void));


   /* //
    // Enable the GPIO port that is used for the on-board LED.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Check if the peripheral access is enabled.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    //
    // Enable the GPIO pin for the LED (PF3).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

    //
    // Loop forever.
    //
    while(1)
    {
        //
        // Turn on the LED.
        //
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);

        //
        // Delay for a bit.
        //
        for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
        {
        }

        //
        // Turn off the LED.
        //
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);

        //
        // Delay for a bit.
        //
        for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
        {
        }
    }*/
}

void PWM1_GEN2_IntHandler(void)
{
// Clear the PWM0 LOAD interrupt flag.
PWMGenIntClear(PWM1_BASE, PWM_GEN_2, PWM_INT_CNT_LOAD);
// If the duty cycle is less or equal to 75% then add 0.1% to the duty
// cycle. Else, reset the duty cycle to 0.1% cycles. Note that 50 is
// 0.01% of the period (50000 cycles).
if((PWMPulseWidthGet(PWM1_BASE, PWM_OUT_5) + 50) <=
((PWMGenPeriodGet(PWM1_BASE, PWM_GEN_2) * 3) / 4))
{
PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,
PWMPulseWidthGet(PWM1_BASE, PWM_OUT_5) + 50);
}
else
{
PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 50);
}
}
/*void IntEnable(uint32_t ui32Interrupt)
{

}*/
//void IntEnable(uint32_t freq)
//{

//}
void PWM1_Init(uint32_t freq)
{
    //Calculate the number of ticks for the desired frequency
    int32_t Cycle=(SysCtlClockGet()/freq);
    //
    // Enable the GPIO Peripheral used by PWM1
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Check if the peripheral access is enabled.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    // Configure GPIO Pins for PWM6
    //
    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);

    //Habilita o PWM1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

    // Check if the peripheral access is enabled.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1));
    //
    // Configure the PWM generator for count down mode with immediate updates
    // to the parameters.
    //
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC | PWM_GEN_MODE_DBG_RUN);
    //
    // Set the period. For a 50 KHz frequency, the period = 1/50,000, or 20
    // microseconds. For a 50 MHz clock, this translates to 1000 clock ticks.
    // Use this value to set the period.
    //
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, Cycle);

    //
    // Set the pulse width of PWM0 for a 0% duty cycle.
    //
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1);
    //
    // Start the timers in generator 0.
    //
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);
    //
    // Disable the outputs.
    //
    PWMOutputInvert(PWM1_BASE, (PWM_OUT_6_BIT), false);
    PWMOutputState(PWM1_BASE, (PWM_OUT_6_BIT), false);
}

void PWM1_Start(void)
{
    //
    // Enable the outputs.
    //
    PWMOutputState(PWM1_BASE, (PWM_OUT_6_BIT), true);
}

void PWM1_Set_Duty(uint32_t duty_ratio)
{
    //lê o valor de Load do PWM1 generator 3, que determina o valor máximo que pode ser colocado em ticks
    int32_t Cycle = HWREG(PWM1_BASE + PWM_O_3_LOAD);

    //calcula o número de ticks de clock que o PWM deve ficar em alto para que se obtenha a duty_ratio configurada
    int32_t ticks = duty_ratio*Cycle / 100000;

    //verifica se o número calculado não é inválido e corrige de acordo
    if (ticks < 1)
    {
        ticks = 1;
    }
    else if(ticks > Cycle - 1)
    {
        ticks = Cycle - 1;
    }
    //configura o valor do PWM
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ticks);
}

