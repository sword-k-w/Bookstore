#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "book.h"
#include "unrolled_linked_list.hpp"
#include <array>
#include <string>

class Account {
private:
  size_t online_count_;
  std::array<unsigned int, 30> userID_;
  std::array<unsigned int, 30> password_;
  std::array<unsigned int, 30> username_;
  unsigned int privilege_;
public:
  Account() : privilege_(0) {}
  Account(const std::array<unsigned int, 30> &userID, const std::array<unsigned int, 30> &password, const std::array<unsigned int, 30> &username, const unsigned char &privilege = 1) : online_count_(0), userID_(userID), password_(password),username_(username), privilege_(privilege) {}
  bool CheckPassword(const std::array<unsigned int, 30> &) const;
  void UpdatePassword(const std::array<unsigned int, 30> &);
  void ModifyOnlineCount(const int &);
  size_t OnlineCount() const;
  std::array<unsigned int, 30> UserID() const;
  std::array<unsigned int, 30> Password() const;
  std::array<unsigned int, 30> Username() const;
  unsigned char Privilege() const;
  friend bool operator < (const Account &x, const Account &y);
  friend bool operator == (const Account &x, const Account &y);
  void Print() const;
};

class AccountSystem {
private:
  UnrolledLinkedList<std::array<unsigned int, 30>, Account> accounts_;
public:
  AccountSystem() = delete;
  AccountSystem(const std::string &name) : accounts_(name) {}
  Account Find(const std::array<unsigned int, 30> &);
  std::vector<Account> FindAll();
  void Add(const Account &);
  void Delete(const Account &);
  void Modify(const std::array<unsigned int, 30> &, const Account &);
};

#endif //ACCOUNT_H
