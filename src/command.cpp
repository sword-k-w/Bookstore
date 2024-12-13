#include "command.h"
#include <iostream>

bool Command::Read() {
  cur_pos = 0;
  command = "";
  char ch = getchar();
  while (true) {
    if (ch == EOF) {
      return false;
    }
    if (ch == '\r' || ch == '\n') {
      return true;
    }
    command += ch;
    ch = getchar();
  }
}

/**
 * @return empty string if failed to find a token
 */
std::string Command::GetToken() {
  size_t size = command.size();
  while (cur_pos < size && command[cur_pos] == ' ') {
    ++cur_pos;
  }
  std::string res;
  while (cur_pos < size && command[cur_pos] != ' ') {
    res += command[cur_pos];
    ++cur_pos;
  }
  return res;
}

/**
 * @return res[0] = '\n' if invalid
 */
std::array<char, 30> ToUserID(const std::string &s) {
  size_t size = s.size();
  if (size > 30) {
    return {'\n'};
  }
  std::array<char, 30> res;
  std::fill(res.begin(), res.end(), 0);
  for (size_t i = 0; i < size; ++i) {
    if (!isalnum(s[i]) && s[i] != '_') {
      return {'\n'};
    }
    res[i] = s[i];
  }
  return res;
}

/**
 * @return res[0] = '\n' if invalid
 */
std::array<char, 30> ToPassword(const std::string &s) {
  return ToUserID(s);
}

/**
 * @return res[0] = '\n' if invalid
 */
std::array<char, 30> ToUsername(const std::string &s) {
  size_t size = s.size();
  if (size > 30) {
    return {'\n'};
  }
  std::array<char, 30> res;
  std::fill(res.begin(), res.end(), 0);
  for (size_t i = 0; i < size; ++i) {
    if (!isprint(s[i])) {
      return {'\n'};
    }
    res[i] = s[i];
  }
  return res;
}

/**
 * @return 0 if invalid
 */
unsigned char ToPrivilege(const std::string &s) {
  if (s.size() != 1 || (s[0] != '1' && s[0] != '3' && s[0] != '7')) {
    return 0;
  }
  return s[0] - '0';
}




