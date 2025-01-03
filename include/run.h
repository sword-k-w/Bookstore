#ifndef FRONT_END_H
#define FRONT_END_H

#include <stack>
#include "account.h"
#include "book.h"
#include "log.h"
#include "command.h"

class Invalid : std::exception {
public:
  const char *what() const noexcept override {
    return"Invalid\n";
  }
};

class Run {
private:
  AccountSystem account_system_;
  BookSystem book_system_;
  LogSystem log_system_;
  size_t time_;
  std::stack<std::array<unsigned int, 30>> online_;
  std::stack<int> select_;
  int cur_book_;
  Account cur_account_;
  Command cur_command_;
public:
  Run();
  ~Run();
  void RunProgram();
  void Login();
  void Logout(bool);
  void Register();
  void Password();
  void Useradd();
  void Delete();
  void Show();
  void Buy();
  void Select();
  void Modify();
  void Import();
  void Log();
  void Report();
  void Info();
};

#endif //FRONT_END_H
