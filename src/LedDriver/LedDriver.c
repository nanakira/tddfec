#include "LedDriver.h"
#include "RuntimeError.h"

enum {ALL_LEDS_OFF = 0, ALL_LEDS_ON = 0xffff};

static uint16_t * ledsAddress;
static uint16_t ledsImage;

void LedDriver_Create(uint16_t * address)
{
    ledsAddress = address;
    ledsImage = ALL_LEDS_OFF;
    *ledsAddress = ledsImage;
}

void LedDriver_Destroy(void)
{
}

enum {FIRST_LED = 1, LAST_LED = 16};

static BOOL IsLedOutOfBounds(int ledNumber)
{
    if ((ledNumber < FIRST_LED) || (ledNumber > LAST_LED))
        {
            RUNTIME_ERROR("LED Driver: out-of-bounds LED", ledNumber);
            return TRUE;
        }
    return FALSE;
}

static uint16_t convertLedNumberToBit(int ledNumber)
{
    return (uint16_t)(1 << (ledNumber - 1));
}

static void updateHardware(void)
{
    *ledsAddress = ledsImage;
}

static void setLedImageBit(int ledNumber)
{
    ledsImage |= convertLedNumberToBit(ledNumber);
}

void LedDriver_TurnOn(int ledNumber)
{
    if (IsLedOutOfBounds(ledNumber))
        return;

    setLedImageBit(ledNumber);
    updateHardware();
}

static void clearLedImageBit(int ledNumber)
{
    ledsImage &= (uint16_t)~convertLedNumberToBit(ledNumber);
}

void LedDriver_TurnOff(int ledNumber)
{
    if (IsLedOutOfBounds(ledNumber))
        return;

    clearLedImageBit(ledNumber);
    updateHardware();
}

void LedDriver_TurnAllOn(void)
{
    ledsImage = ALL_LEDS_ON;
    updateHardware();
}