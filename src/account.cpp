#include "account.h"
#include <vector>
#include <cassert>

bool Account::Online() const {
  return online_;
}

bool Account::CheckPassword(const std::array<char, 30> &password) const {
  return password == password_;
}

void Account::UpdatePassword(const std::array<char, 30> &new_password) {
  password_ = new_password;
}

std::array<char, 30> Account::UserID() const {
  return userID_;
}

unsigned char Account::Privilege() const {
  return privilege_;
}

/**
 *
 * @return if not finded, the privilege is zero.
 */
Account AccountSystem::Find(const std::array<char, 30> &userID) {
  std::vector<Account> res = accounts_.Find(userID);
  if (res.empty()) {
    return Account();
  } else {
    assert(res.size() == 1);
    return res[0];
  }
}

bool AccountSystem::Add(const Account &new_account) {
  Account tmp = Find(new_account.UserID());
  if (tmp.Privilege()) {
    return false;
  } else {
    accounts_.Insert(new_account.UserID(), new_account);
    return true;
  }
}

bool AccountSystem::Delete(const std::array<char, 30> &userID) {
  Account tmp = Find(userID);
  if (tmp.Privilege()) {
    if (tmp.Online()) {
      return false;
    }
    accounts_.Delete(userID, tmp);
    return true;
  } else {
    return false;
  }
}

bool AccountSystem::Modify(const std::array<char, 30> &userID, const Account &new_account) {
  Account tmp = Find(userID);
  if (tmp.Privilege()) {
    accounts_.Delete(userID, tmp);
    accounts_.Insert(userID, new_account);
    return true;
  } else {
    return false;
  }
}
