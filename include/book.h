#ifndef BOOK_H
#define BOOK_H

#include <array>

struct Book {
  std::array<char, 20> ISBN_;
  std::array<char, 60> book_name_;
  std::array<char, 60> author_;
  std::array<char, 60> keyword_;
  int stock_;
  double price_;
};



#endif //BOOK_H
