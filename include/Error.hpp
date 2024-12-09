#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>

class Error {
private:
  std::string message;
public:
  Error() = delete;
  Error(std::string s) : message(s) {}
  std::string Display() const {
    return message;
  }
};

#endif //ERROR_HPP