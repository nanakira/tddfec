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

#include "LightScheduler.h"
#include "LightController.h"
#include "RandomMinute.h"
#include "common.h"

enum
{
    UNUSED = -1,
    TURN_ON, TURN_OFF, DIM, RANDOM_ON, RANDOM_OFF,
    MAX_EVENTS = 128
};

typedef struct
{
    int id;
    Day day;
    int minuteOfDay;
    int event;
    int randomize;
    int randomMinutes;
} ScheduledLightEvent;

static ScheduledLightEvent scheduledEvents[MAX_EVENTS];

void LightScheduler_Create(void)
{
    int i;

    for (i = 0; i < MAX_EVENTS; i++)
        scheduledEvents[i].id = UNUSED;
    TimeService_SetPeriodicAlarmInSeconds(60, LightScheduler_WakeUp);
}

void LightScheduler_Destroy(void)
{
    TimeService_CancelPeriodicAlarmInSeconds(60, LightScheduler_WakeUp);
}

static int scheduleEvent(int id, Day day, int minuteOfDay, int event)
{
    int i;

    if (id < 0 || id >= MAX_LIGHTS)
        return LS_ID_OUT_OF_BOUNDS;

    for (i = 0; i < MAX_EVENTS; i++)
    {
        if (scheduledEvents[i].id == UNUSED)
        {
            scheduledEvents[i].day = day;
            scheduledEvents[i].minuteOfDay = minuteOfDay;
            scheduledEvents[i].event = event;
            scheduledEvents[i].id = id;
            scheduledEvents[i].randomize = RANDOM_OFF;
            scheduledEvents[i].randomMinutes = 0;
            return LS_OK;
        }
    }
    return LS_TOO_MANY_EVENTS;
}

int LightScheduler_ScheduleTurnOn(int id, Day day, int minuteOfDay)
{
    return scheduleEvent(id, day, minuteOfDay, TURN_ON);
}

int LightScheduler_ScheduleTurnOff(int id, Day day, int minuteOfDay)
{
    return scheduleEvent(id, day, minuteOfDay, TURN_OFF);
}

static void operateLight(ScheduledLightEvent * lightEvent)
{
    if (lightEvent->event == TURN_ON)
        LightController_TurnOn(lightEvent->id);
    else if (lightEvent->event == TURN_OFF)
        LightController_TurnOff(lightEvent->id);
}

static int DoesLightRespondToday(Time * time, int reactionDay)
{
    int today = time->dayOfWeek;

    if (reactionDay == EVERYDAY)
        return TRUE;

    if (reactionDay == today)
        return TRUE;

    if (reactionDay == WEEKEND && (today == SATURDAY || today == SUNDAY))
        return TRUE;

    if (reactionDay == WEEKDAY && today >= MONDAY && today <= FRIDAY)
        return TRUE;

    return FALSE;
}

static void processEventDueNow(Time * time, ScheduledLightEvent * lightEvent)
{
    if (lightEvent->id == UNUSED)
        return;
    if (!DoesLightRespondToday(time, lightEvent->day))
        return;
    if ((lightEvent->minuteOfDay + lightEvent->randomMinutes) != time->minuteOfDay)
        return;

    operateLight(lightEvent);
}

void LightScheduler_WakeUp(void)
{
    int i;
    Time time;
    TimeService_GetTime(&time);

    for (i = 0; i < MAX_EVENTS; i++)
    {
        processEventDueNow(&time, &scheduledEvents[i]);
    }
}

void LightScheduler_Randomize(int id, Day day, int minuteOfDay)
{
    int i;
    for (i = 0; i < MAX_EVENTS; i++)
    {
        ScheduledLightEvent * e = &scheduledEvents[i];
        if (e->id == id && e->day == day && e->minuteOfDay == minuteOfDay)
        {
            e->randomize = RANDOM_ON;
            e->randomMinutes = RandomMinute_Get();
        }
    }
}

void LightScheduler_ScheduleRemove(int id, Day day, int minute)
{
    int i;

    for (i = 0; i < MAX_EVENTS; i++)
    {
        if (scheduledEvents[i].id == id
         && scheduledEvents[i].day == day
         && scheduledEvents[i].minuteOfDay == minute)
         {
             scheduledEvents[i].id = UNUSED;
         }
    }
}
