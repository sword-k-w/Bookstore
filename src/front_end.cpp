#include "front_end.h"

#include <iomanip>
#include <iostream>
#include <algorithm>

FrontEnd::FrontEnd() : account_system_("accounts"), book_system_("books"), log_system_("finance") {
  time_ = log_system_.QueryTime();
  if (!time_) {
    account_system_.Add(Account(ToUserID("root"), ToPassword("sjtu"), ToUsername("sword"), 7));
  }
}

FrontEnd::~FrontEnd() {
  ++time_;
  log_system_.RecordTime(time_);
}

void FrontEnd::run() {
  while (true) {
    if (!cur_command_.Read()) {
      break;
    }
    std::string type = cur_command_.GetToken();
    if (type.empty()) {
      continue;
    } else if (type == "quit" || type == "exit") {
      break;
    } else if (type == "su") {
      Login();
    } else if (type == "logout") {
      Logout();
    } else if (type == "register") {
      Register();
    } else if (type == "passwd") {
      Password();
    } else if (type == "useradd") {
      Useradd();
    } else if (type == "delete") {
      Delete();
    } else if (type == "show") {
      Show();
    } else if (type == "buy") {
      Buy();
    } else if (type == "select") {
      Select();
    } else if (type == "modify") {
      Modify();
    } else if (type == "import") {
      Import();
    } else if (type == "log") {
      Log();
    } else if (type == "report") {
      Report();
    } else {
      std::cout << "Invalid\n";
    }
  }
}

void FrontEnd::Login() {
  std::cerr << "Login\n";
  std::string tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  auto userID = ToUserID(tmp);
  if (userID[0] == '\n') {
    std::cout << "Invalid\n";
    return;
  }
  Account nxt = account_system_.Find(userID);

  if (!nxt.Privilege()) {
    std::cerr << "????\n";
    std::cout << "Invalid\n";
    return;
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    if (nxt.Privilege() >= cur_account_.Privilege()) {
      std::cout << "Invalid\n";
      return;
    }
  } else {
    auto password = ToPassword(tmp);
    if (password[0] == '\n' || !nxt.CheckPassword(password)) {
      std::cout << "Invalid\n";
      return;
    }
  }
  if (!cur_command_.GetToken().empty()) {
    std::cout << "Invalid\n";
    return;
  }
  nxt.ModifyOnlineCount(1);
  account_system_.Modify(userID, nxt);
  std::cerr << nxt.OnlineCount() << '\n';
  online_.emplace(userID);
  cur_account_ = nxt;
}

