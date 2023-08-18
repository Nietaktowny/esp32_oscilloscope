#include "Types.h"

#include "Executor.h"
#include "IntrinsicsWrapper.h"

#include "Model.h"
#include "TaskScheduler.h"
#include "TemperatureCalculator.h"
#include "TemperatureFilter.h"

#include "UsartBaudRateRegisterCalculator.h"
#include "UsartConductor.h"
#include "UsartConfigurator.h"
#include "UsartHardware.h"
#include "UsartModel.h"
#include "UsartPutChar.h"
#include "UsartTransmitBufferStatus.h"

#include "TimerConductor.h"
#include "TimerConfigurator.h"
#include "TimerHardware.h"
#include "TimerInterruptConfigurator.h"
#include "TimerInterruptHandler.h"
#include "TimerModel.h"

#include "AdcConductor.h"
#include "AdcHardware.h"
#include "AdcHardwareConfigurator.h"
#include "AdcModel.h"
#include "AdcTemperatureSensor.h"

int AppMain(void) {
  Executor_Init();

  while (Executor_Run())
    ;

  return 0;
}

#ifndef TEST
int main(void) { return AppMain(); }
#endif // TEST
