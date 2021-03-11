#ifndef LED_H
#define LED_H

#include "MKL05Z4.h"

/* czerwona dioda LED jest podlaczona do PTB8 */
#define RED_LED 8

/* funkcja inicjalizujaca diode LED */
void led_init(void);

/* funkcja zarzadzajaca dzialaniem diody LED */
uint8_t handle_led(uint8_t state_of_led, float distance);

#endif
