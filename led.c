#include "led.h"

/* tutaj ustalamy prog, kiedy wlacza sie dioda led */
static const float threshold = 10.0;

void led_init(void)
{
		/* podlaczenie zegara */
		SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
		/* ustawienie pinu jako GPIO */
		PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);
		/* ustawienie pinu jako wyjscie */
		PTB->PDDR |= (1 << RED_LED);
}

uint8_t handle_led(uint8_t state_of_led, float distance)
{
		/* pobieramy obecny stan diody LED */
		uint8_t current_state = state_of_led;
	
		/* zarzadzamy tym, czy wlaczyc czy wylaczyc diode, albo nic nie robic */
		if (distance < threshold && current_state == 0){
				PTB->PDOR &= ~(1 << RED_LED);
				current_state = 1;
		}
		else if (distance > threshold && current_state == 1){
				PTB->PDOR |= (1 << RED_LED);
				current_state = 0;
		}
		/* zwracamy obecny stan diody */
		return current_state;
}
