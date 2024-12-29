#include "command.h"
#include <iostream>
#include <set>
#include <cassert>


bool Command::Read() {
  cur_pos = 0;
  command.clear();
  char ch;
  while (std::cin.get(ch)) {
    if (ch == '\r' || ch == '\n') {
      return true;
    }
    unsigned int res = 0;
    if ((ch & 0x80) == 0) { // 0xxxxxxxx
      res = ch;
    } else if ((ch & 0xE0) == 0xC0) { // 110xxxxx 10xxxxxxx
      res = (ch & 0x1F) << 6;
      assert(std::cin.get(ch));
      res |= ch & 0x3F;
    } else if ((ch & 0xF0) == 0xE0) { // 1110xxxx 10xxxxxxx 10xxxxxx
      res = (ch & 0x0F) << 12;
      assert(std::cin.get(ch));
      res |= (ch & 0x3F) << 6;
      assert(std::cin.get(ch));
      res |= ch & 0x3F;
    } else {
      assert((ch & 0xF8) == 0xF0); // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
      res = (ch & 0x07) << 18;
      assert(std::cin.get(ch));
      res |= (ch & 0x3F) << 12;
      assert(std::cin.get(ch));
      res |= (ch & 0x3F) << 6;
      assert(std::cin.get(ch));
      res |= ch & 0x3F;
    }
    command += ch;
  }
  return false;
}
/**
 * @return empty string if failed to find a token
 */
std::basic_string<unsigned int> Command::GetToken() {
  size_t size = command.size();
  while (cur_pos < size && command[cur_pos] == ' ') {
    ++cur_pos;
  }
  std::basic_string<unsigned int> res;
  while (cur_pos < size && command[cur_pos] != ' ') {
    res += command[cur_pos];
    ++cur_pos;
  }
  return res;
}

/**
 * @return res[0] = '\n' if invalid
 */
