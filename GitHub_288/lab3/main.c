#include "cyBot_uart.h"
#include "lcd.h"
#include "cyBot_Scan.h"
#include "Timer.h"
//#include "stdbool.h"
/**
 * main.c
 */
void sendString(char s[]){
    int i;
    for (i = 0; i < strlen(s); i++){
        cyBot_sendByte(s[i]);
    }
    cyBot_sendByte('\n');
    cyBot_sendByte('\r');
    return;
}

int main(void){
//    timer_init();
    lcd_init();
    cyBot_uart_init();
    cyBOT_init_Scan(0b0111); //init PING

    //part 1


    char message = 'b'; //cyBot_getByte();
    lcd_putc(message);
    char messageR[40] = "recieved letter";
    sendString(messageR);




//    if (message == '2'){
//        //part 2
//        int i;
//        int j;
//        int thinnest;
//        int numObjects;
//        int measure = -1;
//        int maxAngle;
//        int minAngle;
//        int widths[100];
//        cyBOT_Scan_t *scan;
////        cyBOT_Scan_t *scan = malloc(sizeof(cyBOT_Scan_t));
//
//        for (i = 0; i <= 180; i++){
//            cyBOT_Scan(i, scan);
//
//            //prints
//            char tempMes[99];
//            float val = scan->sound_dist;
//            sprintf(tempMes, "%d\t%f", i, val);
//            sendString(tempMes);
//
//            if (val < 50){
//                measure = 1;
//                if (measure == 1){
//                    if (maxAngle >= minAngle){
//                        maxAngle = i;
//                    }
//                }
//            }
//            if (measure == 0){
//                widths[j] = maxAngle-minAngle;
//                j++;
//                numObjects++;
//    //            measure = 0;
//                minAngle = 999;
//                maxAngle = -999;
//                measure = -1;
//            }
//            int k;
//            for (k=0;k<numObjects;k++){
//                if (widths[k] > thinnest){
//                    thinnest = widths[k];
//                }
//            }
//
////            cyBOT_Scan_t* scan;
//
//    //        char arr[sizeof(getScan)];
//    //        memcpy(arr,&getScan,sizeof(getScan));
//    //        sendString(arr);
//    //        char charray[200];
//    //        sendString(sprintf(charray, "2.13f", getScan));
//            lcd_printf("\nNumber of objects: %d\n\rThinnest Object: %d", numObjects, thinnest);
//        }
//    }
    int tolerance = 20;

    if (message == 'a'){


        int objectDist[10];
        int i;
        int measuring;
        int j;
        int angularWidth;
        int smallestWidthAngle, distance;
        char test[25];
        int firstAngle[10];
        int lastAngle[10];
        int location;
        int objectNum;
        cyBOT_Scan_t *scan = malloc(sizeof(cyBOT_Scan_t));
        for (i = 10; i <= 180; i++) {
            cyBOT_Scan(i, scan);
            distance = scan->sound_dist;
            sprintf(test, "%d \t\t %d\n\r", i , distance);
            sendString(test);

            if(measuring){
                if(distance > objectDist[objectNum] + 10){
                    measuring = 0;
                }
            }

            if (distance < tolerance){
                if (!measuring) {
                    measuring = 1;
                    firstAngle[objectNum] = i;
                    objectDist[objectNum] = distance;
                }
            }
        }

        j = 1;
        for (i = 0; i < objectNum; i++){
            angularWidth = lastAngle[i] - firstAngle[i];
            distance = objectDist[i];
            if (angularWidth > 10){
                smallestWidthAngle = angularWidth;
                location = firstAngle[i] + (angularWidth / 2);
            }
            j++;

        }

      cyBOT_Scan(location, scan);
    }
//-----------------------------------------------------------------------
    if (message == 'b'){
        cyBot_sendByte('o');
        lcd_putc('g');

        int angle, distance;
        int measuring, numObjects = 0;
        int objectStart[20];
        int objectEnd[20];
        char output[30];

        cyBOT_Scan_t *scan = malloc(sizeof(cyBOT_Scan_t));
        for (angle = 0; angle <= 180; angle++){
            cyBOT_Scan(angle, scan);
            distance = scan->sound_dist;
            sprintf(output, "%d \t\t %d", angle , distance);
            sendString(output);

            if (measuring){
                if (distance > tolerance){
                    measuring = 0;
                    objectEnd[numObjects] = angle;
                    numObjects++;
                    sendString("Object end");
                }
                else {
                    lcd_puts("object detected");
                }
            }
            else {
                if (distance < tolerance){
                    measuring = 1;
                    objectStart[numObjects] = angle;
                    sendString("object Start");
                }
            }


        }
//        lcd_puts(numObjects); this doest work need to sprintf

        int angularWidth[10];
        int j;
        int smallestWidth = 999;
        int pointAngle;
        for (j = 0; j < numObjects; j++){
            angularWidth[j] = objectEnd[j] - objectStart[j]; //gets the width of each object
            if (angularWidth[j] < smallestWidth && angularWidth[j] > 5 && angularWidth[j] < 180){
                smallestWidth = angularWidth[j];
                pointAngle = objectStart[j] + (angularWidth[j] / 2);
                cyBOT_Scan(pointAngle, scan);
                sendString("newSmallest");
            }
            else if (angularWidth[j] > 180){
                sendString("angularWidth error");
            }
            else if (angularWidth[j] < 0){
                sendString("angularWidth error");
            }
            else if (angularWidth[j] < 5){
                sendString("too small, likely error");
            }
            else {
                sendString("not Smaller");
            }
        }
        while (1){
            cyBOT_Scan(pointAngle, scan);
        }

    }



    //Part 3

    return 0;
}
