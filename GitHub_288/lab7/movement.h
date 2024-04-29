#ifndef HEADER_FILE
#define HEADER_FILE
#include "open_interface.h"
#include "Timer.h"
#include "lcd.h"

//Function headers and macro definintions
double move_forward (oi_t *sensor_data, double disance_mm);
void turn(oi_t *sensor, double degrees);
void move_around(oi_t *sensor);
void move_backwards(oi_t *sensor, double distance);

#endif