void FrontEnd::Logout() {
  std::cerr << "Logout\n";
  if (online_.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  if (!cur_command_.GetToken().empty()) {
    std::cout << "Invalid\n";
    return;
  }
  cur_account_.ModifyOnlineCount(-1);

  account_system_.Modify(cur_account_.UserID(), cur_account_);
  online_.pop();

  if (online_.empty()) {
    cur_account_ = Account();
  } else {
    cur_account_ = account_system_.Find(online_.top());
    std::cerr << cur_account_.OnlineCount() << '\n';
  }
}

void FrontEnd::Register() {
  std::string tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  auto userID = ToUserID(tmp);
  if (userID[0] == '\n' || account_system_.Find(userID).Privilege()) {
    std::cout << "Invalid\n";
    return;
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  auto password = ToPassword(tmp);
  if (password[0] == '\n') {
    std::cout << "Invalid\n";
    return;
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  auto username = ToUsername(tmp);
  if (username[0] == '\n') {
    std::cout << "Invalid\n";
    return;
  }
  if (!cur_command_.GetToken().empty()) {
    std::cout << "Invalid\n";
    return;
  }
  account_system_.Add(Account(userID, password, username));
}

void FrontEnd::Password() {
  if (cur_account_.Privilege() < 1) {
    std::cout << "Invalid\n";
    return;
  }
  std::string tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  auto userID = ToUserID(tmp);
  if (userID[0] == '\n') {
    std::cout << "Invalid\n";
    return;
  }
  Account tmp_acc = account_system_.Find(userID);
  if (!tmp_acc.Privilege()) {
    std::cout << "Invalid\n";
    return;
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  auto password = ToPassword(tmp);
  if (password[0] == '\n') {
    std::cout << "Invalid\n";
    return;
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    if (cur_account_.Privilege() != 7) {
      std::cout << "Invalid\n";
      return;
    }
    tmp_acc.UpdatePassword(password);
    if (userID == cur_account_.UserID()) {
      cur_account_.UpdatePassword(password);
    }
  } else {
    std::array<char, 30> password_prime = ToPassword(tmp);
    if (password_prime[0] == '\n' || !tmp_acc.CheckPassword(password)) {
      std::cout << "Invalid\n";
      return;
    }
    if (!cur_command_.GetToken().empty()) {
      std::cout << "Invalid\n";
      return;
    }
    tmp_acc.UpdatePassword(password_prime);
    if (userID == cur_account_.UserID()) {
      cur_account_.UpdatePassword(password);
    }
  }
  account_system_.Modify(userID, tmp_acc);
}

void FrontEnd::Useradd() {
  if (cur_account_.Privilege() < 3) {
    std::cout << "Invalid\n";
    return;
  }
  std::string tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  auto userID = ToUserID(tmp);
  if (userID[0] == '\n' || account_system_.Find(userID).Privilege()) {
    std::cout << "Invalid\n";
    return;
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  auto password = ToPassword(tmp);
  if (password[0] == '\n') {
    std::cout << "Invalid\n";
    return;
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  auto privilege = ToPrivilege(tmp);
  if (!privilege || privilege >= cur_account_.Privilege()) {
    std::cout << "Invalid\n";
    return;
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  auto username = ToUsername(tmp);
  if (username[0] == '\n') {
    std::cout << "Invalid\n";
    return;
  }
  if (!cur_command_.GetToken().empty()) {
    std::cout << "Invalid\n";
    return;
  }
  account_system_.Add(Account(userID, password, username, privilege));
}

void FrontEnd::Delete() {
  if (cur_account_.Privilege() != 7) {
    std::cout << "Invalid\n";
    return;
  }
  std::string tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  std::array<char, 30> userID = ToUserID(tmp);
  if (userID[0] == '\n') {
    std::cout << "Invalid\n";
    return;
  }
  if (!cur_command_.GetToken().empty()) {
    std::cout << "Invalid\n";
    return;
  }
  Account tmp_acc = account_system_.Find(userID);
  if (!tmp_acc.Privilege() || tmp_acc.OnlineCount()) {
    std::cout << "Invalid\n";
    return;
  }
  account_system_.Delete(tmp_acc);
}

void FrontEnd::Show() {
  if (cur_account_.Privilege() < 1) {
    std::cout << "Invalid\n";
    return;
  }
  std::string tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    // show all books
    auto books = book_system_.QueryAll();
    if (books.empty()) {
      std::cout << '\n';
    }
    std::sort(books.begin(), books.end(), [&] (const Book &x, const Book &y) {
      return x.ISBN_ < y.ISBN_;
    });
    for (auto &book : books) {
      book.Print();
    }
  } else {
    if (tmp == "finance") {
      // finance log
      if (cur_account_.Privilege() < 7) {
        std::cout << "Invalid\n";
        return;
      }
      tmp = cur_command_.GetToken();
      if (tmp.empty()) {
        log_system_.QueryTrade();
      } else {
        auto count = ToCount(tmp);
        if (count < 0 || !cur_command_.GetToken().empty()) {
          std::cout << "Invalid\n";
          return;
        }
        log_system_.QueryTrade(count);
      }
    } else {
      auto ISBN = ToISBN_(tmp);
      if (ISBN[0] == '\n') {
        auto book_name = ToBookName_(tmp);
        if (book_name[0] == '\n') {
          auto author = ToAuthor_(tmp);
          if (author[0] == '\n') {
            auto keyword = ToKeyword_(tmp);
            if (keyword[0] == '\n') {
              std::cout << "Invalid\n";
              return;
            }
            if (!cur_command_.GetToken().empty()) {
              std::cout << "Invalid\n";
              return;
            }
            auto books = book_system_.QueryKeyword(keyword);
            std::sort(books.begin(), books.end(), [&] (const Book &x, const Book &y) {
              return x.ISBN_ < y.ISBN_;
            });
            if (books.empty()) {
              std::cout << '\n';
            }
            for (auto &book : books) {
              book.Print();
            }
          } else {
            if (!cur_command_.GetToken().empty()) {
              std::cout << "Invalid\n";
              return;
            }
            auto books = book_system_.QueryAuthor(author);
            std::sort(books.begin(), books.end(), [&] (const Book &x, const Book &y) {
              return x.ISBN_ < y.ISBN_;
            });
            if (books.empty()) {
              std::cout << '\n';
            }
            for (auto &book : books) {
              book.Print();
            }
          }
        } else {
          if (!cur_command_.GetToken().empty()) {
            std::cout << "Invalid\n";
            return;
          }
          auto books = book_system_.QueryName(book_name);
          std::sort(books.begin(), books.end(), [&] (const Book &x, const Book &y) {
            return x.ISBN_ < y.ISBN_;
          });
          if (books.empty()) {
            std::cout << '\n';
          }
          for (auto &book : books) {
            book.Print();
          }
        }
      } else {
        if (!cur_command_.GetToken().empty()) {
          std::cout << "Invalid\n";
          return;
        }
        Book book = book_system_.QueryISBN(ISBN)[0];
        if (book.price_ < 0) {
          std::cout << '\n';
        } else {
          book.Print();
        }
      }
    }
  }
}

void FrontEnd::Buy() {
  if (cur_account_.Privilege() < 1) {
    std::cout << "Invalid\n";
    return;
  }
  std::string tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  auto ISBN = ToISBN(tmp);
  if (ISBN[0] == '\n') {
    std::cout << "Invalid\n";
    return;
  }
  tmp = cur_command_.GetToken();
  auto quantity = ToQuantity(tmp);
  if (quantity <= 0 || !cur_command_.GetToken().empty()) {
    std::cout << "Invalid\n";
    return;
  }
  auto book = book_system_.QueryISBN(ISBN)[0];
  if (book.stock_ < quantity) {
    std::cout << "Invalid\n";
    return;
  }
  book_system_.ModifyStock(ISBN, book.stock_ - quantity);
  log_system_.RecordTrade(quantity * book.price_);
  std::cout << std::fixed << std::setprecision(2) << quantity * book.price_ << '\n';
}

void FrontEnd::Select() {
  if (cur_account_.Privilege() < 3) {
    std::cout << "Invalid\n";
    return;
  }
  std::string tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  auto ISBN = ToISBN(tmp);
  if (ISBN[0] == '\n') {
    std::cout << "Invalid\n";
    return;
  }
  auto book = book_system_.QueryISBN(ISBN)[0];
  if (book.price_ < 0) {
    book = Book(book_system_.Size() + 1, ISBN);
    book_system_.Add(book);
  }
  cur_account_.cur_book_ = book.id_;
  account_system_.Modify(cur_account_.UserID(), cur_account_);
}

void FrontEnd::Modify() {
  Book cur_book = book_system_.QueryId(cur_account_.cur_book_);

  if (cur_account_.Privilege() < 3 || cur_book.price_ < 0) {
    std::cout << "Invalid\n";
    return;
  }
  std::string tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  std::array<char, 20> new_ISBN = {'\n'};
  std::array<char, 60> new_name = {'\n'};
  std::array<char, 60> new_author = {'\n'};
  std::array<char, 60> new_keyword = {'\n'};
  double new_price = -1;
  while (!tmp.empty()) {
    auto ISBN = ToISBN_(tmp);
    if (ISBN[0] != '\n') {
      if (new_ISBN[0] != '\n') {
        std::cout << "Invalid\n";
        return;
      }
      new_ISBN = ISBN;
    } else {
      auto book_name = ToBookName_(tmp);
      if (book_name[0] != '\n') {
        if (new_name[0] != '\n') {
          std::cout << "Invalid\n";
          return;
        }
        new_name = book_name;
      } else {
        auto author = ToAuthor_(tmp);
        if (author[0] != '\n') {
          if (new_author[0] != '\n') {
            std::cout << "Invalid\n";
            return;
          }
          new_author = author;
        } else {
          auto keyword = ToKeywords_(tmp);
          if (keyword[0] != '\n') {
            if (new_keyword[0] != '\n') {
              std::cout << "Invalid\n";
              return;
            }
            new_keyword = keyword;
          } else {
            auto price = ToPrice_(tmp);
            if (price >= 0) {
              if (new_price >= 0) {
                std::cout << "Invalid\n";
                return;
              }
              new_price = price;
            } else {
              std::cout << "Invalid\n";
              return;
            }
          }
        }
      }
    }
    tmp = cur_command_.GetToken();
  }
  if (book_system_.QueryISBN(new_ISBN)[0].price_ >= 0) {
    std::cout << "Invalid\n";
    return;
  }
  if (new_ISBN[0] != '\n') {
    book_system_.ModifyISBN(cur_book.ISBN_, new_ISBN);
    cur_book.ISBN_ = new_ISBN;
  }
  if (new_name[0] != '\n') {
    book_system_.ModifyName(cur_book.ISBN_, new_name);
    cur_book.book_name_ = new_name;
  }
  if (new_author[0] != '\n') {
    book_system_.ModifyAuthor(cur_book.ISBN_, new_author);
    cur_book.author_ = new_author;
  }
  if (new_keyword[0] != '\n') {
    book_system_.ModifyKeyword(cur_book.ISBN_, new_keyword);
    cur_book.keyword_ = new_keyword;
  }
  if (new_price >= 0) {
    book_system_.ModifyPrice(cur_book.ISBN_, new_price);
    cur_book.price_ = new_price;
  }
}

void FrontEnd::Import() {
  Book cur_book = book_system_.QueryId(cur_account_.cur_book_);
  if (cur_account_.Privilege() < 3 || cur_book.price_ < 0) {
    std::cout << "Invalid\n";
    return;
  }
  std::string tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  auto quantity = ToQuantity(tmp);
  if (quantity < 0) {
    std::cout << "Invalid\n";
    return;
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  auto total_cost = ToTotalCost(tmp);
  if (total_cost < 0 || !cur_command_.GetToken().empty()) {
    std::cout << "Invalid\n";
    return;
  }
  book_system_.ModifyStock(cur_book.ISBN_, cur_book.stock_ + quantity);
  log_system_.RecordTrade(-total_cost);
}

void FrontEnd::Log() {

}

void FrontEnd::Report() {

}
