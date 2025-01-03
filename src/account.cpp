#include "account.h"
#include <vector>
#include <cassert>


// for debug
void Account::Print() const {
  std::cerr << "account info:" << online_count_ << '\t' << userID_ << '\t' << password_ << '\t' << username_ << '\t' << static_cast<int>(privilege_) << '\n';
}

size_t Account::OnlineCount() const {
  return online_count_;
}

void Account::ModifyOnlineCount(const int &delta) {
  assert(delta == 1 || delta == -1);
  assert(delta == 1 || online_count_ > 0);
  online_count_ += delta;
}


bool Account::CheckPassword(const std::array<unsigned int, 30> &password) const {
  return password == password_;
}

void Account::UpdatePassword(const std::array<unsigned int, 30> &new_password) {
  password_ = new_password;
}

std::array<unsigned int, 30> Account::UserID() const {
  return userID_;
}

std::array<unsigned int, 30> Account::Password() const {
  return password_;
}

std::array<unsigned int, 30> Account::Username() const {
  return username_;
}

unsigned char Account::Privilege() const {
  return privilege_;
}

bool operator < (const Account &x, const Account &y) {
  if (x.userID_ == y.userID_) {
    if (x.password_ == y.password_) {
      if (x.username_ == y.username_) {
        if (x.privilege_ == y.privilege_) {
          return x.online_count_ < y.online_count_;
        }
        return x.privilege_ < y.privilege_;
      }
      return x.username_ < y.username_;
    }
    return x.password_ < y.password_;
  }
  return x.userID_ < y.userID_;
}

bool operator == (const Account &x, const Account &y) {
  return x.userID_ == y.userID_ && x.password_ == y.password_ && x.username_ == y.username_ && x.privilege_ == y.privilege_ && x.online_count_ == y.online_count_;
}

/**
 *
 * @return if not finded, the privilege is zero.
 */
Account AccountSystem::Find(const std::array<unsigned int, 30> &userID) {
  std::vector<Account> res = accounts_.Find(userID);
  if (res.empty()) {
    return Account();
  } else {
    assert(res.size() == 1);
    return res[0];
  }
}

// for debug
std::vector<Account> AccountSystem::FindAll() {
  return accounts_.FindAll();
}

void AccountSystem::Add(const Account &new_account) {
  accounts_.Insert(new_account.UserID(), new_account);
}

void AccountSystem::Delete(const Account &account) {
  accounts_.Delete(account.UserID(), account);
}

void AccountSystem::Modify(const std::array<unsigned int, 30> &userID, const Account &new_account) {
  Account tmp = accounts_.Find(userID)[0];
  accounts_.Delete(userID, tmp);
  accounts_.Insert(userID, new_account);
}
