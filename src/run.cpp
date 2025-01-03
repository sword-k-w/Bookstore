#include "run.h"

#include <iomanip>
#include <iostream>
#include <algorithm>

bool operator == (const std::basic_string<unsigned int> &a, const std::string &b) {
  if (a.size() != b.size()) {
    return false;
  }
  size_t size = a.size();
  for (size_t i = 0; i < size; ++i) {
    if (a[i] != b[i]) {
      return false;
    }
  }
  return true;
}

Run::Run() : account_system_("accounts"), book_system_("books"), log_system_("log") {
  time_ = log_system_.QueryTime();
  if (!time_) {
    account_system_.Add(Account(ToUserID({'r', 'o', 'o', 't'}), ToPassword({'s', 'j', 't', 'u'}), ToUsername({'s', 'w', 'o', 'r', 'd'}), 7));
  }
}

Run::~Run() {
  while (!online_.empty()) {
    Logout(true);
  }
  ++time_;
  log_system_.RecordTime(time_);
}

void Run::run() {
  while (true) {
    try {
      bool res = cur_command_.Read();
      std::basic_string<unsigned int> type = cur_command_.GetToken();
      if (type.empty()) {
        if (!res) {
          break;
        }
        continue;
      } else if (type == "quit" || type == "exit") {
        if (!cur_command_.GetToken().empty()) {
          std::cout << "Invalid\n";
          if (!res) {
            break;
          }
          continue;
        }
        ExitOperation exit_ope;
        exit_ope.time_ = ++time_;
        Operation ope{};
        ope.operation_type_ = Operation::kExit;
        ope.exit_operation_ = exit_ope;
        log_system_.RecordOperation(ope);
        break;
      } else if (type == "su") {
        Login();
      } else if (type == "logout") {
        Logout(false);
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
      } else if (type == "info") {
#ifdef front_end
        Info();
#else
        std::cout << "Invalid\n";
#endif
      } else {
        std::cout << "Invalid\n";
      }
      if (!res) {
        break;
      }
    } catch(Invalid x) {
      std::cout << x.what();
    }

  }
}

void Run::Login() {
  std::basic_string<unsigned int> tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    throw Invalid();
  }
  auto userID = ToUserID(tmp);
  if (userID[0] == '\n') {
    throw Invalid();
  }
  Account nxt = account_system_.Find(userID);

  if (!nxt.Privilege()) {
    throw Invalid();
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    if (nxt.Privilege() >= cur_account_.Privilege()) {
      throw Invalid();
    }
  } else {
    auto password = ToPassword(tmp);
    if (password[0] == '\n' || !nxt.CheckPassword(password)) {
      throw Invalid();
    }
  }
  if (!cur_command_.GetToken().empty()) {
    throw Invalid();
  }
  nxt.ModifyOnlineCount(1);
  account_system_.Modify(userID, nxt);
  online_.emplace(userID);
  select_.emplace(-1);
  cur_book_ = -1;
  cur_account_ = nxt;
#ifdef front_end
  std::cout << "Success\n";
#endif

  LoginOperation login_ope;
  login_ope.time_ = ++time_;
  login_ope.cur_account_ = cur_account_;
  Operation ope{};
  ope.operation_type_ = Operation::kLogin;
  ope.login_operation_ = login_ope;
  log_system_.RecordOperation(ope);
}

void Run::Logout(bool force) {
  if (online_.empty()) {
    throw Invalid();
  }
  if (!cur_command_.GetToken().empty() && !force) {
    throw Invalid();
  }

  LogoutOperation logout_ope;
  logout_ope.time_ = ++time_;
  logout_ope.cur_account_ = cur_account_;

  cur_account_.ModifyOnlineCount(-1);
  account_system_.Modify(cur_account_.UserID(), cur_account_);
  online_.pop();
  select_.pop();
  if (online_.empty()) {
    cur_account_ = Account();
    cur_book_ = -1;
  } else {
    cur_account_ = account_system_.Find(online_.top());
    cur_book_ = select_.top();
  }

  Operation ope{};
  ope.operation_type_ = Operation::kLogout;
  ope.logout_operation_ = logout_ope;
  log_system_.RecordOperation(ope);
}

void Run::Register() {
  std::basic_string<unsigned int> tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    throw Invalid();
  }
  auto userID = ToUserID(tmp);
  if (userID[0] == '\n' || account_system_.Find(userID).Privilege()) {
    throw Invalid();
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    throw Invalid();
  }
  auto password = ToPassword(tmp);
  if (password[0] == '\n') {
    throw Invalid();
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    throw Invalid();
  }
  auto username = ToUsername(tmp);
  if (username[0] == '\n') {
    throw Invalid();
  }
  if (!cur_command_.GetToken().empty()) {
    throw Invalid();
  }
  Account new_account(userID, password, username);
  account_system_.Add(new_account);
