#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "unrolled_linked_list.h"
#include <array>
#include <string>

class Account {
private:
  bool online_;
  std::array<char, 30> userID_;
  std::array<char, 30> password_;
  std::array<char, 30> username_;
  unsigned char privilege_;
public:
  Account() : privilege_(0) {}
  Account(const std::array<char, 30> &userID, const std::array<char, 30> &password, const std::array<char, 30> &username, const unsigned char &privilege = 1) : online_(false), userID_(userID), password_(password),username_(username), privilege_(privilege) {}
  bool CheckPassword(const std::array<char, 30> &) const;
  void UpdatePassword(const std::array<char, 30> &);
  bool Online() const;
  std::array<char, 30> UserID() const;
  unsigned char Privilege() const;
};

class AccountSystem {
private:
  UnrolledLinkedList<std::array<char, 30>, Account> accounts_;
public:
  AccountSystem() = delete;
  AccountSystem(const std::string &name) : accounts_(name) {}
  Account Find(const std::array<char, 30> &);
  bool Add(const Account &);
  bool Delete(const std::array<char, 30> &);
  bool Modify(const std::array<char, 30> &, const Account &);
};

#endif //ACCOUNT_H
