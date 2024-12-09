#ifndef MEMORYRIVER_HPP
#define MEMORYRIVER_HPP

#include <fstream>

template<class T, size_t info_len = 2>
class MemoryRiver {
private:
  std::fstream file;
  std::string file_name;
  size_t sizeofT = sizeof(T);
  size_t length_ = 0;
public:
  MemoryRiver() = default;

  MemoryRiver(const std::string& file_name) : file_name(file_name) {}
  ~MemoryRiver() {
    if (!file.is_open()) {
      file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
    }
    write_info(length_, 0);
    file.close();
  }

  void initialise(std::string FN = "") {
    if (file.is_open()) {
      file.close();
    }
    if (FN != "") file_name = FN;
#ifdef FileClear
    file.open(file_name, std::ios::out);
    size_t tmp = 0;
    for (size_t i = 0; i < info_len; ++i) {
      file.write(reinterpret_cast<char *>(&tmp), sizeof(size_t));
    }
    file.close();
#else
    file.open(file_name, std::ios::in);
    if (!file) {
      file.open(file_name, std::ios::out);
      size_t tmp = 0;
      for (size_t i = 0; i < info_len; ++i) {
        file.write(reinterpret_cast<char *>(&tmp), sizeof(size_t));
      }
    } else {
      get_info(length_, 0);
    }
    file.close();
#endif
  }

  void get_info(size_t &tmp, size_t n) {
    if (n >= info_len) return;
    if (!file.is_open()) {
      file.open(file_name, std::ios::binary | std::ios::in | std::ios::out);
    }
    size_t t = n * sizeof(size_t);
    if (file.tellg() != t) {
      file.seekg(t);
    }
    file.read(reinterpret_cast<char *>(&tmp), sizeof(size_t));
  }

  void write_info(size_t tmp, size_t n) {
    if (n >= info_len) return;
    if (!file.is_open()) {
      file.open(file_name, std::ios::binary | std::ios::in | std::ios::out);
    }
    size_t t = n * sizeof(size_t);
    if (file.tellp() != t) {
      file.seekp(t);
    }
    file.write(reinterpret_cast<char *>(&tmp), sizeof(size_t));
  }

  size_t write(T &t) {
    size_t pos = length_;
    if (!file.is_open()) {
      file.open(file_name, std::ios::binary | std::ios::in | std::ios::out);
    }
    size_t tmp = length_ * sizeofT + info_len * sizeof(size_t);
    if (file.tellp() != tmp) {
      file.seekp(tmp);
    }
    file.write(reinterpret_cast<char *>(&t), sizeofT);
    ++length_;
    return pos;
  }

  size_t reserve() {
    size_t pos = length_;
    ++length_;
    return pos;
  }

  void update(T &t, const size_t index) {
    /* your code here */
    if (!file.is_open()) {
      file.open(file_name, std::ios::binary | std::ios::in | std::ios::out);
    }
    size_t tmp = index * sizeofT + info_len * sizeof(size_t);
    if (file.tellp() != tmp) {
      file.seekp(tmp);
    }
    file.write(reinterpret_cast<char *>(&t), sizeofT);
  }

  void read(T &t, const size_t index) {
    /* your code here */
    if (!file.is_open()) {
      file.open(file_name, std::ios::binary | std::ios::in | std::ios::out);
    }
    size_t tmp = index * sizeofT + info_len * sizeof(size_t);
    if (file.tellg() != tmp) {
      file.seekg(tmp);
    }
    file.read(reinterpret_cast<char *>(&t), sizeofT);
  }
};

#endif // MEMORYRIVER_HPP