#ifdef front_end
  std::cout << "Success\n";
#endif

  RegisterOperation register_ope;
  register_ope.time_ = ++time_;
  register_ope.cur_account_ = new_account;
  Operation ope{};
  ope.operation_type_ = Operation::kRegister;
  ope.register_operation_ = register_ope;
  log_system_.RecordOperation(ope);
}

void Run::Password() {
  if (cur_account_.Privilege() < 1) {
    throw Invalid();
  }
  std::basic_string<unsigned int> tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    throw Invalid();
  }
  auto userID = ToUserID(tmp);
  if (userID[0] == '\n') {
    throw Invalid();
  }
  Account tmp_acc = account_system_.Find(userID);
  if (!tmp_acc.Privilege()) {
    throw Invalid();
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    throw Invalid();
  }
  auto password = ToPassword(tmp);
  if (password[0] == '\n') {
    throw Invalid();
  }
  tmp = cur_command_.GetToken();
  std::array<unsigned int, 30> new_password;
  if (tmp.empty()) {
    if (cur_account_.Privilege() != 7) {
      throw Invalid();
    }
    new_password = password;
  } else {
    std::array<unsigned int, 30> password_prime = ToPassword(tmp);
    if (password_prime[0] == '\n' || !tmp_acc.CheckPassword(password)) {
      throw Invalid();
    }
    if (!cur_command_.GetToken().empty()) {
      throw Invalid();
    }
    new_password = password_prime;
  }
  tmp_acc.UpdatePassword(new_password);
  if (userID == cur_account_.UserID()) {
    cur_account_.UpdatePassword(new_password);
  }
  account_system_.Modify(userID, tmp_acc);
#ifdef front_end
  std::cout << "Success\n";
#endif

  PasswordOperation password_ope;
  password_ope.time_ = ++time_;
  password_ope.cur_account_ = cur_account_;
  password_ope.new_password_ = new_password;
  password_ope.modified_ = tmp_acc;
  Operation ope{};
  ope.operation_type_ = Operation::kPassword;
  ope.password_operation_ = password_ope;
  log_system_.RecordOperation(ope);
}

void Run::Useradd() {
  if (cur_account_.Privilege() < 3) {
    throw Invalid();
  }
  std::basic_string<unsigned int> tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    throw Invalid();
  }
  auto userID = ToUserID(tmp);
  if (userID[0] == '\n' || account_system_.Find(userID).Privilege()) {
    throw Invalid();
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    throw Invalid();
  }
  auto password = ToPassword(tmp);
  if (password[0] == '\n') {
    throw Invalid();
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    throw Invalid();
  }
  auto privilege = ToPrivilege(tmp);
  if (!privilege || privilege >= cur_account_.Privilege()) {
    throw Invalid();
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    throw Invalid();
  }
  auto username = ToUsername(tmp);
  if (username[0] == '\n' || !cur_command_.GetToken().empty()) {
    throw Invalid();
  }
  Account new_account(userID, password, username, privilege);
  account_system_.Add(new_account);
#ifdef front_end
  std::cout << "Success\n";
#endif

  CreateOperation create_ope;
  create_ope.time_ = ++time_;
  create_ope.cur_account_ = cur_account_;
  create_ope.new_account_ = new_account;
  Operation ope{};
  ope.operation_type_ = Operation::kCreate;
  ope.create_operation_ = create_ope;
  log_system_.RecordOperation(ope);
}

void Run::Delete() {
  if (cur_account_.Privilege() != 7) {
    throw Invalid();
  }
  std::basic_string<unsigned int> tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    throw Invalid();
  }
  std::array<unsigned int, 30> userID = ToUserID(tmp);
  if (userID[0] == '\n' || !cur_command_.GetToken().empty()) {
    throw Invalid();
  }
  Account tmp_acc = account_system_.Find(userID);
  if (!tmp_acc.Privilege() || tmp_acc.OnlineCount()) {
    throw Invalid();
  }
  account_system_.Delete(tmp_acc);
#ifdef front_end
  std::cout << "Success\n";
#endif

  DeleteOperation delete_ope;
  delete_ope.time_ = ++time_;
  delete_ope.cur_account_ = cur_account_;
  delete_ope.deleted = tmp_acc;
  Operation ope{};
  ope.operation_type_ = Operation::kDelete;
  ope.delete_operation_ = delete_ope;
  log_system_.RecordOperation(ope);
}

