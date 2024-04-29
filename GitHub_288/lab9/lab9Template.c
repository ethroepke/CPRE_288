/**
 * @file lab9_template.c
 * @author
 * Template file for CprE 288 Lab 9
 */

#include "Timer.h"
#include "lcd.h"
#include "ping_template.h"
#include "uart-interrupt.h"

// Uncomment or add any include directives that are needed

//#warning "Possible unimplemented functions"
//#define REPLACEME 0

int main(void) {
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();
    ping_init();
    uart_interrupt_init();

    int timeDif = 0;
    bool overflow = 0;
    float timeDifMS;
    float timeDifCM;
    int num = 0;

    // YOUR CODE HERE

    char text[100];
    uart_sendStr("-------------------\n");

    while(1)
    {

//      // YOUR CODE HERE
//        float pingDist = ping_getDistance();
//        sprintf(text, "%.2f", pingDist);
//        uart_sendStr(text);
//        uart_sendChar('\n');
//        uart_sendChar('\r');
//        timer_waitMillis(500);

        overflow = 0;
        timeDifMS = 0;
        timeDifCM = 0;
        timeDif = ping_getDistance();

        if(timeDif != 0){
            overflow = 1;
            num++;
        }

        timeDifMS = (timeDif * 62.5)/1000000;
        timeDifCM = ((timeDifMS / 2000) * 343) * 100;

        lcd_clear();
        lcd_printf("TimeDif: %d\nNum: %d\nMS: %f\nCM: %f", timeDif, num, timeDifMS, timeDifCM);
        timer_waitMillis(300);
    }

}
