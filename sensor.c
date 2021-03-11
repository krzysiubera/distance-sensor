#include "sensor.h"
 
void init_sensor(){
	
		/* podlaczenie zegara do pinów portu A i B wykrozystywane jako kanaly timera */
		SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK);
		
		/* ustawienie alternatynej funkcji pinu */
		PORTA->PCR[TRIG_PIN] |= PORT_PCR_MUX(2); 
		PORTB->PCR[ECHO_PIN] |= PORT_PCR_MUX(2);   
	
		/* wlaczenie zegara dla tpm */
		SIM->SCGC6 |= (SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM1_MASK);                 
		SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); 
	
		/* konfiguracja TPM0 polaczonego do pinu echo */
		/* ustawienie w trybie upcounting */
		TPM0->SC &= ~TPM_SC_CPWMS_MASK;           
		/* ustawienie preskalera - inkrementacja co 1/3 us */
		TPM0->SC |= TPM_SC_PS(4);                 
		/* wpisanie wartosci modulo */
		TPM0->MOD = 32768;						
		/* ustawienie trybu input capture, capture on rising and falling edge */
		TPM0->CONTROLS[0].CnSC |= TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK;
		/* zezwolenie na przerwania od TPM0 */
		TPM0->CONTROLS[0].CnSC |= TPM_CnSC_CHIE_MASK;  
					
		/* konfiguracja TPM1 polaczonego do pinu trig */
		/* ustawienie w trybie upcounting */
		TPM1->SC &= ~TPM_SC_CPWMS_MASK;        
		/* ustawienie preskalera - inkrementacja co 1/3 us */
		TPM1->SC |= TPM_SC_PS(4);
		/* wpisanie wartosci modulo */
		TPM1->MOD = 32768;                  
		/* ustawienie trybu edge-aligned PWM low-true pulses (set output on match, clear output on reload */
		TPM1->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
		/* ustawienie wartosci porownanai odpowiadajacej 10 us */
		TPM1->CONTROLS[0].CnV = 30;          
		
		/* wyczyszczenie oczekujacych przerwan */
		NVIC_ClearPendingIRQ(TPM0_IRQn);         
		/* wlaczenie przerwan od tpm0 */
		NVIC_EnableIRQ(TPM0_IRQn);                 	
		
		/* wlaczenie obu timerow */
		TPM1->SC |= TPM_SC_CMOD(1);              
		TPM0->SC |= TPM_SC_CMOD(1);            	
}
