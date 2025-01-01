let account = document.getElementById("account");
let privilege;
let userID;

const button_account = document.getElementById("button_account");
const button_book = document.getElementById("button_book");
const button_log = document.getElementById("button_log");
const button_register = document.getElementById("button_register");
const button_login = document.getElementById("button_login");
const button_logout = document.getElementById("button_logout");
const button_password = document.getElementById("button_password");
const button_useradd = document.getElementById("button_useradd");
const button_delete = document.getElementById("button_delete");
const button_addbook = document.getElementById("button_addbook");
const button_modifybook = document.getElementById("button_modifybook");
const button_querybook = document.getElementById("button_querybook");
const button_queryall = document.getElementById("button_queryall");
const button_ISBN = document.getElementById("button_ISBN");
const button_bookname = document.getElementById("button_bookname");
const button_author = document.getElementById("button_author");
const button_keyword = document.getElementById("button_keyword");
const div_login = document.getElementById("div_login");
const div_login_error = document.getElementById("div_login_error");
const div_register = document.getElementById("div_register");
const div_register_password_error = document.getElementById("div_register_password_error");
const div_register_other_error = document.getElementById("div_register_other_error");
const div_password = document.getElementById("div_password");
const div_password_ensure_error = document.getElementById("div_password_ensure_error");
const div_password_other_error = document.getElementById("div_password_other_error");
const div_useradd = document.getElementById("div_useradd");
const div_useradd_error = document.getElementById("div_useradd_error");
const div_delete = document.getElementById("div_delete");
const div_delete_error = document.getElementById("div_delete_error");
const div_addbook = document.getElementById("div_addbook");
const div_addbook_error = document.getElementById("div_addbook_error");
const div_modifybook = document.getElementById("div_modifybook");
const div_modifybook_error = document.getElementById("div_modifybook_error");
const div_querybook = document.getElementById("div_querybook");
const div_querybook_error = document.getElementById("div_querybook_error");
const form_querybook_ISBN = document.getElementById("form_querybook_ISBN");
const form_querybook_bookname = document.getElementById("form_querybook_bookname");
const form_querybook_author = document.getElementById("form_querybook_author");
const form_querybook_keyword = document.getElementById("form_querybook_keyword");
const book_table = document.getElementById("book_table");

function CheckDisplay() {
  if (button_login.classList.contains("hidden")) {
    div_login.classList.add("hidden");
  }
  if (button_register.classList.contains("hidden")) {
    div_register.classList.add("hidden");
  }
  if (button_password.classList.contains("hidden")) {
    div_password.classList.add("hidden");
  }
  if (button_useradd.classList.contains("hidden")) {
    div_useradd.classList.add("hidden");
  }
  if (button_delete.classList.contains("hidden")) {
    div_delete.classList.add("hidden");
  }
  if (button_addbook.classList.contains("hidden")) {
    div_addbook.classList.add("hidden");
  }
  if (button_modifybook.classList.contains("hidden")) {
    div_modifybook.classList.add("hidden");
  }
  if (button_querybook.classList.contains("hidden")) {
    div_querybook.classList.add("hidden");
  }
}

function RemoveAccount() {
  button_login.classList.add("hidden");
  button_logout.classList.add("hidden");
  button_register.classList.add("hidden");
  button_password.classList.add("hidden");
  button_useradd.classList.add("hidden");
  button_delete.classList.add("hidden");
  CheckDisplay();
}

function RemoveBook() {
  button_addbook.classList.add("hidden");
  button_modifybook.classList.add("hidden");
  button_querybook.classList.add("hidden");
  CheckDisplay();
}

fetch('http://localhost:5000/GetUser')
  .then(response => response.json())
  .then(data => {
    account.textContent = data.user_name;
    privilege = data.privilege;
    userID = data.userID;
    if (privilege === 7) {
      button_log.classList.remove("hidden");
      document.getElementById("password_password_text").classList.add("hidden");
    }
    if (privilege === 3) {
      document.getElementById("useradd_privilege_text").classList.add("hidden");
    }
    if (privilege !== 0) {
      button_book.classList.remove("hidden");
    }
  });

