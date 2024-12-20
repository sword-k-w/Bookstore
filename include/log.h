#ifndef LOG_H
#define LOG_H
#include "memory_river.hpp"
#include "account.h"

struct BasicInfo {
  size_t time_;
  Account cur_account_;
};

struct RegisterOperation : BasicInfo {
};

struct LoginOperation : BasicInfo {
};

struct LogoutOperation : BasicInfo {
};

struct CreateOperation : BasicInfo {
  Account new_account_;
};

struct DeleteOperation : BasicInfo {
  Account deleted;
};

struct PasswordOperation : BasicInfo {
  std::array<char, 30> new_password_;
  Account modified_;
};

struct SearchOperation : BasicInfo {
};

struct BuyOperation : BasicInfo {
  Book book_;
  int quantity_;
};

struct ModifyOperation : BasicInfo {
  Book book_;
  Book new_book_;
};

struct ImportOperation : BasicInfo {
  Book book_;
  int quantity_;
  double cost_;
};

struct ExitOperation : BasicInfo {
};

struct Operation {
  enum {
    kRegister, kLogin, kLogout, kCreate, kDelete, kPassword, kSearch, kBuy, kModify, kImport, kExit
  } operation_type_;
  union {
    RegisterOperation register_operation_;
    LoginOperation login_operation_;
    LogoutOperation logout_operation_;
    CreateOperation create_operation_;
    DeleteOperation delete_operation_;
    PasswordOperation password_operation_;
    SearchOperation search_operation_;
    BuyOperation buy_operation_;
    ModifyOperation modify_operation_;
    ImportOperation import_operation_;
    ExitOperation exit_operation_;
  };
};

class LogSystem {
private:
  MemoryRiver<std::pair<double, double>> finance_;
  MemoryRiver<std::array<char, 300>> finance_report_;
  MemoryRiver<std::array<char, 300>> log_;
  UnrolledLinkedList<std::array<char, 30>, std::array<char, 300>> employee_report_;
public:
  LogSystem() = delete;
  LogSystem(const std::string &name) : finance_(name + "_finance"), finance_report_(name + "_finance_report"), log_(name), employee_report_(name + "_employee_report") {
    finance_.Initialise();
    finance_report_.Initialise();
    log_.Initialise();
  }
  void RecordTime(const size_t &);
  size_t QueryTime();
  void RecordTrade(const double &);
  void QueryTrade();
  void QueryTrade(const size_t &);
  void RecordOperation(const Operation &);
  void ReportFinance();
  void ReportEmployee();
  void ReportLog();
};

#endif //LOG_H
