#include "log.h"
#include <iostream>
#include <iomanip>

void LogSystem::RecordTime(const size_t &time) {
  finance_.WriteInfo(time, 1);
}

size_t LogSystem::QueryTime(){
  size_t res;
  finance_.GetInfo(res, 1);
  return res;
}

void LogSystem::RecordTrade(const double &money) {
  std::pair<double, double> tmp;
  finance_.Read(tmp, finance_.Length() - 1);
  if (money > 0) {
    tmp.first += money;
  } else {
    tmp.second -= money;
  }
  finance_.Write(tmp);
}

void LogSystem::QueryTrade() {
  if (!finance_.Length()) {
    std::cout << "+ 0.00 - 0.00\n";
    return;
  }
  std::pair<double, double> tmp;
  finance_.Read(tmp, finance_.Length() - 1);
  std::cout << std::fixed << std::setprecision(2) << "+ " << tmp.first << " - " << tmp.second << '\n';
}

void LogSystem::QueryTrade(const size_t &count) {
  if (!count) {
    std::cout << "\n";
    return;
  }
  if (count > finance_.Length()) {
    std::cout << "Invalid\n";
    return;
  }
  if (count == finance_.Length()) {
    QueryTrade();
  } else {
    std::pair<double, double> tmp1, tmp2;
    finance_.Read(tmp1, finance_.Length() - 1);
    finance_.Read(tmp2, finance_.Length() - 1 - count);
    std::cout << std::fixed << std::setprecision(2) << "+ " << tmp1.first - tmp2.first << " - " << tmp1.second - tmp2.second << '\n';
  }
}
