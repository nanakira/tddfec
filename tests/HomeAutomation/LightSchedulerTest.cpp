//- ------------------------------------------------------------------
//-    Copyright (c) James W. Grenning -- All Rights Reserved         
//-    For use by owners of Test-Driven Development for Embedded C,   
//-    and attendees of Renaissance Software Consulting, Co. training 
//-    classes.                                                       
//-                                                                   
//-    Available at http://pragprog.com/titles/jgade/                 
//-        ISBN 1-934356-62-X, ISBN13 978-1-934356-62-3               
//-                                                                   
//-    Authorized users may use this source code in your own          
//-    projects, however the source code may not be used to           
//-    create training material, courses, books, articles, and        
//-    the like. We make no guarantees that this source code is       
//-    fit for any purpose.                                           
//-                                                                   
//-    www.renaissancesoftware.net james@renaissancesoftware.net      
//- ------------------------------------------------------------------


#include "CppUTest/TestHarness.h"

extern "C"
{
#include "LightScheduler.h"
#include "LightControllerSpy.h"
#include "FakeTimeService.h"
}


TEST_GROUP(LightScheduler)
{
    void setup()
    {
        LightController_Create();
        LightScheduler_Create();
    }

    void teardown()
    {
        LightController_Destroy();
        LightScheduler_Destroy();
    }

    void setTimeTo(int day, int minuteOfDay)
    {
        FakeTimeService_SetDay(day);
        FakeTimeService_SetMinute(minuteOfDay);
    }

    void checkLightState(int id, int level)
    {
        if (id == LIGHT_ID_UNKNOWN)
        {
            LONGS_EQUAL(id, LightControllerSpy_GetLastId());
            LONGS_EQUAL(level, LightControllerSpy_GetLastState());
        }
        else
            LONGS_EQUAL(level, LightControllerSpy_GetLightState(id));
    }
};

TEST(LightScheduler, NoChangeToLightsDuringInitialization)
{
    checkLightState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

TEST(LightScheduler, NoScheduleNothingHappens)
{
    setTimeTo(MONDAY, 100);
    LightScheduler_WakeUp();
    checkLightState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

TEST(LightScheduler, ScheduleOnEverydayNotTimeYet)
{
    LightScheduler_ScheduleTurnOn(3, EVERYDAY, 1200);
    setTimeTo(MONDAY, 1199);
    LightScheduler_WakeUp();
    checkLightState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

TEST(LightScheduler, ScheduleOnEverydayItsTime)
{
    LightScheduler_ScheduleTurnOn(3, EVERYDAY, 1200);
    setTimeTo(MONDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(3, LIGHT_ON);
}

TEST(LightScheduler, ScheduleOffEverydayItsTime)
{
    LightScheduler_ScheduleTurnOff(3, EVERYDAY, 1200);
    setTimeTo(MONDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(3, LIGHT_OFF);
}

TEST(LightScheduler, ScheduleWeekEndItsMonday)
{
    LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);
    setTimeTo(MONDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

TEST(LightScheduler, ScheduleWeekEndItsFriday)
{
    LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);
    setTimeTo(FRIDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

TEST(LightScheduler, ScheduleWeekEndItsSaturday)
{
    LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);
    setTimeTo(SATURDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(3, LIGHT_ON);
}

TEST(LightScheduler, ScheduleWeekEndItsSunday)
{
    LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);
    setTimeTo(SUNDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(3, LIGHT_ON);
}

TEST(LightScheduler, ScheduleTuesdayButItsMonday)
{
    LightScheduler_ScheduleTurnOn(3, TUESDAY, 1200);
    setTimeTo(MONDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

TEST(LightScheduler, ScheduleTuesdayAndItsTuesday)
{
    LightScheduler_ScheduleTurnOn(3, TUESDAY, 1200);
    setTimeTo(TUESDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(3, LIGHT_ON);
}

TEST(LightScheduler, WeekdayScheduleNotOperatedOnSunday)
{
    LightScheduler_ScheduleTurnOn(3, WEEKDAY, 1200);
    setTimeTo(SUNDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

TEST(LightScheduler, WeekdayScheduleIsOperatedOnMonday)
{
    LightScheduler_ScheduleTurnOn(3, WEEKDAY, 1200);
    setTimeTo(MONDAY, 1200);
    LightScheduler_WakeUp();
    checkLightState(3, LIGHT_ON);
}

TEST(LightScheduler, ScheduleTwoEventsAtTheSameTime)
{
    LightScheduler_ScheduleTurnOn(3, SUNDAY, 1200);
    LightScheduler_ScheduleTurnOn(12, SUNDAY, 1200);

    setTimeTo(SUNDAY, 1200);

    LightScheduler_WakeUp();

    checkLightState(3, LIGHT_ON);
    checkLightState(12, LIGHT_ON);
}

TEST(LightScheduler, RejectsTooManyEvents)
{
    int i;
    for (i = 0; i < 128; i++)
        LONGS_EQUAL(LS_OK, LightScheduler_ScheduleTurnOn(6, MONDAY, 600+i));

    LONGS_EQUAL(LS_TOO_MANY_EVENTS, LightScheduler_ScheduleTurnOn(6, MONDAY, 600+i));
}

TEST_GROUP(LightSchedulerInitAndCleanup)
{
};

TEST(LightSchedulerInitAndCleanup, CreateStartsOneMinuteAlarm)
{
    LightScheduler_Create();
    POINTERS_EQUAL((void *)LightScheduler_WakeUp,
                   (void *)FakeTimeService_GetAlarmCallback());

    LONGS_EQUAL(60, FakeTimeService_GetAlarmPeriod());
    LightScheduler_Destroy();
}

TEST(LightSchedulerInitAndCleanup, DestroyCancelsOneMinuteAlarm)
{
    LightScheduler_Create();
    LightScheduler_Destroy();
    POINTERS_EQUAL(NULL, (void *)FakeTimeService_GetAlarmCallback());
}