button_account.addEventListener("click", () => {
  if (privilege === 0) {
    button_login.classList.toggle("hidden");
    button_register.classList.toggle("hidden");
  }
  if (privilege !== 0) {
    button_logout.classList.toggle("hidden");
    button_password.classList.toggle("hidden");
  }
  if (privilege === 7) {
    button_delete.classList.toggle("hidden");
  }
  if (privilege >= 3) {
    button_useradd.classList.toggle("hidden");
  }
  RemoveBook();
});

button_login.addEventListener("click", () => {
  div_login.classList.remove("hidden");
  div_register.classList.add("hidden");
});

button_register.addEventListener("click", () => {
  div_register.classList.remove("hidden");
  div_login.classList.add("hidden");
});

button_logout.addEventListener("click", () => {
  fetch('http://localhost:5000/Logout')
    .then(response => {
      alert("注销成功！");
      location.reload();
    });
});

button_password.addEventListener("click", () => {
  div_password.classList.remove("hidden");
  div_useradd.classList.add("hidden");
  div_delete.classList.add("hidden");
});

button_useradd.addEventListener("click", () => {
  div_useradd.classList.remove("hidden");
  div_password.classList.add("hidden");
  div_delete.classList.add("hidden");
});

button_delete.addEventListener("click", () => {
  div_delete.classList.remove("hidden");
  div_password.classList.add("hidden");
  div_useradd.classList.add("hidden");
});

button_book.addEventListener("click", () => {
  button_querybook.classList.toggle("hidden");
  if (privilege >= 3) {
    button_addbook.classList.toggle("hidden");
    button_modifybook.classList.toggle("hidden");
  }
  RemoveAccount();
});

button_addbook.addEventListener("click", () => {
  div_addbook.classList.remove("hidden");
  div_modifybook.classList.add("hidden");
  div_querybook.classList.add("hidden");
});

button_modifybook.addEventListener("click", () => {
  div_modifybook.classList.remove("hidden");
  div_addbook.classList.add("hidden");
  div_querybook.classList.add("hidden");
});

button_querybook.addEventListener("click", () => {
  div_querybook.classList.remove("hidden");
  div_addbook.classList.add("hidden");
  div_modifybook.classList.add("hidden");
});

button_queryall.addEventListener("click", (event) => {
  event.preventDefault();
  form_querybook_ISBN.classList.add("hidden");
  form_querybook_bookname.classList.add("hidden");
  form_querybook_author.classList.add("hidden");
  form_querybook_keyword.classList.add("hidden");
  fetch('http://localhost:5000/CheckQueryAll')
    .then(response => response.json())
    .then(data => {
      if (data.result) {
        book_table.classList.remove("hidden");
        div_querybook_error.classList.add("hidden");
        book_table.innerHTML = '';
        book_table.appendChild(GetTable(data.books));
      } else {
        book_table.classList.add("hidden");
        div_querybook_error.classList.remove("hidden");
      }
    });
});

button_ISBN.addEventListener("click", () => {
  form_querybook_ISBN.classList.remove("hidden");
  form_querybook_bookname.classList.add("hidden");
  form_querybook_author.classList.add("hidden");
  form_querybook_keyword.classList.add("hidden");
  book_table.classList.add("hidden");
  div_querybook_error.classList.add("hidden");
});

button_bookname.addEventListener("click", () => {
  form_querybook_bookname.classList.remove("hidden");
  form_querybook_ISBN.classList.add("hidden");
  form_querybook_author.classList.add("hidden");
  form_querybook_keyword.classList.add("hidden");
  book_table.classList.add("hidden");
  div_querybook_error.classList.add("hidden");
});

button_author.addEventListener("click", () => {
  form_querybook_author.classList.remove("hidden");
  form_querybook_bookname.classList.add("hidden");
  form_querybook_ISBN.classList.add("hidden");
  form_querybook_keyword.classList.add("hidden");
  book_table.classList.add("hidden");
  div_querybook_error.classList.add("hidden");
});

button_keyword.addEventListener("click", () => {
  form_querybook_keyword.classList.remove("hidden");
  form_querybook_bookname.classList.add("hidden");
  form_querybook_ISBN.classList.add("hidden");
  form_querybook_author.classList.add("hidden");
  book_table.classList.add("hidden");
  div_querybook_error.classList.add("hidden");
});

document.getElementById("form_login").addEventListener("submit", (event) => {
  event.preventDefault();
  const new_userID = document.getElementById("login_ID_text").value;
  const password = document.getElementById("login_password_text").value;
  fetch('http://localhost:5000/CheckLogin', {
    method: "POST",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify({new_userID, password})
  }).then(response => response.json())
    .then(data => {
      if (data.result) {
        alert("登录成功！");
        location.reload();
      } else {
        div_login_error.classList.remove("hidden");
      }
    });
});

