#ifndef BOOK_H
#define BOOK_H

#include <array>
#include "unrolled_linked_list.hpp"

struct Book {
  int id_;
  std::array<char, 20> ISBN_;
  std::array<char, 60> book_name_;
  std::array<char, 60> author_;
  std::array<char, 60> keyword_;
  int stock_;
  double price_;
  Book() : price_(-1) {
    id_ = -1;
    std::fill(ISBN_.begin(), ISBN_.end(), 0);
    std::fill(book_name_.begin(), book_name_.end(), 0);
    std::fill(author_.begin(), author_.end(), 0);
    std::fill(keyword_.begin(), keyword_.end(), 0);
    stock_ = 0;
  }
  Book(const int id, const std::array<char, 20> ISBN) : id_(id), ISBN_(ISBN) {
    std::fill(book_name_.begin(), book_name_.end(), 0);
    std::fill(author_.begin(), author_.end(), 0);
    std::fill(keyword_.begin(), keyword_.end(), 0);
    stock_ = 0;
    price_ = 0;
  }
  void Print() const;
  friend bool operator < (const Book &, const Book &);
  friend bool operator == (const Book &, const Book &);
};

class BookSystem {
private:
  MemoryRiver<Book> books_id_;
  UnrolledLinkedList<std::array<char, 20>, int> books_ISBN_;
  UnrolledLinkedList<std::array<char, 60>, int> books_name_;
  UnrolledLinkedList<std::array<char, 60>, int> books_author_;
  UnrolledLinkedList<std::array<char, 60>, int> books_keyword_;
public:
  BookSystem() = delete;
  BookSystem(const std::string &name) : books_id_(name + "_id"), books_ISBN_(name + "_ISBN"), books_name_(name + "_name"), books_author_(name + "_author"), books_keyword_(name + "_keyword") {}
  Book QueryId(const int &);
  std::vector<Book> QueryISBN(const std::array<char, 20> &);
  std::vector<Book> QueryName(const std::array<char, 60> &);
  std::vector<Book> QueryAuthor(const std::array<char, 60> &);
  std::vector<Book> QueryKeyword(const std::array<char, 60> &);
  std::vector<Book> QueryAll();
  void Add(Book &);
  void ModifyISBN(const std::array<char, 20> &, const std::array<char, 20> &);
  void ModifyName(const std::array<char, 20> &, const std::array<char, 60> &);
  void ModifyAuthor(const std::array<char, 20> &, const std::array<char, 60> &);
  void ModifyKeyword(const std::array<char, 20> &, const std::array<char, 60> &);
  void ModifyStock(const std::array<char, 20> &, const int &);
  void ModifyPrice(const std::array<char, 20> &, const double &);
  size_t Size() const { return books_ISBN_.Size(); }
};

std::vector<std::array<char, 60>> GetKeywords(const std::array<char, 60> &);

#endif //BOOK_H
