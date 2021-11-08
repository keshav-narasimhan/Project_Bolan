// SlidePot.cpp
// Runs on TM4C123
// Provide functions that initialize ADC0
// copy code from Lab 8 or Lab 9
// Last Modified: 1/16/2021 
// Student names: solutions

#include <stdint.h>
#include "SlidePot.h"
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
// measures from PD2, analog channel 5
void ADC_Init(void){ 
	SYSCTL_RCGCGPIO_R |= 0x08;								// enable PORT D clock
	while ((SYSCTL_PRGPIO_R & 0x08) == 0) {};	// busy-wait
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	GPIO_PORTD_DIR_R &= ~0x04;								// PD2 = input
	GPIO_PORTD_AFSEL_R |= 0x04;								// enable alternate function
	GPIO_PORTD_DEN_R &= ~0x04;								// disable digital for PD2
	GPIO_PORTD_AMSEL_R |= 0x04;								// enable analog function

	SYSCTL_RCGCADC_R |= 0x01;									// activate ADC0
	int delay = SYSCTL_RCGCADC_R;									// stabilize
	delay = SYSCTL_RCGCADC_R;									// stabilize
	delay = SYSCTL_RCGCADC_R;									// stabilize
	delay = SYSCTL_RCGCADC_R;									// stabilize
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	
	ADC0_PC_R = 0x01;													// speed = 125k samples/sec
	ADC0_SSPRI_R = 0x0123;										// SEQ3 = highest priority
	ADC0_ACTSS_R &= ~0x0008;									// disable SEQ3
	ADC0_EMUX_R &= ~0xF000;										// trigger = SEQ3
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R & 0xFFFFFFF0) + 5;	// Ain5
	ADC0_SSCTL3_R = 0x0006;										// set CTL register
	ADC0_IM_R &= ~0x0008;											// disable interrutps
	ADC0_ACTSS_R |= 0x0008;										// enable SEQ3
	// ADC0_SAC_R = sac;													// enable SEQ averaging
}

//------------ADCIn------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD2, analog channel 5
uint32_t ADC_In(void){  
	uint32_t data;
	ADC0_PSSI_R = 0x0008;											// start ADC0
	while ((ADC0_RIS_R & 0x0008) == 0) {};		// busy-wait
	data = ADC0_SSFIFO3_R & 0xFFF;						// fetch 12-bit data
	ADC0_ISC_R = 0x0008;											// clear flag
	return data;															// return 12-bit data
}

// constructor, invoked on creation of class
// m and b are linear calibration coeffients 
SlidePot::SlidePot(uint32_t m, uint32_t b){
	// private variables
	data = 0;
	flag = 0;
	distance = 0;
	slope = m;
	offset = b;
}

void SlidePot::Save(uint32_t n){
	data = n;												// save data in private variable
	distance = Convert(data);				// convert ADC val into distance
	flag = 1;												// set semaphore flag to 1
}
uint32_t SlidePot::Convert(uint32_t n){
	return ((167 * n)/4096) + 24;
}

void SlidePot::Sync(void){
	while (flag == 0) {};										// wait for a nonzero flag val
	flag = 0;
}

uint32_t SlidePot::ADCsample(void){ // return ADC sample value (0 to 4095)
	return data;
}

uint32_t SlidePot::Distance(void){  // return distance value (0 to 2000), 0.001cm
	return distance;
}


