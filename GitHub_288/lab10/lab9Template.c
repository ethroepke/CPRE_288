/**
 * @file lab9_template.c
 * @author
 * Template file for CprE 288 Lab 9
 */

#include "Timer.h"
#include "lcd.h"
#include "ping_template.h"
#include "uart-interrupt.h"
#include "servo.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "movement.h"
#include <math.h>
#include "stdint.h"

// Uncomment or add any include directives that are needed

//#warning "Possible unimplemented functions"
//#define REPLACEME 0

int main(void) {
    timer_init();
    lcd_init();
    servo_init();


    //bot 8 upstairs
    right_calibration_value = 243250;
    left_calibration_value = 124600;

    while(1){
        lcd_printf("90");
        servo_move(90);
        timer_waitMillis(2000);
        lcd_printf("0");
        servo_move(0);
        timer_waitMillis(2000);
        lcd_printf("180");
        servo_move(180);
        timer_waitMillis(2000);

    }

    return 0;
}
