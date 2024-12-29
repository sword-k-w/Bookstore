#include "log.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

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

std::string ToString(const Book &book) {
  return "(-ISBN=" + ToString(book.ISBN_) + " -name=\"" + ToString(book.book_name_) + "\" -author=\"" + ToString(book.author_) + "\" -keyword=\"" + ToString(book.keyword_) + "\")";
}

std::array<unsigned int, 400> ToInfo(const std::string &s) {
  size_t size = s.size();
  if (size > 400) {
    std::cerr << "info too long!";
    exit(1);
  }
  std::array<unsigned int, 400> res;
  std::fill(res.begin(), res.end(), 0);
  for (size_t i = 0; i < size; ++i) {
    res[i] = s[i];
  }
  return res;
}

constexpr int kWidthParameter1 = 7;
constexpr int kWidthParameter2 = 35;

void LogSystem::RecordOperation(const Operation &operation) {
  std::array<unsigned int, 400> info;
  std::fill(info.begin(), info.end(), 0);
  Account account;
  std::stringstream ss;
  switch (operation.operation_type_) {
    case Operation::kLogin : {
      ss << std::left << std::setw(kWidthParameter1) << std::to_string(operation.login_operation_.time_);
      ss << std::left << std::setw(kWidthParameter2) << ToString(operation.login_operation_.cur_account_.UserID());
      ss << "login";
      info = ToInfo(ss.str());
      if (operation.login_operation_.cur_account_.Privilege() >= 3) {
        account = operation.login_operation_.cur_account_;
      }
      break;
    }
    case Operation::kLogout : {
      ss << std::left << std::setw(kWidthParameter1) << std::to_string(operation.logout_operation_.time_);
      ss << std::left << std::setw(kWidthParameter2) << ToString(operation.logout_operation_.cur_account_.UserID());
      ss << "logout";
      info = ToInfo(ss.str());
      if (operation.logout_operation_.cur_account_.Privilege() >= 3) {
        account = operation.logout_operation_.cur_account_;
      }
      break;
    }
    case Operation::kRegister : {
      ss << std::left << std::setw(kWidthParameter1) << std::to_string(operation.register_operation_.time_);
      ss << std::left << std::setw(kWidthParameter2) << ToString(operation.register_operation_.cur_account_.UserID());
      ss << "register";
      info = ToInfo(ss.str());
      break;
    }
    case Operation::kCreate : {
      ss << std::left << std::setw(kWidthParameter1) << std::to_string(operation.create_operation_.time_);
      ss << std::left << std::setw(kWidthParameter2) << ToString(operation.create_operation_.cur_account_.UserID());
      ss << "create " << ToString(operation.create_operation_.new_account_.UserID());
      info = ToInfo(ss.str());
      account = operation.create_operation_.cur_account_;
      break;
    }
    case Operation::kDelete : {
      ss << std::left << std::setw(kWidthParameter1) << std::to_string(operation.delete_operation_.time_);
      ss << std::left << std::setw(kWidthParameter2) << ToString(operation.delete_operation_.cur_account_.UserID());
      ss << "delete " << ToString(operation.delete_operation_.deleted.UserID());
      info = ToInfo(ss.str());
      account = operation.delete_operation_.cur_account_;
      break;
    }
    case Operation::kPassword : {
      ss << std::left << std::setw(kWidthParameter1) << std::to_string(operation.password_operation_.time_);
      ss << std::left << std::setw(kWidthParameter2) << ToString(operation.password_operation_.cur_account_.UserID());
      ss << "modify his/her password from " << ToString(operation.password_operation_.cur_account_.Password()) << " to " << ToString(operation.password_operation_.new_password_);
      info = ToInfo(ss.str());
      break;
    }
    case Operation::kSearch : {
      ss << std::left << std::setw(kWidthParameter1) << std::to_string(operation.search_operation_.time_);
      ss << std::left << std::setw(kWidthParameter2) << ToString(operation.search_operation_.cur_account_.UserID());
      ss << "successfully search";
      info = ToInfo(ss.str());
      break;
    }
    case Operation::kModify : {
      ss << std::left << std::setw(kWidthParameter1) << std::to_string(operation.modify_operation_.time_);
      ss << std::left << std::setw(kWidthParameter2) << ToString(operation.modify_operation_.cur_account_.UserID());
      ss << "modify the book " + ToString(operation.modify_operation_.book_) + " to " + ToString(operation.modify_operation_.new_book_);
      info = ToInfo(ss.str());
      account = operation.modify_operation_.cur_account_;
      break;
    }
    case Operation::kImport : {
      ss << std::left << std::setw(kWidthParameter1) << std::to_string(operation.import_operation_.time_);
      ss << std::left << std::setw(kWidthParameter2) << ToString(operation.import_operation_.cur_account_.UserID());
      ss << "import " << std::to_string(operation.import_operation_.quantity_) + " book with -ISBN= " + ToString(operation.import_operation_.book_.ISBN_) + " and cost " + std::to_string(operation.import_operation_.cost_);
      info = ToInfo(ss.str());
      account = operation.import_operation_.cur_account_;
      std::pair<double, double> tmp;
      finance_.Read(tmp, finance_.Length() - 1);
      ss << " || current income : " << std::to_string(tmp.first) << " || current expenditure : " << std::to_string(tmp.second);
      auto t = ToInfo(ss.str());
      finance_report_.Write(t);
      break;
    }
    case Operation::kBuy : {
      ss << std::left << std::setw(kWidthParameter1) << std::to_string(operation.buy_operation_.time_);
      ss << std::left << std::setw(kWidthParameter2) << ToString(operation.buy_operation_.cur_account_.UserID());
      ss << "buy " << std::to_string(operation.buy_operation_.quantity_) + " book with -ISBN= " + ToString(operation.buy_operation_.book_.ISBN_) + " and cost " + std::to_string(operation.buy_operation_.quantity_ * operation.buy_operation_.book_.price_);
      info = ToInfo(ss.str());
      std::pair<double, double> tmp;
      finance_.Read(tmp, finance_.Length() - 1);
      ss << " || current income : " << std::to_string(tmp.first) << " || current expenditure : " << std::to_string(tmp.second);
      auto t = ToInfo(ss.str());
      finance_report_.Write(t);
      break;
    }
    default : {
      ss << std::left << std::setw(kWidthParameter1) << std::to_string(operation.exit_operation_.time_);
      ss << std::left << std::setw(kWidthParameter2) << ToString(operation.exit_operation_.cur_account_.UserID());
      ss << "exit the system";
      info = ToInfo(ss.str());
    }
  }
  log_.Write(info);
  if (account.Privilege()) {
    employee_report_.Insert(account.UserID(), info);
  }
}

