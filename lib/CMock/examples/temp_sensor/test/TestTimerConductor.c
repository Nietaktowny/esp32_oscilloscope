#include "MockTimerHardware.h"
#include "MockTimerInterruptHandler.h"
#include "MockTimerModel.h"
#include "TimerConductor.h"
#include "Types.h"
#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

void testInitShouldCallHardwareInit(void) {
  TimerHardware_Init_Expect();

  TimerConductor_Init();
}

void testRunShouldGetSystemTimeAndPassOnToModelForEventScheduling(void) {
  Timer_GetSystemTime_ExpectAndReturn(1230);
  TimerModel_UpdateTime_Expect(1230);
  TimerConductor_Run();

  Timer_GetSystemTime_ExpectAndReturn(837460);
  TimerModel_UpdateTime_Expect(837460);
  TimerConductor_Run();
}
