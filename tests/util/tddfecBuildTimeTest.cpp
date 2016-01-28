#include "CppUTest/TestHarness.h"
#include "tddfecBuildTime.h"

TEST_GROUP(tddfecBuildTime)
{
  tddfecBuildTime* projectBuildTime;

  void setup()
  {
    projectBuildTime = new tddfecBuildTime();
  }
  void teardown()
  {
    delete projectBuildTime;
  }
};

TEST(tddfecBuildTime, Create)
{
  CHECK(0 != projectBuildTime->GetDateTime());
}