std::ostream &operator << (std::ostream &os, const std::array<unsigned int, 400> &x) {
  for (size_t i = 0; i < 400; ++i) {
    if (x[i] != '\0') {
      os << x[i];
    }
  }
  return os;
}

void LogSystem::ReportLog() {
  std::cout << "The Log of Sword's Bookstore\n";
  std::cout << std::left << std::setw(kWidthParameter1) << "time";
  std::cout << std::left << std::setw(kWidthParameter2) << "UserID" << "operation\n";
  size_t size = log_.Length();
  for (size_t i = 0; i < size; ++i) {
    std::array<unsigned int, 400> tmp;
    log_.Read(tmp, i);
    std::cout << tmp << '\n';
  }
}

void LogSystem::ReportFinance() {
  std::cout << "The Finance Report of Sword's Bookstore\n";
  std::cout << std::left << std::setw(kWidthParameter1) << "time";
  std::cout << std::left << std::setw(kWidthParameter2) << "UserID" << "operation\n";
  size_t size = finance_report_.Length();
  for (size_t i = 0; i < size; ++i) {
    std::array<unsigned int, 400> tmp;
    finance_report_.Read(tmp, i);
    std::cout << tmp << '\n';
  }
}

int ToTime(const std::array<unsigned int, 400> &x) {
  int res = 0;
  for (size_t i = 0; i < 6; ++i) {
    if (isdigit(x[i])) {
      res = res * 10 + x[i] - '0';
    }
  }
  return res;
}

void LogSystem::ReportEmployee() {
  std::cout << "The Employee Report of Sword's Bookstore\n";
  auto tmp = employee_report_.FindAllInfo();
  std::sort(tmp.begin(), tmp.end(), [&] (const std::pair<std::array<unsigned int, 30>, std::array<unsigned int, 400>> &x, const std::pair<std::array<unsigned int, 30>, std::array<unsigned int, 400>> &y) {
    return ToTime(x.second) < ToTime(y.second);
  });
  std::cout << "root's working log:\n";
  std::cout << std::left << std::setw(kWidthParameter1) << "time";
  std::cout << std::left << std::setw(kWidthParameter2) << "UserID" << "operation\n";
  for (auto &x : tmp) {
    if (ToString(x.first) == "root") {
      std::cout << x.second << '\n';
    }
  }
  std::string las = "root";
  for (auto &x : tmp) {
    std::string cur = ToString(x.first);
    if (cur == "root") {
      continue;
    }
    if (cur != las) {
      std::cout << '\n';
      las = cur;
      std::cout << cur << "'s working log:\n";
      std::cout << std::left << std::setw(kWidthParameter1) << "time";
      std::cout << std::left << std::setw(kWidthParameter2) << "UserID" << "operation\n";
    }
    std::cout << x.second << '\n';
  }
}