#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include "sensor.h"
#include "led.h"
#include <stdio.h>

/* deklaracja zmiennych - flaga informujaca o przeslaniu wyniku */
static uint8_t wynik_ok = 0;
/* deklaracja zmiennych - flaga informujaca z jakim zboczem sygnalu echo mamy do czynienia */
static uint8_t falling_edge = 0;

/* zmienne sluzace do odczytania wartosci z timera */
static volatile uint32_t start_value = 0;
static volatile uint32_t stop_time = 0;
static volatile uint32_t difference = 0;
static float distance = 0.0;

/* deklaracja stalych do przeliczenia wyniku */
static const float convert_to_distance = 58.0;
static const float convert_ticks_to_time = 3.0;

/* obsluga przerwania od TPM0 */
void TPM0_IRQHandler(void){
	
		if((TPM0->STATUS & TPM_STATUS_CH0F_MASK)){
				/* przy zboczu narastajacym zapisujemy stan startowy licznika */
				if ( falling_edge == 0){
						start_value = TPM0->CONTROLS[0].CnV;
						falling_edge = 1;
				}
				/* przy zboczu opadajacym zapisujemy stan koncowy licznika,
				ktory przeliczamy najpierw na mikrosekundy a nastepnie na odleglosc
				wynik jest przeslany do petli glownej */
				else if (falling_edge == 1){
						stop_time = TPM0->CONTROLS[0].CnV;
						difference = stop_time - start_value;
						distance = (float)difference / (float) (convert_to_distance * convert_ticks_to_time);
						if(!wynik_ok){
								wynik_ok = 1;
								start_value = 0;
								stop_time = 0;
								difference = 0;
						}
						falling_edge = 0;
			}
	}
	/* wyczyszczenie flagi przerwania po jego obsludze */
	TPM0->STATUS |= TPM_STATUS_CH0F_MASK;		
}


int main(void){
		
		/* inicjalizacja wszystkich modulow */
		LCD1602_Init();
		LCD1602_Backlight(TRUE);
		init_sensor();
		led_init();
		char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
		sprintf(display, "TEST");
		LCD1602_SetCursor(0, 0);
		LCD1602_Print(display);
		
		/* ta zmienna informuje nas czy dioda LED sie swieci */
		uint8_t state_of_led = 0;
		
		/* odebranie wyniku z podprogamuu obslugi przerwania i wyswietlenie wynik na LCD */
		while(1){
				if (wynik_ok){
						sprintf(display, "D= %.1f cm     ", distance);
						LCD1602_SetCursor(0, 0);
						LCD1602_Print(display);
						LCD1602_Print("");
						/* jezeli odleglosc jest mniejsza od ustalonego progu to zaswieci sie
						czerwona dioda led */
						state_of_led = handle_led(state_of_led, distance);
						wynik_ok = 0;
				}
		}
}
