// Sound.cpp

// Sound assets based off the original Space Invaders 
// These arrays create classic sounds!
// Jonathan Valvano
// 1/16/2021
#include <stdint.h>
#include "Sound.h"
#include "Timer0.h"
#include "DAC.h"
#include "../inc/tm4c123gh6pm.h"
// these are sampled at 8 bits 11kHz
// If your DAC is less than 8 bits you will need to scale the data
	
// Global Vars
int length;
char *SoundPt;
	
void SoundTask() {
	if (length) {
		DAC_Out(*SoundPt);
		SoundPt++;
		length--;
	}
	else {
		NVIC_DIS0_R = 1<<19;
	}
}	

// initialize a 11kHz timer, flags, pointers, counters,and the DAC
void Sound_Init(void){
	length = 0;
	DAC_Init();
	//Timer0_Init(&SoundTask, 80000000/11025);			// 11 kHz
};

//******* Sound_Start ************
// This function does not output to the DAC. 
// Rather, it sets a pointer and counter, and then enables the timer interrupt.
// It starts the sound, and the timer ISR does the output
// feel free to change the parameters
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement
void Sound_Start(char *pt, uint32_t count){
	length = count;
	SoundPt = pt;
	Timer0_Init(&SoundTask, 80000000/11025);			// 11 kHz
	NVIC_EN0_R = 1<<19;
};


const char *sp;

void SoundTask1() {
	if (length) {
		DAC_Out(*sp);
		sp++;
		length--;
	}
	else {
		NVIC_DIS0_R = 1<<19;
	}
}

void SoundST(const char *pt, uint32_t count) {
	length = count;
	sp = pt;
	Timer0_Init(&SoundTask1, 80000000/11025);			// 11 kHz
	NVIC_EN0_R = 1<<19;
}
// 11 kHz timer ISR
// 
// timer ISR sends one point to DAC each interrupt
// Because TExas sends data to TExasDisplay at 10kHz, you will not be able to toggle a profile pin
// However, you can activate TExaS in analog scope mode and connect PD2 to the DAC output to visualize the sound wave
