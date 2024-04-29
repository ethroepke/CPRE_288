/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping_template.h"
#include "Timer.h"

volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile enum{LOW, HIGH, DONE} STATE = LOW; // State of ping echo pulse

void ping_init (void){

    // enable clock to Timer 3
    SYSCTL_RCGCTIMER_R |= 0x08;
    // wait for Timer peripherals to be ready
    while ((SYSCTL_PRTIMER_R & 0x08) == 0) {};
    // enable clock to GPIO port B
    SYSCTL_RCGCGPIO_R |= 0x02;
    // wait for GPIOB peripherals to be ready
    while ((SYSCTL_PRGPIO_R & 0x02) == 0) {};
    // make PB3 input
    GPIO_PORTB_DIR_R &= ~0x08;
    // enable alternate functions on PB3
    GPIO_PORTB_AFSEL_R |= 0x08;
    // enable digital functionality on PB3
    GPIO_PORTB_DEN_R |= 0x08;
    // enable alternate functionality on PB3 to Timer 3B
    GPIO_PORTB_PCTL_R &= ~0x0000F000;
    GPIO_PORTB_PCTL_R |= 0x00007000;


    // TIMER configuration

    //disable Timer3B
    TIMER3_CTL_R &= ~0x0100;
    //configure 16-bit timer mode
    TIMER3_CFG_R &= ~0x7;
    TIMER3_CFG_R |= 0x4;
    //capture mode
    TIMER3_TBMR_R |= 0x07;
    TIMER3_TBMR_R &= ~0x18;
    //TBEVENT is both edge triggers
    TIMER3_CTL_R |= 0x0C00;
    //load start into GPTMTAILR
    TIMER3_TBILR_R = 0xFFFF;
    //activate prescale, creating 24-bit
    TIMER3_TBPR_R = 0xFF;
    //clear Timer 3B capture mode event interrupt
    TIMER3_ICR_R |= 0x400;
    //enable Timer 3B capture mode event interrupt
    TIMER3_IMR_R |= 0x400;

        //attempt according to mtg18-slides-GPTimerIC.pdf
    //bits 7:5 is priority level (001) = priority 1
    NVIC_PRI9_R &= ~0xE0;
    NVIC_PRI9_R |= 0x20;
    //bit 4 is for Timer3B interrupt
    NVIC_EN1_R |= 0x10;

        //first attempt
    //bits 15:13 is priority level (010) = priority 2
    //NVIC_PRI25_R |= 0x4000;
    //bit 5 is for Timer3B interrupt
    //NVIC_EN3_R |= 0x20;

    //Timer 3B vector #117 and interrupt #101


    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();

    // Configure and enable the timer
    TIMER3_CTL_R |= 0x0100;

}

void ping_trigger (void){
    STATE = LOW;
    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0x100;
    TIMER3_IMR_R = ~0x400;
    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= ~0x08;

    // YOUR CODE HERE FOR PING TRIGGER/START PULSE
    // make PB3 output
    GPIO_PORTB_DIR_R |= 0x08;

    GPIO_PORTB_DATA_R &= ~0x08;
    GPIO_PORTB_DATA_R |= 0x08;
    timer_waitMicros(10);
    GPIO_PORTB_DATA_R &= ~0x08;

    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= 0x400;

    //we were missing this!!!!
    GPIO_PORTB_DIR_R &= ~0x08;

    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0x08;
    TIMER3_IMR_R |= 0x400;
    TIMER3_CTL_R |= 0x0100;
}

void TIMER3B_Handler(void){

  // YOUR CODE HERE
  // As needed, go back to review your interrupt handler code for the UART lab.
  // What are the first lines of code in the ISR? Regardless of the device, interrupt handling
  // includes checking the source of the interrupt and clearing the interrupt status bit.
  // Checking the source: test the MIS bit in the MIS register (is the ISR executing
  // because the input capture event happened and interrupts were enabled for that event?
  // Clearing the interrupt: set the ICR bit (so that same event doesn't trigger another interrupt)
  // The rest of the code in the ISR depends on actions needed when the event happens.

    if ((TIMER3_MIS_R & 0x400) == 0x400)
    {
      //clear the interrupt trigger flag (clear by writing 1 to ICR)
      TIMER3_ICR_R |= 0x400;

      if(STATE == LOW){
          START_TIME = TIMER3_TBR_R;
          STATE = HIGH;
      } else if(STATE == HIGH){
          END_TIME = TIMER3_TBR_R;
          STATE = DONE;
      }
      //pulse_flag = 1;
    }

}

float ping_getDistance (void){

    // YOUR CODE HERE
    ping_trigger();

    int timeDif = 0;
    uint8_t overflow = 0;
    while(STATE != DONE){}; //was given this by another student. not sure why its here, nothing else changes state back to done

    if (STATE==DONE){
        timeDif = START_TIME - END_TIME;
        overflow = (END_TIME > START_TIME);
        //lcd_printf("time:%d", timeDif);
    }
    else{
        lcd_printf("state != DONE");
    }
//    if (START_TIME > END_TIME){
//        timeDif = START_TIME - END_TIME;
//        lcd_printf("time:%d. num:%d", timeDif, num);
//    }
//    else {
//        timeDif = ((unsigned long) overflow << 24) + START_TIME - END_TIME;
//        //num++;
//        uart_sendChar('e');
//        lcd_printf("time:%d. num:%d", timeDif, num);
//    }
//    timeDif /= 160000000; //16000000??
//    timeDif = (timeDif/2)*34300; //math for speed of sound

    return timeDif;
}
