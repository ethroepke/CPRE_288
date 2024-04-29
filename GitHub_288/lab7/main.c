#include "cyBot_uart.h"
#include "lcd.h"
#include "cyBot_Scan.h"
#include "Timer.h"
#include "movement.h"
#include "open_interface.h"
#include <math.h>
#include "uart-interrupt.h"
#include "stdint.h"

//#include "stdbool.h"
/**
 * main.c
 */

//global variables for use in functions for cleaning up
uint16_t array[180];
int angle[10];
float distance[10];
float width[10];
int object = 0;
int midPoint = 0;
char setting;

//calibration values
int diffTol = 20;
int tolerance = 70; //40 for on table, 70 for on floor
int irTolerance = 550; //could go higher
int irDiffTol = 100;

void doScan();
void findSmallest(oi_t *sensor);

//bandaid solution for old use case.
void sendString(char s[]){
    uart_sendStr(s);
    return;
}

int main(void){

    lcd_init();
    timer_init();
    uart_interrupt_init();
    cyBOT_init_Scan(0b0111); //init PING

    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()

    //bot 9 upstairs
    //right_calibration_value = 269500;
    //left_calibration_value = 1256500;

    //bot 8 upstairs
    right_calibration_value = 243250;
    left_calibration_value = 124600;

    char recieve;
    while (1) {
        lcd_puts("waiting for PuTTY");
        recieve = uart_receive();
        if (recieve == 'f') { //run lab 7 fast mode
            setting = 'f';
            doScan();
            findSmallest(sensor_data);
            break;
        }
        if (recieve == 'a') { //run lab 7
            setting = 'a';
            doScan();
            findSmallest(sensor_data);
            break;
        }
        if (recieve == 't') { //run lab 7
//            setting = 'a';
//            doScan();
//            findSmallest(sensor_data);
            turn(sensor_data, 90);
            sendString("test");
            break;
        }
        if (recieve == 'h') {
            cyBOT_SERVO_cal();
        }

    }


    oi_setWheels(0,0);
    oi_free(sensor_data); // do this once at end of main()
    return 0;
}


//lab 7 run scan
void doScan(){
    int i,j,x,dist1,dist2,dist3;
    char output[100];
    char angleChar[5];
//    cyBOT_Scan_t *scan = malloc(sizeof(cyBOT_Scan_t));
    cyBOT_Scan_t scan;
    sendString("/n/r For Loop 1, fill array with raw IR values \n\r");
    for (i=0;i<=180;i++){
        if (command_flag == 1){
            return;
        }
        cyBOT_Scan(i, &scan);
        dist1 = scan.IR_raw_val;
        if (dist1 > 700 && setting == 'a') { // (dist1 < tolerance && setting == 'a') double checks when sensor is saying further away than farTol or closer than tolerance
            //lcd_puts("."); //outputs when object is further than 150
            cyBOT_Scan(i, &scan);
            dist2 = scan.IR_raw_val;
            cyBOT_Scan(i, &scan);
            dist3 = scan.IR_raw_val;

            x = (dist1+dist2+dist3)/3;
        }else{
            x = dist1;
        }
        array[i] = x;
        sprintf(output, " %d\r\n", array[i]);
        sprintf(angle, "%d ", i);
        sendString(output);
        sendString(angle);
    }

    sendString("\r\nFor Loop 2 \n\r");
    for (j=0;j<179;j++){
        if (command_flag == 1){
            return;
        }
        int count = 0;
        int finalAngle = 0;
        int initAngle = j;
        //while point is far from the point after to it, or when point is far from the point before it, and is between tolerances
        while(((array[j] - array[j+1] > -90) || (array[j]-array[j+1] < 90)) && (array[j] > 700 && array[j] < 1600)) {
//        while(array[j] > 550 && array[j] < 1600) {
            count++;
            sprintf(output, "angle = %d \r\n", j);
            sendString(output);
            j++;
        }
        if (count >=5){
            finalAngle = j - 1;
            midPoint = (initAngle + finalAngle) /2;
            angle[object] = midPoint;
            sprintf(output, "object declared: initAngle = %d, finalAngle = %d, midPoint=%d\r\nScanning...", initAngle, finalAngle, midPoint);
            sendString(output);
            cyBOT_Scan(midPoint, &scan);
            if (scan.sound_dist < 35.0){
                timer_waitMillis(500);
                cyBOT_Scan(midPoint, &scan);
            }
            sprintf(output, "midPoint = %d ", midPoint);
            sendString(output);
            sprintf(output, "midPoint distance = %d ", scan.sound_dist);
            sendString(output);
            cyBOT_Scan(midPoint, &scan);
            distance[object] = scan.sound_dist;
            width[object] = 2 * distance[object] * tan(((midPoint - initAngle) * M_PI) /180);
            sprintf(output, "Width: %f\r\n ", width[object]);
            sendString(output);
            object++;
            timer_waitMillis(500);
        }
    }
}

