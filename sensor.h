#ifndef SENSOR_H
#define SENSOR_H

#include "MKL05Z4.h"

/* pin PTA12 jest podlaczony do pinu trig */
#define TRIG_PIN 12 
/* pin PTB11 jest podlaczony do pinu echo */
#define ECHO_PIN 11 

/* funkcja inicjalizujaca czujnik odleglosci */
void init_sensor(void);

#endif
