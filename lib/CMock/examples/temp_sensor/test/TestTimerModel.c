#include "MockTaskScheduler.h"
#include "TimerModel.h"
#include "Types.h"
#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

void testUpdateTimeShouldDelegateToTaskScheduler(void) {
  TaskScheduler_Update_Expect(19387L);
  TimerModel_UpdateTime(19387L);
}