document.getElementById("form_register").addEventListener("submit", (event) => {
  event.preventDefault();
  const new_userID = document.getElementById("register_ID_text").value;
  const new_name = document.getElementById("register_name_text").value;
  const password = document.getElementById("register_password_text").value;
  const password_ = document.getElementById("register_password_text_").value;
  if (password !== password_) {
    div_register_password_error.classList.remove("hidden");
    div_register_other_error.classList.add("hidden");
  } else {
    fetch('http://localhost:5000/CheckRegister', {
      method: "POST",
      headers: {
        "Content-Type": "application/json"
      },
      body: JSON.stringify({new_userID, new_name, password})
    }).then(response => response.json())
      .then(data => {
        if (data.result) {
          alert("注册成功！");
          location.reload();
        } else {
          div_register_other_error.classList.remove("hidden");
          div_register_password_error.classList.add("hidden");
        }
      });
  }
});

document.getElementById("form_password").addEventListener("submit", (event) => {
  event.preventDefault();
  const new_userID = document.getElementById("password_ID_text").value;
  let password = "";
  const new_password = document.getElementById("password_new_password_text").value;
  const new_password_ = document.getElementById("password_new_password_text_").value;
  if (privilege === 7) {
    password = document.getElementById("password_password_text").value;
  }
  if (new_password !== new_password_) {
    div_password_ensure_error.classList.remove("hidden");
    div_password_other_error.classList.add("hidden");
  } else {
    fetch('http://localhost:5000/CheckPassword', {
      method: "POST",
      headers: {
        "Content-Type": "application/json"
      },
      body: JSON.stringify({new_userID, password, new_password, new_password_})
    }).then(response => response.json())
      .then(data => {
        if (data.result) {
          alert("修改成功！");
          location.reload();
        } else {
          div_password_ensure_error.classList.add("hidden");
          div_password_other_error.classList.remove("hidden");
        }
      });
  }
});

document.getElementById("form_useradd").addEventListener("submit", (event) => {
  event.preventDefault();
  const new_userID = document.getElementById("useradd_ID_text").value;
  const new_name = document.getElementById("useradd_name_text").value;
  const password = document.getElementById("useradd_password_text").value;
  let new_privilege = "1";
  if (privilege === 7) {
    new_privilege = document.getElementById("useradd_privilege_text").value;
  }

  fetch('http://localhost:5000/CheckUseradd', {
    method: "POST",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify({new_userID, new_name, password, new_privilege})
  }).then(response => response.json())
    .then(data => {
      if (data.result) {
        alert("添加成功！");
        location.reload();
      } else {
        div_useradd_error.classList.remove("hidden");
      }
    });
});

document.getElementById("form_delete").addEventListener("submit", (event) => {
  event.preventDefault();
  const delete_userID = document.getElementById("delete_ID_text").value;
  fetch('http://localhost:5000/CheckDelete', {
    method: "POST",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify({delete_userID})
  }).then(response => response.json())
    .then(data => {
      if (data.result) {
        alert("删除成功！");
        location.reload();
      } else {
        div_delete_error.classList.remove("hidden");
      }
    });
});

document.getElementById("form_addbook").addEventListener("submit", (event) => {
  event.preventDefault();
  const ISBN = document.getElementById("addbook_ISBN").value;
  const bookname = document.getElementById("addbook_bookname").value;
  const author = document.getElementById("addbook_author").value;
  const keyword1 = document.getElementById("addbook_keyword1").value;
  const keyword2 = document.getElementById("addbook_keyword2").value;
  const keyword3 = document.getElementById("addbook_keyword3").value;
  const keyword4 = document.getElementById("addbook_keyword4").value;
  const keyword5 = document.getElementById("addbook_keyword5").value;
  fetch('http://localhost:5000/CheckAddbook', {
    method: "POST",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify({ISBN, bookname, author, keyword1, keyword2, keyword3, keyword4, keyword5})
  }).then(response => response.json())
    .then(data => {
      if (data.result) {
        alert("添加成功！");
        location.reload();
      } else {
        div_addbook_error.classList.remove("hidden");
      }
    });
});

