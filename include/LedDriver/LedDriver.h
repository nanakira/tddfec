#ifndef D_LedDriver_H
#define D_LedDriver_H
#include <stdint.h>

#define TRUE 1
#define FALSE 0
typedef int BOOL;

void LedDriver_Create(uint16_t * ledsAddress);
void LedDriver_TurnOn(int ledNumber);
void LedDriver_TurnOff(int ledNumber);
void LedDriver_TurnAllOn(void);

#endif
