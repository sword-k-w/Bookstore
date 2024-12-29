#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <array>
#include <iostream>



class Command {
private:
  size_t cur_pos;
public:
  std::basic_string<unsigned int> command;
  Command() : cur_pos(0) {}
  unsigned int Unicode(bool);
  bool Read();
  std::basic_string<unsigned int> GetToken();
};

std::array<unsigned int, 30> ToUserID(const std::basic_string<unsigned int> &);
std::array<unsigned int, 30> ToPassword(const std::basic_string<unsigned int> &);
std::array<unsigned int, 30> ToUsername(const std::basic_string<unsigned int> &);
unsigned char ToPrivilege(const std::basic_string<unsigned int> &);
std::array<unsigned int, 20> ToISBN(const std::basic_string<unsigned int> &);
std::array<unsigned int, 60> ToBookName(const std::basic_string<unsigned int> &);
std::array<unsigned int, 60> ToAuthor(const std::basic_string<unsigned int> &);
std::array<unsigned int, 60> ToKeyword(const std::basic_string<unsigned int> &);
std::array<unsigned int, 60> ToKeywords(const std::basic_string<unsigned int> &);
int ToQuantity(const std::basic_string<unsigned int> &);
double ToPrice(const std::basic_string<unsigned int> &);
double ToTotalCost(const std::basic_string<unsigned int> &);
int ToCount(const std::basic_string<unsigned int> &);

std::array<unsigned int, 20> ToISBN_(const std::basic_string<unsigned int> &);
std::array<unsigned int, 60> ToBookName_(const std::basic_string<unsigned int> &);
std::array<unsigned int, 60> ToAuthor_(const std::basic_string<unsigned int> &);
std::array<unsigned int, 60> ToKeyword_(const std::basic_string<unsigned int> &);
std::array<unsigned int, 60> ToKeywords_(const std::basic_string<unsigned int> &);
double ToPrice_(const std::basic_string<unsigned int> &);

#endif //COMMAND_H
