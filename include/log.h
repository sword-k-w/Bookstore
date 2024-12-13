#ifndef LOG_H
#define LOG_H
#include "memory_river.hpp"

class LogSystem {
private:
  MemoryRiver<std::pair<double, double>> finance_;
public:
  LogSystem() = delete;
  LogSystem(const std::string &name) : finance_(name) {
    finance_.Initialise();
  }
  void RecordTime(const size_t &);
  size_t QueryTime();
};

#endif //LOG_H
