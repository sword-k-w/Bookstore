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
const button_delete = document.getElementById("button_delete");
const div_login = document.getElementById("div_login");
const div_login_error = document.getElementById("div_login_error");
const div_register = document.getElementById("div_register");
const div_register_password_error = document.getElementById("div_register_password_error");
const div_register_other_error = document.getElementById("div_register_other_error");

function CheckDisplay() {
  if (button_login.classList.contains("hidden")) {
    div_login.classList.add("hidden");
  }
}
fetch('http://localhost:5000/GetUser')
  .then(response => response.json())
  .then(data => {
    account.textContent = data.user_name;
    privilege = data.privilege;
    userID = data.userID;
    if (privilege === 7) {
      button_log.classList.remove("hidden");
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
    if (privilege === 7) {
      button_delete.classList.toggle("hidden");
    }
  }
  CheckDisplay();
});

button_login.addEventListener("click", () => {
  div_login.classList.remove("hidden");
  div_register.classList.add("hidden");
});

button_register.addEventListener("click", () => {
  div_register.classList.remove("hidden");
  div_login.classList.add("hidden");
})

button_logout.addEventListener("click", () => {
  fetch('http://localhost:5000/Logout')
    .then(response => {
      location.reload();
    })
})

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
        location.reload();
      } else {
        div_login_error.classList.remove("hidden");
      }
    })
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
          location.reload();
        } else {
          div_register_other_error.classList.remove("hidden");
          div_register_password_error.classList.add("hidden");
        }
      })
  }
})