document.getElementById("form_modifybook").addEventListener("submit", (event) => {
  event.preventDefault();
  const ISBN = document.getElementById("modifybook_ISBN").value;
  const new_ISBN = document.getElementById("modifybook_new_ISBN").value;
  const bookname = document.getElementById("modifybook_bookname").value;
  const author = document.getElementById("modifybook_author").value;
  const keyword1 = document.getElementById("modifybook_keyword1").value;
  const keyword2 = document.getElementById("modifybook_keyword2").value;
  const keyword3 = document.getElementById("modifybook_keyword3").value;
  const keyword4 = document.getElementById("modifybook_keyword4").value;
  const keyword5 = document.getElementById("modifybook_keyword5").value;
  fetch('http://localhost:5000/CheckModifybook', {
    method: "POST",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify({ISBN, new_ISBN, bookname, author, keyword1, keyword2, keyword3, keyword4, keyword5})
  }).then(response => response.json())
    .then(data => {
      if (data.result) {
        alert("修改成功！");
        location.reload();
      } else {
        div_modifybook_error.classList.remove("hidden");
      }
    });
});

function GetTable(books) {
  const table = document.createElement("table");
  table.innerHTML = `
    <thead>
      <tr>
        <th>ISBN</th>
        <th>书名</th>
        <th>作者</th>
        <th>关键词</th>
        <th>价格</th>
        <th>库存</th>
      </tr>
    </thead>
    <tbody>
      ${books.map(book => `
        <tr>
          <td>${book.ISBN}</td>
          <td>${book.bookname}</td>
          <td>${book.author}</td>
          <td>${book.keyword}</td>
          <td>${book.price}</td>
          <td>${book.stock}</td>
        </tr>
      `).join('')}
    </tbody>
  `;
  return table;
}

document.getElementById("form_querybook_ISBN").addEventListener("submit", (event) => {
  event.preventDefault();
  const ISBN = document.getElementById("querybook_ISBN").value;
  fetch('http://localhost:5000/CheckQuerybookISBN', {
    method: "POST",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify({ISBN})
  }).then(response => response.json())
    .then(data => {
      if (data.result) {
        book_table.classList.remove("hidden");
        div_querybook_error.classList.add("hidden");
        book_table.innerHTML = '';
        book_table.appendChild(GetTable(data.books));
      } else {
        book_table.classList.add("hidden");
        div_querybook_error.classList.remove("hidden");
      }
    });
});

document.getElementById("form_querybook_bookname").addEventListener("submit", (event) => {
  event.preventDefault();
  const bookname = document.getElementById("querybook_bookname").value;
  fetch('http://localhost:5000/CheckQuerybookBookname', {
    method: "POST",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify({bookname})
  }).then(response => response.json())
    .then(data => {
      if (data.result) {
        book_table.classList.remove("hidden");
        div_querybook_error.classList.add("hidden");
        book_table.innerHTML = '';
        book_table.appendChild(GetTable(data.books));
      } else {
        book_table.classList.add("hidden");
        div_querybook_error.classList.remove("hidden");
      }
    });
});

document.getElementById("form_querybook_author").addEventListener("submit", (event) => {
  event.preventDefault();
  const author = document.getElementById("querybook_author").value;
  fetch('http://localhost:5000/CheckQuerybookAuthor', {
    method: "POST",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify({author})
  }).then(response => response.json())
    .then(data => {
      if (data.result) {
        book_table.classList.remove("hidden");
        div_querybook_error.classList.add("hidden");
        book_table.innerHTML = '';
        book_table.appendChild(GetTable(data.books));
      } else {
        book_table.classList.add("hidden");
        div_querybook_error.classList.remove("hidden");
      }
    });
});

document.getElementById("form_querybook_keyword").addEventListener("submit", (event) => {
  event.preventDefault();
  const keyword = document.getElementById("querybook_keyword").value;
  fetch('http://localhost:5000/CheckQuerybookKeyword', {
    method: "POST",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify({keyword})
  }).then(response => response.json())
    .then(data => {
      if (data.result) {
        book_table.classList.remove("hidden");
        div_querybook_error.classList.add("hidden");
        book_table.innerHTML = '';
        book_table.appendChild(GetTable(data.books));
      } else {
        book_table.classList.add("hidden");
        div_querybook_error.classList.remove("hidden");
      }
    });
});