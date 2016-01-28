#include "tddfecBuildTime.h"

tddfecBuildTime::tddfecBuildTime()
: dateTime(__DATE__ " " __TIME__)
{
}

tddfecBuildTime::~tddfecBuildTime()
{
}

const char* tddfecBuildTime::GetDateTime()
{
    return dateTime;
}

