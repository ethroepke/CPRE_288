/*
 * servo.c
 *
 *  Created on: Apr 3, 2024
 *      Author: awfoss
 */
#include "servo.h"
#include "timer.h"
#include "lcd.h"

//another group said to do this. Seems useful
unsigned long pwmPer = 0x4E200;

void servo_init(void){
    //timer1b
    //gpio pb5
    /*
     *  In this lab, the PWM control signal for the servo is obtained using Timer 1B, which is connected to
     *  GPIO Port B pin 5 (PB5) and the “SERVO” header on the baseboard of the CyBot. Timer1B is a
     *  16-bit timer, and in PWM mode, the 8-bit prescaler register is used as an extension of the timer,
     *  creating a 24-bit timer (most-significant bits are in the prescaler register). The timer is configured
     *  as a 24-bit count-down counter in PWM mode. You will use it in periodic mode.
     */
        SYSCTL_RCGCGPIO_R |= 0x02;
        SYSCTL_RCGCTIMER_R |= 0x02;
        while ((SYSCTL_PRGPIO_R & 0x02) != 0x02) {};
        GPIO_PORTB_DIR_R &= ~0x20;
        GPIO_PORTB_AFSEL_R |= 0x20;
        GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFF7FFFFF) | 0x00700000;
        GPIO_PORTB_DEN_R |= 0x20;

        // pwm mode enabled
        TIMER1_CTL_R &= ~0x100;
        TIMER1_CFG_R = 0x4;
        TIMER1_TBMR_R = 0x0A;
        // start values
        TIMER1_TBPR_R = pwmPer >> 16;
        TIMER1_TBILR_R = pwmPer & 0xFFFF;
        TIMER1_CTL_R |= 0x100;

//       // enable clock to Timer 1
//       while ((SYSCTL_PRGPIO_R & 0x02) == 0) {};
//       GPIO_PORTB_DIR_R &= ~0x20;
//       GPIO_PORTB_AFSEL_R |= 0x20;
//       GPIO_PORTB_DEN_R |= 0x20;
//       GPIO_PORTB_PCTL_R &= ~0x0000F000;
//       GPIO_PORTB_PCTL_R |= 0x00007000;
//
//
//       // TIMER configuration
//       TIMER1_CTL_R &= ~0x0100;
//       TIMER1_CFG_R &= ~0x7;
//       TIMER1_CFG_R |= 0x4;
//       TIMER1_TBMR_R &= ~0x0F;
//       TIMER1_TBMR_R |= 0x0A;
//       TIMER1_CTL_R &= ~0x4000;
//       TIMER1_TBILR_R = 0xE200;
//       TIMER1_TBMATCHR_R &= ~0xFFFFFFFF;
//       TIMER1_TBPMR_R &= ~0xFFFF;
//
//       // Configure and enable the timer
//       //TIMER3_CTL_R |= 0x0100;
//
//       //pwm mode
//       TIMER1_CTL_R &= ~0x100;
//       TIMER1_CFG_R = 0x4;
//       TIMER1_TBMR_R = 0x0A;
//       TIMER1_TBPR_R = pwmPer >> 16;
//       TIMER1_TBILR_R = pwmPer & 0xFFFF;
//       TIMER1_CTL_R |= 0x100;

}

void servo_move(uint16_t degrees){
    int high, low;
    float pulseTime = 1 + (degrees * 0.0056);
    //send pulse high with length pulseTime to the port that we assign it
    high = (pulseTime * 1000000) / 62.5;
    low = 320000 - high;

    TIMER1_TBMATCHR_R = low;
    TIMER1_TBPMR_R = low >> 16;

}



