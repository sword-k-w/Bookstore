#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <array>

class Command {
private:
  size_t cur_pos;
public:
  std::string command;
  Command() : command(""), cur_pos(0) {}
  bool Read();
  std::string GetToken();
};

std::array<char, 30> ToUserID(const std::string &);
std::array<char, 30> ToPassword(const std::string &);
std::array<char, 30> ToUsername(const std::string &);
unsigned char ToPrivilege(const std::string &);
std::array<char, 20> ToISBN(const std::string &);
std::array<char, 60> ToBookName(const std::string &);
std::array<char, 60> ToAuthor(const std::string &);
std::array<char, 60> ToKeyword(const std::string &);
int ToQuantity(const std::string &);
double ToPrice(const std::string &);
double ToTotalCoast(const std::string &);
int ToCount(const std::string &);

std::array<char, 20> ToISBN_(const std::string &);
std::array<char, 60> ToBookName_(const std::string &);
std::array<char, 60> ToAuthor_(const std::string &);
std::array<char, 60> ToKeyword_(const std::string &);
double ToPrice_(const std::string &);

#endif //COMMAND_H
