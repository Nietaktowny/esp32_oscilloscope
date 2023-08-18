#include "Executor.h"
#include "MockAdcConductor.h"
#include "MockIntrinsicsWrapper.h"
#include "MockModel.h"
#include "MockTimerConductor.h"
#include "MockUsartConductor.h"
#include "Types.h"
#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

void testInitShouldCallInitOfAllConductorsAndTheModel(void) {
  Model_Init_Expect();
  UsartConductor_Init_Expect();
  AdcConductor_Init_Expect();
  TimerConductor_Init_Expect();
  Interrupt_Enable_Expect();

  Executor_Init();
}

void testRunShouldCallRunForEachConductorAndReturnTrueAlways(void) {
  UsartConductor_Run_Expect();
  TimerConductor_Run_Expect();
  AdcConductor_Run_Expect();

  TEST_ASSERT_EQUAL(TRUE, Executor_Run());
}
