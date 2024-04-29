#include "movement.h"
#include "open_interface.h"
#include "Timer.h"
#include "lcd.h"
#include "uart-interrupt.h"

//funcs

double move_forward (oi_t *sensor_data, double distance_mm){
    uart_sendStr("move_forward called\r\n");
    // the following code could be put in function move_forward()
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(50,50); //move forward at full speed

    while (sum < distance_mm) {
        oi_update(sensor_data);
        //detect if bump, then do move around func
        if((sensor_data->bumpLeft) !=0 || (sensor_data->bumpRight) != 0){
            oi_setWheels(0,0);
            move_around(sensor_data);
            sum +=350;
            oi_setWheels(50,50);
        }
        //otherwise, keep moving
        else{
            sum += sensor_data -> distance; // use -> notation since pointer
            lcd_printf("%f", sum);
        }
    }
    oi_setWheels(0,0); //stop
    return 0;
}

void turn(oi_t *sensor, double degrees) {
    uart_sendStr("turn called\r\n");
    char testOutput[75]; //sprintf things into this to output to putty or lcd
    double temp = 0;
    if (degrees >= 0) {
        oi_setWheels(50,-50);
        while(degrees >= temp){
            oi_update(sensor);
            temp += sensor -> angle;
            //output how far its turning
            sprintf(testOutput, "turning, angle = %.3lf, degrees left = %.3lf \r\n", temp, degrees);
            uart_sendStr(testOutput);
        }
    } else {
        oi_setWheels(-50 , 50);
        while(degrees <= temp){
            oi_update(sensor);
            temp += sensor -> angle;
            //output how far its turning
            sprintf(testOutput, "turning, angle = %lf.3, degrees to turn = %lf.3 \r\n", temp, degrees);
            uart_sendStr(testOutput);
        }
    }
    uart_sendStr("setting wheels to 0");
    oi_setWheels(0,0);
}

void move_around(oi_t *sensor) {
    uart_sendStr("move_around called\r\n");
    double bumpR = sensor -> bumpRight;
    move_backwards(sensor, -50);
    if (bumpR != 0) {
        //turn 90 degrees, go right 400
        turn(sensor, 90);
        move_forward(sensor, 400);
        //turn back 90 degrees, go straight 400
        turn(sensor, -90);
        move_forward(sensor, 400);
        //turn back another 90 degrees, go left 400
        turn(sensor, -90);
        move_forward(sensor, 400);
        //turn to look forward
        turn(sensor, 90);
    }
    else {
        //go other direction
        turn(sensor, -90);
        move_forward(sensor, 400);
        turn(sensor, 90);
        move_forward(sensor, 400);
        turn(sensor, 90);
        move_forward(sensor, 400);
        turn(sensor, -90);
    }
}

//i definitely wrote this before? i dont know where it went
void move_backwards(oi_t *sensor, double distance){
    uart_sendStr("move_backwards called\r\n");
    double temp = 0;

    oi_setWheels(-50,-50);
    while(temp > distance){
        oi_update(sensor);
        temp += sensor -> distance;
    }
    oi_setWheels(0,0);
    return;
}
