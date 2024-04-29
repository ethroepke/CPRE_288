///**
// * lab5_template.c
// *
// * Template file for CprE 288 Lab 5
// *
// * @author Zhao Zhang, Chad Nelson, Zachary Glanz
// * @date 08/14/2016
// *
// * @author Phillip Jones, updated 6/4/2019
// * @author Diane Rover, updated 2/25/2021, 2/17/2022
// */
//
//#include "button.h"
//#include "timer.h"
//#include "lcd.h"
//#include "uart.h"
//#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART1)
//                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1
//
//#include "cyBot_Scan.h"  // Scan using CyBot servo and sensors
//
////#warning "Possible unimplemented functions"
//#define REPLACEME 0
//
//
//void sendString(char s[]){
//    int i;
//    for (i = 0; i < strlen(s); i++){
//        uart_sendChar(s[i]);
//    }
//    uart_sendChar('\r');
//    uart_sendChar('\n');
//    return;
//}
//
//void newLine(){
//    uart_sendChar('\r');
//    uart_sendChar('\n');
//}
//
//int main2(void) {
//	button_init();
//	timer_init(); // Must be called before lcd_init(), which uses timer functions
//	lcd_init();
////	uart_init();
//  // initialize the cyBot UART1 before trying to use it
//
//  // (Uncomment ME for UART init part of lab)
//	 cyBot_uart_init_clean();  // Clean UART1 initialization, before running your UART1 GPIO init code
//
//	 //Complete this code for configuring the GPIO PORTB part of UART1 initialization (your UART1 GPIO init code)
//     SYSCTL_RCGCGPIO_R |= 0b000010;
//	 while ((SYSCTL_PRGPIO_R & 0x10) == 0) {};
//	 GPIO_PORTB_DEN_R |= 0x03;
//	 GPIO_PORTB_AFSEL_R |= 0x03;
//	 GPIO_PORTB_PCTL_R &= ~0x000000FF;     // Force 0's in the desired locations
//     GPIO_PORTB_PCTL_R |= 0x00000011;     // Force 1's in the desired locations
//		 // Or see the notes for a coding alternative to assign a value to the PCTL field
//
//     // (Uncomment ME for UART init part of lab)
//     cyBot_uart_init_last_half();  // Complete the UART device configuration
//
//     // Initialize the scan
//     cyBOT_init_Scan(0b0111);
//	 // Remember servo calibration function and variables from Lab 3
//
//	 // YOUR CODE HERE
////     uart_sendChar('t');
////
////     newLine();
////     sendString("help");
////
////     char new = uart_receive();
////     uart_sendChar(new);
////     newLine();
//
//
//
////     while(1)
////     {
////         int i = 0;
////         char loop = 0;
////         char str[21] = "";
////         lcd_clear(); //clear lcd
////
////         while(i<21){
////             loop = uart_receive(); //receive character
////             if (loop == '\n'){ //check for /n character, if it is, break out of while loop
////                 break;
////             }
////             if (loop != 0) {//otherwise, place the character, append to the string, and show character on putty
////                 lcd_putc(loop);
////                 str[i] = loop;
////                 uart_sendChar(loop);
////                 i++;
////             }
////             loop = 0;
////         }
////         lcd_clear();
////         lcd_printf("%s\n%d", str, i);
////         sendString(str);
////     }
//     int i;
//     char loop;
//     while(1){
//         char str[21]= "";
//         i = 0;
//         while(i<21){
//             loop = uart_receive();
//             if (loop == '\r'){
////                 sendString("%d", i)
//                 break;
//             }
//             str[i] = loop;
//             lcd_clear();
//             lcd_puts(str);
//             lcd_gotoLine(2);
//             i++;
//             lcd_printf("%s\n%d", str, i);
//             uart_sendChar(loop);
//         }
//         lcd_clear();
//         newLine();
//         uart_sendStr("finished string: ");
//         uart_sendStr(str);
//         newLine();
//     }
//}