void Run::Show() {
  if (cur_account_.Privilege() < 1) {
    throw Invalid();
  }
  std::basic_string<unsigned int> tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    // show all books
    auto books = book_system_.QueryAll();
    if (books.empty()) {
      std::cout << '\n';
    }
    std::sort(books.begin(), books.end(), [&](const Book &x, const Book &y) {
      return x.ISBN_ < y.ISBN_;
    });
    for (auto &book: books) {
      book.Print();
    }
  } else {
    if (tmp == "finance") {
      // finance log
      if (cur_account_.Privilege() < 7) {
        throw Invalid();
      }
      tmp = cur_command_.GetToken();
      if (tmp.empty()) {
        log_system_.QueryTrade();
      } else {
        auto count = ToCount(tmp);
        if (count < 0 || !cur_command_.GetToken().empty()) {
          throw Invalid();
        }
        log_system_.QueryTrade(count);
      }
      return;
    } else {
      auto ISBN = ToISBN_(tmp);
      if (ISBN[0] == '\n') {
        auto book_name = ToBookName_(tmp);
        if (book_name[0] == '\n') {
          auto author = ToAuthor_(tmp);
          if (author[0] == '\n') {
            auto keyword = ToKeyword_(tmp);
            if (keyword[0] == '\n') {
              throw Invalid();
            }
            if (!cur_command_.GetToken().empty()) {
              throw Invalid();
            }
            auto books = book_system_.QueryKeyword(keyword);
            std::sort(books.begin(), books.end(), [&](const Book &x, const Book &y) {
              return x.ISBN_ < y.ISBN_;
            });
            if (books.empty()) {
              std::cout << '\n';
            }
            for (auto &book: books) {
              book.Print();
            }
          } else {
            if (!cur_command_.GetToken().empty()) {
              throw Invalid();
            }
            auto books = book_system_.QueryAuthor(author);
            std::sort(books.begin(), books.end(), [&](const Book &x, const Book &y) {
              return x.ISBN_ < y.ISBN_;
            });
            if (books.empty()) {
              std::cout << '\n';
            }
            for (auto &book: books) {
              book.Print();
            }
          }
        } else {
          if (!cur_command_.GetToken().empty()) {
            throw Invalid();
          }
          auto books = book_system_.QueryName(book_name);
          std::sort(books.begin(), books.end(), [&](const Book &x, const Book &y) {
            return x.ISBN_ < y.ISBN_;
          });
          if (books.empty()) {
            std::cout << '\n';
          }
          for (auto &book: books) {
            book.Print();
          }
        }
      } else {
        if (!cur_command_.GetToken().empty()) {
          throw Invalid();
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
  SearchOperation search_ope;
  search_ope.time_ = ++time_;
  search_ope.cur_account_ = cur_account_;
  Operation ope{};
  ope.operation_type_ = Operation::kSearch;
  ope.search_operation_ = search_ope;
  log_system_.RecordOperation(ope);
}

void Run::Buy() {
  if (cur_account_.Privilege() < 1) {
    throw Invalid();
  }
  std::basic_string<unsigned int> tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    throw Invalid();
  }
  auto ISBN = ToISBN(tmp);
  if (ISBN[0] == '\n') {
    throw Invalid();
  }
  tmp = cur_command_.GetToken();
  auto quantity = ToQuantity(tmp);
  if (quantity <= 0 || !cur_command_.GetToken().empty()) {
    throw Invalid();
  }
  auto book = book_system_.QueryISBN(ISBN)[0];
  if (book.stock_ < quantity) {
    throw Invalid();
  }
  book_system_.ModifyStock(ISBN, book.stock_ - quantity);
  log_system_.RecordTrade(quantity * book.price_);
  std::cout << std::fixed << std::setprecision(2) << quantity * book.price_ << '\n';

  BuyOperation buy_ope;
  buy_ope.time_ = ++time_;
  buy_ope.cur_account_ = cur_account_;
  buy_ope.book_ = book;
  buy_ope.quantity_ = quantity;
  Operation ope{};
  ope.operation_type_ = Operation::kBuy;
  ope.buy_operation_ = buy_ope;
  log_system_.RecordOperation(ope);
}

void Run::Select() {
  if (cur_account_.Privilege() < 3) {
    throw Invalid();
  }
  std::basic_string<unsigned int> tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    throw Invalid();
  }
  auto ISBN = ToISBN(tmp);
  if (ISBN[0] == '\n' || !cur_command_.GetToken().empty()) {
    throw Invalid();
  }
  auto book = book_system_.QueryISBN(ISBN)[0];
  if (book.price_ < 0) {
    book = Book(book_system_.Size(), ISBN);
    book_system_.Add(book);
  }
#ifdef front_end
  std::cout << "Success\n";
#endif
  cur_book_ = book.id_;
  select_.pop();
  select_.emplace(book.id_);
  account_system_.Modify(cur_account_.UserID(), cur_account_);
}

void Run::Modify() {
  Book cur_book = book_system_.QueryId(cur_book_);

  if (cur_account_.Privilege() < 3 || cur_book.price_ < 0) {
    throw Invalid();
  }
  std::basic_string<unsigned int> tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    throw Invalid();
  }
  std::array<unsigned int, 20> new_ISBN = {'\n'};
  std::array<unsigned int, 60> new_name = {'\n'};
  std::array<unsigned int, 60> new_author = {'\n'};
  std::array<unsigned int, 60> new_keyword = {'\n'};
  double new_price = -1;
  while (!tmp.empty()) {
    auto ISBN = ToISBN_(tmp);
    if (ISBN[0] != '\n') {
      if (new_ISBN[0] != '\n') {
        throw Invalid();
      }
      new_ISBN = ISBN;
    } else {
      auto book_name = ToBookName_(tmp);
      if (book_name[0] != '\n') {
        if (new_name[0] != '\n') {
          throw Invalid();
        }
        new_name = book_name;
      } else {
        auto author = ToAuthor_(tmp);
        if (author[0] != '\n') {
          if (new_author[0] != '\n') {
            throw Invalid();
          }
          new_author = author;
        } else {
          auto keyword = ToKeywords_(tmp);
          if (keyword[0] != '\n') {
            if (new_keyword[0] != '\n') {
              throw Invalid();
            }
            new_keyword = keyword;
          } else {
            auto price = ToPrice_(tmp);
            if (price >= 0) {
              if (new_price >= 0) {
                throw Invalid();
              }
              new_price = price;
            } else {
              throw Invalid();
            }
          }
        }
      }
    }
    tmp = cur_command_.GetToken();
  }
  if (book_system_.QueryISBN(new_ISBN)[0].price_ >= 0) {
    throw Invalid();
  }
  Book tmp_book = cur_book;
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
  if (new_ISBN[0] != '\n') {
    book_system_.ModifyISBN(cur_book.ISBN_, new_ISBN);
    cur_book.ISBN_ = new_ISBN;
  }

#ifdef front_end
  std::cout << "Success\n";
#endif

  ModifyOperation modify_ope;
  modify_ope.time_ = ++time_;
  modify_ope.cur_account_ = cur_account_;
  modify_ope.book_ = tmp_book;
  modify_ope.new_book_ = cur_book;
  Operation ope{};
  ope.operation_type_ = Operation::kModify;
  ope.modify_operation_ = modify_ope;
  log_system_.RecordOperation(ope);
}

