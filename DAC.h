// put prototypes for public functions, explain what it does
// put your names here, date
#ifndef __DAC_H__ // do not include more than once
#define __DAC_H__
#include <stdint.h>

// initialize DAC
void DAC_Init();

// output to the DAC
void DAC_Out(uint8_t data);

#endif
