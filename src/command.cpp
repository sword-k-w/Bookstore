#include "command.h"
#include <iostream>
#include <set>

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

/**
 * @return res[0] = '\n' if invalid
 */
std::array<char, 20> ToISBN(const std::string &s) {
  size_t size = s.size();
  if (size > 20) {
    return {'\n'};
  }
  std::array<char, 20> res;
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
 * @return res[0] = '\n' if invalid
 */
std::array<char, 60> ToBookName(const std::string &s) {
  size_t size = s.size();
  if (size > 60) {
    return {'\n'};
  }
  std::array<char, 60> res;
  std::fill(res.begin(), res.end(), 0);
  for (size_t i = 0; i < size; ++i) {
    if (!isprint(s[i]) || s[i] == '\"') {
      return {'\n'};
    }
    res[i] = s[i];
  }
  return res;
}

/**
 * @return res[0] = '\n' if invalid
 */
std::array<char, 60> ToAuthor(const std::string &s) {
  return ToBookName(s);
}

/**
 * @return res[0] = '\n' if invalid
 */
std::array<char, 60> ToKeyword(const std::string &s) {
  size_t size = s.size();
  if (size > 60) {
    return {'\n'};
  }
  std::array<char, 60> res;
  std::fill(res.begin(), res.end(), 0);
  for (size_t i = 0; i < size; ++i) {
    if (!isprint(s[i]) || s[i] == '\"' || s[i] == '|') {
      return {'\n'};
    }
    res[i] = s[i];
  }
  return res;
}

std::array<char, 60> ToKeywords(const std::string &s) {
  size_t size = s.size();
  if (size > 60) {
    return {'\n'};
  }
  std::array<char, 60> res;
  std::fill(res.begin(), res.end(), 0);
  size_t las = 0;
  std::set<std::string> keywords;
  keywords.emplace("");
  for (size_t i = 0; i < size; ++i) {
    if (!isprint(s[i]) || s[i] == '\"') {
      return {'\n'};
    }
    if (s[i] == '|') {
      std::string tmp(std::string(s.begin() + las, s.begin() + i));
      if (keywords.count(tmp)) {
        return {'\n'};
      }
      keywords.emplace(tmp);
      las = i + 1;
    }
    res[i] = s[i];
  }
  if (las == size) {
    return {'\n'};
  }
  std::string tmp(std::string(s.begin() + las, s.end()));
  if (keywords.count(tmp)) {
    return {'\n'};
  }
  return res;
}


/**
 * @return -1 if invalid
 */
int ToQuantity(const std::string &s) {
  size_t size = s.size();
  if (size > 10) {
    return -1;
  }
  long long res = 0;
  for (size_t i = 0; i < size; ++i) {
    if (!isdigit(s[i])) {
      return -1;
    }
    res = res * 10 + s[i] - '0';
  }
  return res > 2147483647 ? -1 : res;
}

/**
 * @return -1 if invalid
 */
double ToPrice(const std::string &s) {
  size_t size = s.size();
  if (size > 13) {
    return -1;
  }
  double res = 0;
  for (size_t i = 0; i < size; ++i) {
    if (!isdigit(s[i]) && s[i] != '.') {
      return -1;
    } else if (s[i] != '.') {
      res = res * 10 + s[i] - '0';
    }
  }
  size_t pos = size;
  for (int i = size - 1; i >= 0; --i) {
    if (s[i] == '.') {
      if (pos < size) {
        return -1;
      }
      pos = i;
    }
  }
  if (pos + 1 == size || pos == 0) {
    return -1;
  }
  while (pos + 1 < size) {
    res /= 10;
    ++pos;
  }
  return res;
}

/**
 * @return -1 if invalid
 */
double ToTotalCoast(const std::string &s) {
  return ToPrice(s);
}

/**
 * @return -1 if invalid
 */
int ToCount(const std::string &s) {
  return ToQuantity(s);
}

/**
 * @return res[0] = '\n' if invalid
 */
std::array<char, 20> ToISBN_(const std::string &s) {
  if (s.size() < 7 || s[0] != '-' || s[1] != 'I' || s[2] != 'S' || s[3] != 'B' || s[4] != 'N' || s[5] != '=') {
    return {'\n'};
  }
  return ToISBN(std::string(s.begin() + 6, s.end()));
}

/**
 * @return res[0] = '\n' if invalid
 */
std::array<char, 60> ToBookName_(const std::string &s) {
  if (s.size() < 9 || s[0] != '-' || s[1] != 'n' || s[2] != 'a' || s[3] != 'm' || s[4] != 'e' || s[5] != '=' || s[6] != '\"' || *s.rbegin() != '\"') {
    return {'\n'};
  }
  return ToBookName(std::string(s.begin() + 7, s.end() - 1));
}

/**
 * @return res[0] = '\n' if invalid
 */
std::array<char, 60> ToAuthor_(const std::string &s) {
  if (s.size() < 11 || s[0] != '-' || s[1] != 'a' || s[2] != 'u' || s[3] != 't' || s[4] != 'h' || s[5] != 'o' || s[6] != 'r' || s[7] != '=' || s[8] != '\"' || *s.rbegin() != '\"') {
    return {'\n'};
  }
  return ToAuthor(std::string(s.begin() + 9, s.end() - 1));
}

/**
 * @return res[0] = '\n' if invalid
 */
std::array<char, 60> ToKeyword_(const std::string &s) {
  if (s.size() < 12 || s[0] != '-' || s[1] != 'k' || s[2] != 'e' || s[3] != 'y' || s[4] != 'w' || s[5] != 'o' || s[6] != 'r' || s[7] != 'd' || s[8] != '=' || s[9] != '\"' || *s.rbegin() != '\"') {
    return {'\n'};
  }
  return ToKeyword(std::string(s.begin() + 10, s.end() - 1));
}

/**
 * @return res[0] = '\n' if invalid
 */
std::array<char, 60> ToKeywords_(const std::string &s) {
  if (s.size() < 12 || s[0] != '-' || s[1] != 'k' || s[2] != 'e' || s[3] != 'y' || s[4] != 'w' || s[5] != 'o' || s[6] != 'r' || s[7] != 'd' || s[8] != '=' || s[9] != '\"' || *s.rbegin() != '\"') {
    return {'\n'};
  }
  return ToKeywords(std::string(s.begin() + 10, s.end() - 1));
}

/**
 * @return -1 if invalid
 */
double ToPrice_(const std::string &s) {
  if (s.size() < 8 || s[0] != '-' || s[1] != 'p' || s[2] != 'r' || s[3] != 'i' | s[4] != 'c' || s[5] != 'e' || s[6] != '=') {
    return -1;
  }
  return ToPrice(std::string(s.begin() + 7, s.end()));
}
