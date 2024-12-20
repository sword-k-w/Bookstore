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

template<size_t length>
std::string ToString(const std::array<char, length> &tmp) {
  std::string res;
  for (size_t i = 0; i < length; ++i) {
    if (tmp[i] == '\0') {
      break;
    }
    res += tmp[i];
  }
  return res;
}

std::string ToString(const Book &book) {
  return "(-ISBN=" + ToString(book.ISBN_) + " -name=\"" + ToString(book.book_name_) + "\" -author=\"" + ToString(book.author_) + "\" -keyword=\"" + ToString(book.keyword_) + "\")";
}

std::array<char, 400> ToInfo(const std::string &s) {
  size_t size = s.size();
  if (size > 400) {
    std::cerr << "info too long!";
    exit(1);
  }
  std::array<char, 400> res;
  std::fill(res.begin(), res.end(), 0);
  for (size_t i = 0; i < size; ++i) {
    res[i] = s[i];
  }
  return res;
}

void LogSystem::RecordOperation(const Operation &operation) {
  std::array<char, 400> info;
  std::fill(info.begin(), info.end(), 0);
  Account account;
  switch (operation.operation_type_) {
    case Operation::kLogin : {
      info = ToInfo(std::to_string(operation.login_operation_.time_) + "\t\t" + ToString(operation.login_operation_.cur_account_.UserID()) + "\t\t\t\t\t\t\t\t\tlogin");
      if (operation.login_operation_.cur_account_.Privilege() >= 3) {
        account = operation.login_operation_.cur_account_;
      }
      break;
    }
    case Operation::kLogout : {
      info = ToInfo(std::to_string(operation.logout_operation_.time_) + "\t\t" + ToString(operation.logout_operation_.cur_account_.UserID()) + "\t\t\t\t\t\t\t\t\tlogout");
      if (operation.logout_operation_.cur_account_.Privilege() >= 3) {
        account = operation.logout_operation_.cur_account_;
      }
      break;
    }
    case Operation::kRegister : {
      info = ToInfo(std::to_string(operation.register_operation_.time_) + "\t\t" + ToString(operation.register_operation_.cur_account_.UserID()) + "\t\t\t\t\t\t\t\t\tregister");
      break;
    }
    case Operation::kCreate : {
      info = ToInfo(std::to_string(operation.create_operation_.time_) + "\t\t" + ToString(operation.create_operation_.cur_account_.UserID()) + "\t\t\t\t\t\t\t\t\tcreate " + ToString(operation.create_operation_.new_account_.UserID()));
      account = operation.create_operation_.cur_account_;
      break;
    }
    case Operation::kDelete : {
      info = ToInfo(std::to_string(operation.delete_operation_.time_) + "\t\t" + ToString(operation.delete_operation_.cur_account_.UserID()) + "\t\t\t\t\t\t\t\t\tdelete " + ToString(operation.delete_operation_.deleted.UserID()));
      account = operation.delete_operation_.cur_account_;
      break;
    }
    case Operation::kPassword : {
      info = ToInfo(std::to_string(operation.password_operation_.time_) + "\t\t" + ToString(operation.password_operation_.cur_account_.UserID()) + "\t\t\t\t\t\t\t\t\tmodify his/her password from " + ToString(operation.password_operation_.cur_account_.Password()) + " to " + ToString(operation.password_operation_.new_password_));
      break;
    }
    case Operation::kSearch : {
      info = ToInfo(std::to_string(operation.search_operation_.time_) + "\t\t" + ToString(operation.search_operation_.cur_account_.UserID()) + "\t\t\t\t\t\t\t\t\tsuccessfully search");
      break;
    }
    case Operation::kModify : {
      info = ToInfo(std::to_string(operation.modify_operation_.time_) + "\t\t" + ToString(operation.modify_operation_.cur_account_.UserID()) + "\t\t\t\t\t\t\t\t\tmodify the book " + ToString(operation.modify_operation_.book_) + " to " + ToString(operation.modify_operation_.new_book_));
      account = operation.modify_operation_.cur_account_;
      break;
    }
    case Operation::kImport : {
      std::string tt = std::to_string(operation.import_operation_.time_) + "\t\t" + ToString(operation.import_operation_.cur_account_.UserID()) + "\t\t\t\t\t\t\t\t\timport " + std::to_string(operation.import_operation_.quantity_) + " book with -ISBN= " + ToString(operation.import_operation_.book_.ISBN_) + " and cost " + std::to_string(operation.import_operation_.cost_);
      info = ToInfo(tt);
      account = operation.import_operation_.cur_account_;
      std::pair<double, double> tmp;
      finance_.Read(tmp, finance_.Length() - 1);
      tt += " || current income : " + std::to_string(tmp.first) + " || current expenditure : " + std::to_string(tmp.second);
      auto t = ToInfo(tt);
      finance_report_.Write(t);
      break;
    }
    case Operation::kBuy : {
      std::string tt = std::to_string(operation.buy_operation_.time_) + "\t\t" + ToString(operation.buy_operation_.cur_account_.UserID()) + "\t\t\t\t\t\t\t\t\tbuy " + std::to_string(operation.buy_operation_.quantity_) + " book with -ISBN= " + ToString(operation.buy_operation_.book_.ISBN_) + " and cost " + std::to_string(operation.buy_operation_.quantity_ * operation.buy_operation_.book_.price_);
      info = ToInfo(tt);
      std::pair<double, double> tmp;
      finance_.Read(tmp, finance_.Length() - 1);
      tt += " || current income : " + std::to_string(tmp.first) + " || current expenditure : " + std::to_string(tmp.second);
      auto t = ToInfo(tt);
      finance_report_.Write(t);
      break;
    }
    default : {
      info = ToInfo(std::to_string(operation.exit_operation_.time_) + "\t\t" + ToString(operation.exit_operation_.cur_account_.UserID()) + "\t\t\t\t\t\t\t\t\texit the system");
    }
  }
  log_.Write(info);
  if (account.Privilege()) {
    employee_report_.Insert(account.UserID(), info);
  }
}

std::ostream &operator << (std::ostream &os, const std::array<char, 400> &x) {
  for (size_t i = 0; i < 400; ++i) {
    if (x[i] != '\0') {
      os << x[i];
    }
  }
  return os;
}

void LogSystem::ReportLog() {
  std::cout << "The Log of Sword's Bookstore\n";
  std::cout << "time\tUserID\t\t\t\t\t\t\t\t\toperation\n";
  size_t size = log_.Length();
  for (size_t i = 0; i < size; ++i) {
    std::array<char, 400> tmp;
    log_.Read(tmp, i);
    std::cout << tmp << '\n';
  }
}

void LogSystem::ReportFinance() {
  size_t size = finance_report_.Length();
  for (size_t i = 0; i < size; ++i) {
    std::array<char, 400> tmp;
    finance_report_.Read(tmp, i);
    std::cout << tmp << '\n';
  }
}

void LogSystem::ReportEmployee() {
  auto tmp = employee_report_.FindAllInfo();
  std::cout << "root's working log:\n";
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
    }
    std::cout << x.second << '\n';
  }
}