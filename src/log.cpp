#include "log.h"

void LogSystem::RecordTime(const size_t &time) {
  finance_.WriteInfo(time, 1);
}

size_t LogSystem::QueryTime(){
  size_t res;
  finance_.GetInfo(res, 1);
  return res;
}