//lab 7 find smallest
void findSmallest(oi_t *sensor){
    sendString("findSmallest called!\r\n");
    if (object < 2){
        sendString("only one object");
        return;
    }
    int k = 0;
    int min = k;
    for (k = 1; k < object; k++) {
        if (width[k] < width[min]){
            min = k;
            char output[100];
            sprintf(output, "New Smallest: Width: %d\r\n", min);
            sendString(output);
        }
    }
    int ang = angle[min] - 90;
    float dist = distance[min];

    sendString("calling turn\r\n");
    turn(sensor, ang);
    sendString("Calling move_forward\r\n");
    move_forward(sensor, (dist * 10) - 10.0);
    oi_free(sensor);
}



//-----------------------------------------------------------------------
//    if (message == 'b'){
//        //cyBot_sendByte('o');
//        //lcd_putc('g');
//
//        int angle;
//        double distance, dist1, dist2, dist3, dist4;
//        int measuring, numObjects = 0;
//        int objectStart[20];
//        int objectEnd[20];
//        char output[30];
//
//        cyBOT_Scan_t *scan = malloc(sizeof(cyBOT_Scan_t));
//        for (angle = 0; angle <= 180; angle++){
//            cyBOT_Scan(angle, scan);
//            dist1 = scan->sound_dist;
//            dist2 = scan->sound_dist;
//            dist3 = scan->sound_dist;
//            dist4 = scan->sound_dist;
//            distance = (dist1+dist2+dist3+dist4)/4.0;
//            sprintf(output, "%d \t\t %lf", angle , distance);
//            sendString(output);
//
//            if (measuring){
//                if (distance > tolerance){
//                    measuring = 0;
//                    objectEnd[numObjects] = angle;
//                    numObjects++;
//                    sendString("Object end");
//                }
//                else if (measuring == 0){
//                    lcd_puts("object detected");
//                    //measuring += 1;
//                }
//                else{
//                    lcd_puts(".");
//                }
//            }
//            else {
//                if (distance < tolerance){
//                    measuring = 1;
//                    objectStart[numObjects] = angle;
//                    sendString("object Start");
//                }
//            }
//
//
//        }
////        lcd_puts(numObjects); this doest work need to sprintf
//
//        double angularWidth[20];
//        int j;
//        double objectCenter[20];
//        double objectDist[20];
//        int smallestAngle = 999;
//        double smallestDist = 9999;
//
//        //gets width for each object
//        for (j = 0; j < numObjects; j++){
//            angularWidth[j] = objectEnd[j] - objectStart[j]; //gets the width of each object
//            if (angularWidth[j] > 5 && angularWidth[j] < 180){ //tests for bad widths
//                objectCenter[j] = objectStart[j] + (angularWidth[j] / 2);
//                cyBOT_Scan(objectCenter[j], scan);
//                timer_waitMillis(500);
//                dist1 = scan->sound_dist;
//                dist2 = scan->sound_dist;
//                dist3 = scan->sound_dist;
//                dist4 = scan->sound_dist;
//                objectDist[j] = (dist1+dist2+dist3+dist4)/4.0;
//                double actualWidth = 6.28 * objectDist[j] * (angularWidth[j]/360.0);
//                sprintf(output, "%lf\t%lf\t%lf\t%lf\n\r", objectCenter[j],angularWidth[j], objectDist[j], actualWidth);
//                sendString(output);
//            }
//            else if (angularWidth[j] > 180){
//                sendString("angularWidth error");
//            }
//            else if (angularWidth[j] < 0){
//                sendString("angularWidth error");
//            }
//            else if (angularWidth[j] < 5){
//                sendString("too small, likely error");
//            }
//            else {
//                sendString("not Smaller");
//            }
//
//            //find smallestAngle
//            if (smallestAngle > angularWidth[j]){
//                smallestAngle = angularWidth[j];
//                smallestDist = objectDist[j];
//                //sendString("new smallestAngle %d", smallestAngle);
//            }
//        }
//
//        smallestAngle = smallestAngle - 90; //find turn angle
//        //float smallestObjectDist = objectDist[j];
//        //char output[100];
//        sprintf(output, "turning %d degrees", smallestAngle);
//        lcd_puts(output);
//        turn(sensor_data, smallestAngle - (smallestAngle / 12));
//        lcd_puts("moving forward");
//        double tempDist = (smallestDist * 10.0) - 100.0;
//        move_forward(sensor_data, tempDist); //drive within 10cm of smallest width object
//
//
//
////        for (j = 0; j < numObjects; j++){
////            angularWidth[j] = objectEnd[j] - objectStart[j]; //gets the width of each object
////            if (angularWidth[j] < smallestWidth && angularWidth[j] > 5 && angularWidth[j] < 180){
////                smallestWidth = angularWidth[j];
////                pointAngle = objectStart[j] + (angularWidth[j] / 2);
////                cyBOT_Scan(pointAngle, scan);
////                sendString("newSmallest");
////            }
////            else if (angularWidth[j] > 180){
////                sendString("angularWidth error");
////            }
////            else if (angularWidth[j] < 0){
////                sendString("angularWidth error");
////            }
////            else if (angularWidth[j] < 5){
////                sendString("too small, likely error");
////            }
////            else {
////                sendString("not Smaller");
////            }
////        }
////        while (1){
////            cyBOT_Scan(pointAngle, scan);
////        }
//
////    }
//
//
//    }


    //Part 3