std::array<unsigned int, 30> ToUserID(const std::basic_string<unsigned int> &s) {
  size_t size = s.size();
  if (size > 30) {
    return {'\n'};
  }
  std::array<unsigned int, 30> res;
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
std::array<unsigned int, 30> ToPassword(const std::basic_string<unsigned int> &s) {
  return ToUserID(s);
}

/**
 * @return res[0] = '\n' if invalid
 */
std::array<unsigned int, 30> ToUsername(const std::basic_string<unsigned int> &s) {
  size_t size = s.size();
  if (size > 30) {
    return {'\n'};
  }
  std::array<unsigned int, 30> res;
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
unsigned char ToPrivilege(const std::basic_string<unsigned int> &s) {
  if (s.size() != 1 || (s[0] != '1' && s[0] != '3' && s[0] != '7')) {
    return 0;
  }
  return s[0] - '0';
}

/**
 * @return res[0] = '\n' if invalid
 */
std::array<unsigned int, 20> ToISBN(const std::basic_string<unsigned int> &s) {
  size_t size = s.size();
  if (size > 20) {
    return {'\n'};
  }
  std::array<unsigned int, 20> res;
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
std::array<unsigned int, 60> ToBookName(const std::basic_string<unsigned int> &s) {
  size_t size = s.size();
  if (size > 60) {
    return {'\n'};
  }
  std::array<unsigned int, 60> res;
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
std::array<unsigned int, 60> ToAuthor(const std::basic_string<unsigned int> &s) {
  return ToBookName(s);
}

/**
 * @return res[0] = '\n' if invalid
 */
std::array<unsigned int, 60> ToKeyword(const std::basic_string<unsigned int> &s) {
  size_t size = s.size();
  if (size > 60) {
    return {'\n'};
  }
  std::array<unsigned int, 60> res;
  std::fill(res.begin(), res.end(), 0);
  for (size_t i = 0; i < size; ++i) {
    if (!isprint(s[i]) || s[i] == '\"' || s[i] == '|') {
      return {'\n'};
    }
    res[i] = s[i];
  }
  return res;
}

std::array<unsigned int, 60> ToKeywords(const std::basic_string<unsigned int> &s) {
  size_t size = s.size();
  if (size > 60) {
    return {'\n'};
  }
  std::array<unsigned int, 60> res;
  std::fill(res.begin(), res.end(), 0);
  size_t las = 0;
  std::set<std::basic_string<unsigned int>> keywords;
  keywords.insert({});
  for (size_t i = 0; i < size; ++i) {
    if (!isprint(s[i]) || s[i] == '\"') {
      return {'\n'};
    }
    if (s[i] == '|') {
      std::basic_string<unsigned int> tmp(std::basic_string<unsigned int>(s.begin() + las, s.begin() + i));
      if (keywords.count(tmp)) {
        return {'\n'};
      }
      keywords.emplace(tmp);
      las = i + 1;
    }
    res[i] = s[i];
  }
  std::basic_string<unsigned int> tmp(std::basic_string<unsigned int>(s.begin() + las, s.end()));
  if (keywords.count(tmp)) {
    return {'\n'};
  }
  return res;
}

/**
 * @return -1 if invalid
 */
int ToQuantity(const std::basic_string<unsigned int> &s) {
  size_t size = s.size();
  if (size > 10) {
    return -1;
  }
  if (size > 1 && s[0] == '0') {
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
double ToPrice(const std::basic_string<unsigned int> &s) {
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
  if (pos + 1 == size || pos == 0 || size - pos > 3) {
    return -1;
  }
  if (pos > 1 && s[0] == '0') {
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
double ToTotalCost(const std::basic_string<unsigned int> &s) {
  return ToPrice(s);
}

/**
 * @return -1 if invalid
 */
int ToCount(const std::basic_string<unsigned int> &s) {
  return ToQuantity(s);
}

/**
 * @return res[0] = '\n' if invalid
 */
std::array<unsigned int, 20> ToISBN_(const std::basic_string<unsigned int> &s) {
  if (s.size() < 7 || s[0] != '-' || s[1] != 'I' || s[2] != 'S' || s[3] != 'B' || s[4] != 'N' || s[5] != '=') {
    return {'\n'};
  }
  return ToISBN(std::basic_string<unsigned int>(s.begin() + 6, s.end()));
}

/**
 * @return res[0] = '\n' if invalid
 */
std::array<unsigned int, 60> ToBookName_(const std::basic_string<unsigned int> &s) {
  if (s.size() < 9 || s[0] != '-' || s[1] != 'n' || s[2] != 'a' || s[3] != 'm' || s[4] != 'e' || s[5] != '=' || s[6] != '\"' || *s.rbegin() != '\"') {
    return {'\n'};
  }
  return ToBookName(std::basic_string<unsigned int>(s.begin() + 7, s.end() - 1));
}

/**
 * @return res[0] = '\n' if invalid
 */
std::array<unsigned int, 60> ToAuthor_(const std::basic_string<unsigned int> &s) {
  if (s.size() < 11 || s[0] != '-' || s[1] != 'a' || s[2] != 'u' || s[3] != 't' || s[4] != 'h' || s[5] != 'o' || s[6] != 'r' || s[7] != '=' || s[8] != '\"' || *s.rbegin() != '\"') {
    return {'\n'};
  }
  return ToAuthor(std::basic_string<unsigned int>(s.begin() + 9, s.end() - 1));
}

/**
 * @return res[0] = '\n' if invalid
 */
std::array<unsigned int, 60> ToKeyword_(const std::basic_string<unsigned int> &s) {
  if (s.size() < 12 || s[0] != '-' || s[1] != 'k' || s[2] != 'e' || s[3] != 'y' || s[4] != 'w' || s[5] != 'o' || s[6] != 'r' || s[7] != 'd' || s[8] != '=' || s[9] != '\"' || *s.rbegin() != '\"') {
    return {'\n'};
  }
  return ToKeyword(std::basic_string<unsigned int>(s.begin() + 10, s.end() - 1));
}

/**
 * @return res[0] = '\n' if invalid
 */
std::array<unsigned int, 60> ToKeywords_(const std::basic_string<unsigned int> &s) {
  if (s.size() < 12 || s[0] != '-' || s[1] != 'k' || s[2] != 'e' || s[3] != 'y' || s[4] != 'w' || s[5] != 'o' || s[6] != 'r' || s[7] != 'd' || s[8] != '=' || s[9] != '\"' || *s.rbegin() != '\"') {
    return {'\n'};
  }
  return ToKeywords(std::basic_string<unsigned int>(s.begin() + 10, s.end() - 1));
}

/**
 * @return -1 if invalid
 */
double ToPrice_(const std::basic_string<unsigned int> &s) {
  if (s.size() < 8 || s[0] != '-' || s[1] != 'p' || s[2] != 'r' || s[3] != 'i' | s[4] != 'c' || s[5] != 'e' || s[6] != '=') {
    return -1;
  }
  return ToPrice(std::basic_string<unsigned int>(s.begin() + 7, s.end()));
}
