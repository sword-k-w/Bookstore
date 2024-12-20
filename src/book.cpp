#include "book.h"

#include <command.h>
#include <iomanip>

bool operator < (const Book &x, const Book &y) {
  if (x.id_ != y.id_) {
    if (x.ISBN_ == y.ISBN_) {
      if (x.book_name_ == y.book_name_) {
        if (x.author_ == y.author_) {
          if (x.keyword_ == y.keyword_) {
            if (x.price_ == y.price_) {
              return x.stock_ < y.stock_;
            }
            return x.price_ < y.price_;
          }
          return x.keyword_ < y.keyword_;
        }
        return x.author_ < y.author_;
      }
      return x.book_name_ < y.book_name_;
    }
    return x.ISBN_ < y.ISBN_;
  }
  return x.id_ < y.id_;
}

bool operator == (const Book &x, const Book &y) {
  return x.id_ == y.id_ && x.ISBN_ == y.ISBN_ && x.book_name_ == y.book_name_ && x.author_ == y.author_ && x.keyword_ == y.keyword_ && x.price_ == y.price_ && x.stock_ == y.stock_;
}

std::ostream &operator << (std::ostream &os, const std::array<char, 20> &x) {
  for (size_t i = 0; i < 20; ++i) {
    if (x[i] != '\0') {
      os << x[i];
    }
  }
  return os;
}

std::ostream &operator << (std::ostream &os, const std::array<char, 60> &x) {
  for (size_t i = 0; i < 60; ++i) {
    if (x[i] != '\0') {
      os << x[i];
    }
  }
  return os;
}

void Book::Print() const {
  std::cout << ISBN_ << '\t' << book_name_ << '\t' << author_ << '\t' << keyword_ << '\t';
  std::cout << std::fixed << std::setprecision(2) << price_ << '\t';
  std::cout << stock_ << '\n';
}

std::vector<std::array<char, 60>> GetKeywords(const std::array<char, 60> &keyword) {
  std::vector<std::array<char, 60>> res;
  size_t las = 0;
  for (size_t i = 1; i < 60; ++i) {
    std::array<char, 60> tmp;
    std::fill(tmp.begin(), tmp.end(), 0);
    if (keyword[i] == '|') {
      for (size_t j = las; j < i; ++j) {
        tmp[j - las] = keyword[j];
      }
      res.emplace_back(tmp);
      las = i + 1;
    } else if (keyword[i] == '\0') {
      for (size_t j = las; j < i; ++j) {
        tmp[j - las] = keyword[j];
      }
      res.emplace_back(tmp);
      break;
    } else if (i == 59) {
      for (size_t j = las; j <= i; ++j) {
        tmp[j - las] = keyword[j];
      }
      res.emplace_back(tmp);
    }
  }
  return res;
}

void BookSystem::Add(const Book &book) {
  books_id_.Insert(book.id_, book);
  books_ISBN_.Insert(book.ISBN_, book.id_);
  books_name_.Insert(book.book_name_, book.id_);
  books_author_.Insert(book.author_, book.id_);
  auto keywords = GetKeywords(book.keyword_);
  for (auto &keyword : keywords) {
    books_keyword_.Insert(keyword, book.id_);
  }
}

void BookSystem::ModifyISBN(const std::array<char, 20> &ISBN, const std::array<char, 20> &new_ISBN) {
  Book book = books_id_.Find(books_ISBN_.Find(ISBN)[0])[0];
  books_id_.Delete(book.id_, book);
  books_ISBN_.Delete(ISBN, book.id_);
  book.ISBN_ = new_ISBN;
  books_id_.Insert(book.id_, book);
  books_ISBN_.Insert(new_ISBN, book.id_);
}

void BookSystem::ModifyName(const std::array<char, 20> &ISBN, const std::array<char, 60> &new_name) {
  Book book = books_id_.Find(books_ISBN_.Find(ISBN)[0])[0];
  books_id_.Delete(book.id_, book);
  books_name_.Delete(book.book_name_, book.id_);
  book.book_name_ = new_name;
  books_id_.Insert(book.id_, book);
  books_name_.Insert(new_name, book.id_);
}

void BookSystem::ModifyAuthor(const std::array<char, 20> &ISBN, const std::array<char, 60> &new_author) {
  Book book = books_id_.Find(books_ISBN_.Find(ISBN)[0])[0];
  books_id_.Delete(book.id_, book);
  books_author_.Delete(book.author_, book.id_);
  book.author_ = new_author;
  books_id_.Insert(book.id_, book);
  books_author_.Insert(new_author, book.id_);
}

void BookSystem::ModifyKeyword(const std::array<char, 20> &ISBN, const std::array<char, 60> &new_keyword) {
  Book book = books_id_.Find(books_ISBN_.Find(ISBN)[0])[0];

  books_id_.Delete(book.id_, book);
  auto keywords = GetKeywords(book.keyword_);
  for (auto &keyword : keywords) {
    books_keyword_.Delete(keyword, book.id_);
  }

  book.keyword_ = new_keyword;

  books_id_.Insert(book.id_, book);
  auto new_keywords = GetKeywords(new_keyword);
  for (auto &keyword : new_keywords) {
    books_keyword_.Insert(keyword, book.id_);
  }
}

void BookSystem::ModifyStock(const std::array<char, 20> &ISBN, const int &new_stock) {
  Book book = books_id_.Find(books_ISBN_.Find(ISBN)[0])[0];
  books_id_.Delete(book.id_, book);
  book.stock_ = new_stock;
  books_id_.Insert(book.id_, book);
}

void BookSystem::ModifyPrice(const std::array<char, 20> &ISBN, const double &new_price) {
  Book book = books_id_.Find(books_ISBN_.Find(ISBN)[0])[0];
  books_id_.Delete(book.id_, book);
  book.price_ = new_price;
  books_id_.Insert(book.id_, book);
}

Book BookSystem::QueryId(const int &id) {
 std::vector<Book> books = books_id_.Find(id);
  if (books.empty()) {
    return Book();
  } else {
    return books[0];
  }
}

std::vector<Book> BookSystem::QueryISBN(const std::array<char, 20> &ISBN) {
  std::vector<int> ids = books_ISBN_.Find(ISBN);
  if (ids.empty()) {
    return {Book()};
  } else {
    return books_id_.Find(ids[0]);
  }
}

std::vector<Book> BookSystem::QueryName(const std::array<char, 60> &name) {
  std::vector<int> ids = books_name_.Find(name);
  size_t size = ids.size();
  std::vector<Book> books(size);
  for (size_t i = 0; i < size; ++i) {
    books[i] = books_id_.Find(ids[i])[0];
  }
  return books;
}

std::vector<Book> BookSystem::QueryAuthor(const std::array<char, 60> &author) {
  std::vector<int> ids = books_author_.Find(author);
  size_t size = ids.size();
  std::vector<Book> books(size);
  for (size_t i = 0; i < size; ++i) {
    books[i] = books_id_.Find(ids[i])[0];
  }
  return books;
}

std::vector<Book> BookSystem::QueryKeyword(const std::array<char, 60> &keyword) {
  std::vector<int> ids = books_keyword_.Find(keyword);
  size_t size = ids.size();
  std::vector<Book> books(size);
  for (size_t i = 0; i < size; ++i) {
    books[i] = books_id_.Find(ids[i])[0];
  }
  return books;
}

std::vector<Book> BookSystem::QueryAll() {
  return books_id_.FindAll();
}
