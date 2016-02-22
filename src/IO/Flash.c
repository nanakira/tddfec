/***
 * Excerpted from "Test-Driven Development for Embedded C",
 * published by The Pragmatic Bookshelf.
 * Copyrights apply to this code. It may not be used to create training material, 
 * courses, books, articles, and the like. Contact us if you are in doubt.
 * We make no guarantees that this code is fit for any purpose. 
 * Visit http://www.pragmaticprogrammer.com/titles/jgade for more book information.
***/
/*- ------------------------------------------------------------------ -*/
/*-    Copyright (c) James W. Grenning -- All Rights Reserved          -*/
/*-    For use by owners of Test-Driven Development for Embedded C,    -*/
/*-    and attendees of Renaissance Software Consulting, Co. training  -*/
/*-    classes.                                                        -*/
/*-                                                                    -*/
/*-    Available at http://pragprog.com/titles/jgade/                  -*/
/*-        ISBN 1-934356-62-X, ISBN13 978-1-934356-62-3                -*/
/*-                                                                    -*/
/*-    Authorized users may use this source code in your own           -*/
/*-    projects, however the source code may not be used to            -*/
/*-    create training material, courses, books, articles, and         -*/
/*-    the like. We make no guarantees that this source code is        -*/
/*-    fit for any purpose.                                            -*/
/*-                                                                    -*/
/*-    www.renaissancesoftware.net james@renaissancesoftware.net       -*/
/*- ------------------------------------------------------------------ -*/


#include "Flash.h"
#include "IO.h"
#include "m28w160ect.h"
#include "MicroTime.h"

#define FLASH_WRITE_TIMEOUT_IN_MICROSECONDS 5000

void Flash_Create(void)
{
}

void Flash_Destroy(void)
{
}

int Flash_Write(ioAddress address, ioData data)
{
    ioData status = 0;

    IO_Write(CommandRegister, ProgramCommand);
    IO_Write(address, data);

    while ((status & ReadyBit) == 0)
        status = IO_Read(StatusRegister);

    if (status != ReadyBit)
    {
        IO_Write(CommandRegister, Reset);

        if (status & VppErrorBit)
            return FLASH_VPP_ERROR;
    }
    IO_Read(address);
    return FLASH_SUCCESS;
}
