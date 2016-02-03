#include "LedDriver.h"

enum {ALL_LEDS_OFF = 0, ALL_LEDS_ON = 0xffff};

static uint16_t * ledsAddress;
void LedDriver_Create(uint16_t * address)
{
    ledsAddress = address;
    *ledsAddress = ALL_LEDS_OFF;
}

void LedDriver_Destroy(void)
{
}

static uint16_t convertLedNumberToBit(int ledNumber)
{
    return (uint16_t)(1 << (ledNumber - 1));
}

void LedDriver_TurnOn(int ledNumber)
{
    *ledsAddress |= convertLedNumberToBit(ledNumber);
}

void LedDriver_TurnOff(int ledNumber)
{
    *ledsAddress = ALL_LEDS_OFF;
}

void LedDriver_TurnAllOn(void)
{
    *ledsAddress = ALL_LEDS_ON;
}