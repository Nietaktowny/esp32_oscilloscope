#include "TimerModel.h"
#include "TaskScheduler.h"
#include "Types.h"

void TimerModel_UpdateTime(uint32 systemTime) {
  TaskScheduler_Update(systemTime);
}