void Run::Import() {
  Book cur_book = book_system_.QueryId(cur_book_);
  if (cur_account_.Privilege() < 3 || cur_book.price_ < 0) {
    throw Invalid();
  }
  std::basic_string<unsigned int> tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    throw Invalid();
  }
  auto quantity = ToQuantity(tmp);
  if (quantity <= 0) {
    throw Invalid();
  }
  tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    throw Invalid();
  }
  auto total_cost = ToTotalCost(tmp);
  if (total_cost <= 0 || !cur_command_.GetToken().empty()) {
    throw Invalid();
  }
  book_system_.ModifyStock(cur_book.ISBN_, cur_book.stock_ + quantity);
  log_system_.RecordTrade(-total_cost);

  ImportOperation import_ope;
  import_ope.time_ = ++time_;
  import_ope.cur_account_ = cur_account_;
  import_ope.book_ = cur_book;
  import_ope.quantity_ = quantity;
  import_ope.cost_ = total_cost;
  Operation ope{};
  ope.operation_type_ = Operation::kImport;
  ope.import_operation_ = import_ope;
  log_system_.RecordOperation(ope);
}

void Run::Log() {
  if (cur_account_.Privilege() < 7) {
    throw Invalid();
  }
  if (!cur_command_.GetToken().empty()) {
    throw Invalid();
  }
  log_system_.ReportLog();
}

void Run::Report() {
  if (cur_account_.Privilege() < 7) {
    throw Invalid();
  }
  std::basic_string<unsigned int> tmp = cur_command_.GetToken();
  if (tmp.empty()) {
    throw Invalid();
  }
  if (tmp == "finance") {
    if (!cur_command_.GetToken().empty()) {
      throw Invalid();
    }
    log_system_.ReportFinance();
  } else if (tmp == "employee") {
    if (!cur_command_.GetToken().empty()) {
      throw Invalid();
    }
    log_system_.ReportEmployee();
  } else {
    throw Invalid();
  }
}

void Run::Info() {
  if (!cur_command_.GetToken().empty()) {
    throw Invalid();
  }
  std::cout << static_cast<int>(cur_account_.Privilege()) << " ";
  if (cur_account_.Privilege() == 0) {
    std::cout << "visitor visitor\n";
    return;
  }
  std::cout << cur_account_.UserID() << " " << cur_account_.Username() << '\n';
}