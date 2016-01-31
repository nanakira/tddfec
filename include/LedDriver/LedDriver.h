#ifndef D_LedDriver_H
#define D_LedDriver_H
#include <stdint.h>

void LedDriver_Create(uint16_t * ledsAddress);
void LedDriver_TurnOn(int ledNumber);
void LedDriver_TurnOff(int ledNumber);

#endif
