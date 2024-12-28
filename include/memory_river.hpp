#ifndef MEMORYRIVER_HPP
#define MEMORYRIVER_HPP

#include <fstream>
#include <vector>
#include <iostream>

template<class T, size_t info_len = 2>
class MemoryRiver {
private:
  std::fstream file_;
  std::string file_name_;
  const size_t sizeofT_ = sizeof(T);
  size_t length_ = 0;
public:
  MemoryRiver() = default;

  MemoryRiver(const std::string& file_name_) : file_name_(file_name_) {}
  ~MemoryRiver() {
    if (!file_.is_open()) {
      file_.open(file_name_, std::ios::in | std::ios::out | std::ios::binary);
    }
    WriteInfo(length_, 0);
    file_.close();
  }
  size_t Length() {
    return length_;
  }
  void Initialise(std::string FN = "") {
    if (file_.is_open()) {
      file_.close();
    }
    if (FN != "") file_name_ = FN;
#ifdef FileClear
    file_.open(file_name_, std::ios::out);
    size_t tmp = 0;
    for (size_t i = 0; i < info_len; ++i) {
      file_.write(reinterpret_cast<char *>(&tmp), sizeof(size_t));
    }
    file_.close();
#else
    file_.open(file_name_, std::ios::in);
    if (!file_) {
      file_.open(file_name_, std::ios::out);
      size_t tmp = 0;
      for (size_t i = 0; i < info_len; ++i) {
        file_.write(reinterpret_cast<char *>(&tmp), sizeof(size_t));
      }
    } else {
      GetInfo(length_, 0);
    }
    file_.close();
#endif
  }

  void GetInfo(size_t &tmp, size_t n) {
    if (n >= info_len) return;
    if (!file_.is_open()) {
      file_.open(file_name_, std::ios::binary | std::ios::in | std::ios::out);
    }
    size_t t = n * sizeof(size_t);
    if (file_.tellg() != t) {
      file_.seekg(t);
    }
    file_.read(reinterpret_cast<char *>(&tmp), sizeof(size_t));
  }

  void WriteInfo(size_t tmp, size_t n) {
    if (n >= info_len) return;
    if (!file_.is_open()) {
      file_.open(file_name_, std::ios::binary | std::ios::in | std::ios::out);
    }
    size_t t = n * sizeof(size_t);
    if (file_.tellp() != t) {
      file_.seekp(t);
    }
    file_.write(reinterpret_cast<char *>(&tmp), sizeof(size_t));
  }

  size_t Write(T &t) {
    size_t pos = length_;
    if (!file_.is_open()) {
      file_.open(file_name_, std::ios::binary | std::ios::in | std::ios::out);
    }
    size_t tmp = length_ * sizeofT_ + info_len * sizeof(size_t);
    if (file_.tellp() != tmp) {
      file_.seekp(tmp);
    }
    file_.write(reinterpret_cast<char *>(&t), sizeofT_);
    ++length_;
    return pos;
  }

  size_t Reserve() {
    size_t pos = length_;
    ++length_;
    return pos;
  }

  void Update(T &t, const size_t index) {
    if (!file_.is_open()) {
      file_.open(file_name_, std::ios::binary | std::ios::in | std::ios::out);
    }
    size_t tmp = index * sizeofT_ + info_len * sizeof(size_t);
    if (file_.tellp() != tmp) {
      file_.seekp(tmp);
    }
    file_.write(reinterpret_cast<char *>(&t), sizeofT_);
  }

  void Read(T &t, const size_t index) {
    if (!file_.is_open()) {
      file_.open(file_name_, std::ios::binary | std::ios::in | std::ios::out);
    }
    size_t tmp = index * sizeofT_ + info_len * sizeof(size_t);
    if (file_.tellg() != tmp) {
      file_.seekg(tmp);
    }
    file_.read(reinterpret_cast<char *>(&t), sizeofT_);
  }

  std::vector<T> FindAll() {
    std::vector<T> res(length_);
    for (size_t i = 0; i < length_; ++i) {
      Read(res[i], i);
    }
    return res;
  }
};

#endif // MEMORYRIVER_HPP