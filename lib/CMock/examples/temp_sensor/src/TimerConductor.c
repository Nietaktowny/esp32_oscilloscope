#include "TimerConductor.h"
#include "TimerHardware.h"
#include "TimerInterruptHandler.h"
#include "TimerModel.h"
#include "Types.h"

void TimerConductor_Init(void) { TimerHardware_Init(); }

void TimerConductor_Run(void) { TimerModel_UpdateTime(Timer_GetSystemTime()); }
