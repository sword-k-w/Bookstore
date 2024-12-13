#include "front_end.h"
#include <iostream>

FrontEnd::FrontEnd() : account_system_("accounts"), log_system_("finance") {
  time_ = log_system_.QueryTime();
  if (!time_) {
    account_system_.Add(Account(ToUserID("root"), ToPassword("sjtu"), ToUsername("sword"), 7));
  }
}

FrontEnd::~FrontEnd() {
  ++time_;
  log_system_.RecordTime(time_);
}

void FrontEnd::run() {
  while (true) {
    if (!cur_command_.Read()) {
      break;
    }
    std::string type = cur_command_.GetToken();
    if (type.empty()) {
      continue;
    } else if (type == "quit" || type == "exit") {
      break;
    } else if (type == "su") {
      Login();
    } else if (type == "logout") {
      Logout();
    } else if (type == "register") {
      Register();
    } else if (type == "passwd") {
      Password();
    } else if (type == "useradd") {
      Useradd();
    } else if (type == "delete") {
      Delete();
    } else if (type == "show") {
      Show();
    } else if (type == "buy") {
      Buy();
    } else if (type == "select") {
      Select();
    } else if (type == "modify") {
      Modify();
    } else if (type == "import") {
      Import();
    } else if (type == "log") {
      Log();
    } else if (type == "report") {
      Report();
    } else {
      std::cout << "Invalid\n";
    }
  }
}

void FrontEnd::Login() {
  std::string tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  std::array<char, 30> userID = ToUserID(tmp);
  if (userID[0] == '\n') {
    std::cout << "Invalid\n";
    return;
  }
  Account nxt = account_system_.Find(userID);
  if (!nxt.Privilege()) {
    std::cout << "Invalid\n";
    return;
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    if (nxt.Privilege() >= cur_account_.Privilege()) {
      std::cout << "Invalid\n";
      return;
    }
  } else {
    std::array<char, 30> password = ToPassword(tmp);
    if (password[0] == '\n' || !nxt.CheckPassword(password)) {
      std::cout << "Invalid\n";
      return;
    }
  }
  if (!cur_command_.GetToken().empty()) {
    std::cout << "Invalid\n";
    return;
  }
  nxt.ModifyOnlineCount(1);
  account_system_.Modify(userID, nxt);
  online_.emplace(userID);
  cur_account_ = nxt;
}

void FrontEnd::Logout() {
  if (online_.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  if (!cur_command_.GetToken().empty()) {
    std::cout << "Invalid\n";
    return;
  }
  cur_account_.ModifyOnlineCount(-1);
  account_system_.Modify(cur_account_.UserID(), cur_account_);
  online_.pop();
  if (online_.empty()) {
    cur_account_ = Account();
  } else {
    cur_account_ = account_system_.Find(online_.top());
  }
}

void FrontEnd::Register() {
  std::string tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  std::array<char, 30> userID = ToUserID(tmp);
  if (userID[0] == '\n' || account_system_.Find(userID).Privilege()) {
    std::cout << "Invalid\n";
    return;
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  std::array<char, 30> password = ToPassword(tmp);
  if (password[0] == '\n') {
    std::cout << "Invalid\n";
    return;
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  std::array<char, 30> username = ToUsername(tmp);
  if (username[0] == '\n') {
    std::cout << "Invalid\n";
    return;
  }
  if (!cur_command_.GetToken().empty()) {
    std::cout << "Invalid\n";
    return;
  }
  account_system_.Add(Account(userID, password, username));
}

void FrontEnd::Password() {
  if (cur_account_.Privilege() < 1) {
    std::cout << "Invalid\n";
    return;
  }
  std::string tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  std::array<char, 30> userID = ToUserID(tmp);
  if (userID[0] == '\n') {
    std::cout << "Invalid\n";
    return;
  }
  Account tmp_acc = account_system_.Find(userID);
  if (!tmp_acc.Privilege()) {
    std::cout << "Invalid\n";
    return;
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  std::array<char, 30> password = ToPassword(tmp);
  if (password[0] == '\n') {
    std::cout << "Invalid\n";
    return;
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    if (cur_account_.Privilege() != 7) {
      std::cout << "Invalid\n";
      return;
    }
    tmp_acc.UpdatePassword(password);
    if (userID == cur_account_.UserID()) {
      cur_account_.UpdatePassword(password);
    }
  } else {
    std::array<char, 30> password_prime = ToPassword(tmp);
    if (password_prime[0] == '\n' || !tmp_acc.CheckPassword(password)) {
      std::cout << "Invalid\n";
      return;
    }
    if (!cur_command_.GetToken().empty()) {
      std::cout << "Invalid\n";
      return;
    }
    tmp_acc.UpdatePassword(password_prime);
    if (userID == cur_account_.UserID()) {
      cur_account_.UpdatePassword(password);
    }
  }
  account_system_.Modify(userID, tmp_acc);
}

void FrontEnd::Useradd() {
  if (cur_account_.Privilege() < 3) {
    std::cout << "Invalid\n";
    return;
  }
  std::string tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  std::array<char, 30> userID = ToUserID(tmp);
  if (userID[0] == '\n' || account_system_.Find(userID).Privilege()) {
    std::cout << "Invalid\n";
    return;
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  std::array<char, 30> password = ToPassword(tmp);
  if (password[0] == '\n') {
    std::cout << "Invalid\n";
    return;
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  unsigned char privilege = ToPrivilege(tmp);
  if (!privilege || privilege > cur_account_.Privilege()) {
    std::cout << "Invalid\n";
    return;
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  std::array<char, 30> username = ToUsername(tmp);
  if (username[0] == '\n') {
    std::cout << "Invalid\n";
    return;
  }
  if (!cur_command_.GetToken().empty()) {
    std::cout << "Invalid\n";
    return;
  }
  account_system_.Add(Account(userID, password, username, privilege));
}

void FrontEnd::Delete() {
  if (cur_account_.Privilege() != 7) {
    std::cout << "Invalid\n";
    return;
  }
  std::string tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  std::array<char, 30> userID = ToUserID(tmp);
  if (userID[0] == '\n') {
    std::cout << "Invalid\n";
    return;
  }
  if (!cur_command_.GetToken().empty()) {
    std::cout << "Invalid\n";
    return;
  }
  Account tmp_acc = account_system_.Find(userID);
  if (!tmp_acc.Privilege() || tmp_acc.OnlineCount()) {
    std::cout << "Invalid\n";
    return;
  }
  account_system_.Delete(userID);
}

void FrontEnd::Show() {

}

void FrontEnd::Buy() {

}

void FrontEnd::Select() {

}

void FrontEnd::Modify() {

}

void FrontEnd::Import() {

}

void FrontEnd::Log() {

}

void FrontEnd::Report() {

}
