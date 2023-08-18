#include "MockTaskScheduler.h"
#include "MockUsartHardware.h"
#include "MockUsartModel.h"
#include "Types.h"
#include "UsartConductor.h"
#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

void testShouldInitializeHardwareWhenInitCalled(void) {
  UsartModel_GetBaudRateRegisterSetting_ExpectAndReturn(4);
  UsartModel_GetWakeupMessage_ExpectAndReturn("Hey there!");
  UsartHardware_TransmitString_Expect("Hey there!");
  UsartHardware_Init_Expect(4);

  UsartConductor_Init();
}

void testRunShouldNotDoAnythingIfSchedulerSaysItIsNotTimeYet(void) {
  TaskScheduler_DoUsart_ExpectAndReturn(FALSE);

  UsartConductor_Run();
}

void testRunShouldGetCurrentTemperatureAndTransmitIfSchedulerSaysItIsTime(
    void) {
  TaskScheduler_DoUsart_ExpectAndReturn(TRUE);
  UsartModel_GetFormattedTemperature_ExpectAndReturn("hey there");
  UsartHardware_TransmitString_Expect("hey there");

  UsartConductor_Run();
}
