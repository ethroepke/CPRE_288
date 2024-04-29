#include "adc.h"
#include "Timer.h"
#include "lcd.h"
#include <math.h>

    void main(){
    lab8();
}

int lab8(){
    timer_init();
    lcd_init();
    adc_init();

    uint16_t temp;
    double dist;
    char message[90];
    int i = 0;
    double total = 0;

    while(1){
        temp = adc_read();
        dist = 2.54 *(  8057.6 * pow(temp, -0.937));

        total = total + dist;
        i++;
        sprintf(message, "IR: %d\nDistance: %lf\nAverage: %lf", temp, dist, total/i);
        lcd_printf(message);
        timer_waitMillis(50);
    }
}
