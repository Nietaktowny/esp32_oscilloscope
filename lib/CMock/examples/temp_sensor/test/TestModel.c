#include "MockTaskScheduler.h"
#include "MockTemperatureFilter.h"
#include "Model.h"
#include "Types.h"
#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

void testInitShouldCallSchedulerAndTemperatureFilterInit(void) {
  TaskScheduler_Init_Expect();
  TemperatureFilter_Init_Expect();
  Model_Init();
}
