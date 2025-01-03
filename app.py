from flask import Flask, jsonify, request
from flask_cors import CORS
import subprocess
import select

cpp_executable = subprocess.Popen(["./cmake-build-debug/code"], stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE, text = True)

app = Flask(__name__)

CORS(app)

# problem : 第一次按下查询所有图书时无法显示所有信息
def Output() :
    ls = []
    while True :
        ready_to_read, _, _ = select.select([cpp_executable.stdout], [], [], 0.02)
        if ready_to_read :
            ls.append(cpp_executable.stdout.readline().strip())
        else :
            break
    return ls

@app.route('/GetUser', methods = ['GET'])
def GetUser() :
    cpp_executable.stdin.write("info\n")
    cpp_executable.stdin.flush()
    output = Output()[0].split()
    return jsonify({
        "privilege" : int(output[0]),
        "userID" : output[1],
        "user_name" : output[2]
    })

@app.route('/CheckLogin', methods = ['POST'])
def CheckLogin() :
    tmp = request.get_json()

    cpp_executable.stdin.write("su " + tmp.get("new_userID") + " " + tmp.get("password") + '\n')
    cpp_executable.stdin.flush()
    if Output()[0] == "Success" :
        return jsonify({"result" : True})
    return jsonify({"result" : False})

@app.route('/Logout', methods = ['GET'])
def Logout() :
    cpp_executable.stdin.write("logout\n")
    cpp_executable.stdin.flush()
    return jsonify()

@app.route('/CheckRegister', methods = ['POST'])
def CheckRegister() :
    tmp = request.get_json()
    cpp_executable.stdin.write("register " + tmp.get("new_userID") + " " + tmp.get("password") + " " + tmp.get("new_name") + '\n')
    cpp_executable.stdin.flush()
    if Output()[0] == "Success" :
        return jsonify({"result" : True})
    return jsonify({"result" : False})

@app.route('/CheckPassword', methods = ['POST'])
def CheckPassword() :
    tmp = request.get_json()
    cpp_executable.stdin.write("passwd " + tmp.get("new_userID") + " " + tmp.get("password") + " " + tmp.get("new_password") + '\n')
    cpp_executable.stdin.flush()
    if Output()[0] == "Success" :
        return jsonify({"result" : True})
    return jsonify({"result" : False})

@app.route('/CheckUseradd', methods = ['POST'])
def CheckUseradd() :
    tmp = request.get_json()
    cpp_executable.stdin.write("useradd " + tmp.get("new_userID") + " " + tmp.get("password") + " " + tmp.get("new_privilege") + " " + tmp.get("new_name") + '\n')
    cpp_executable.stdin.flush()
    if Output()[0] == "Success" :
        return jsonify({"result" : True})
    return jsonify({"result" : False})

@app.route('/CheckDelete', methods = ['POST'])
def CheckDelete() :
    tmp = request.get_json()
    cpp_executable.stdin.write("delete " + tmp.get("delete_userID") + '\n')
    cpp_executable.stdin.flush()
    if Output()[0] == "Success" :
        return jsonify({"result" : True})
    return jsonify({"result" : False})

@app.route('/CheckAddbook', methods = ['POST'])
def CheckAddbook() :
    tmp = request.get_json()
    cpp_executable.stdin.write("show -ISBN=" + tmp.get("ISBN") + '\n')
    cpp_executable.stdin.flush()
    output = Output()
    if output[0] != "" :
        return jsonify({"result" : False})

    cpp_executable.stdin.write("select " + tmp.get("ISBN") + '\n')
    cpp_executable.stdin.flush()
    if Output()[0] != "Success" :
        return jsonify({"result" : False})

    input = "modify"
    if tmp.get("bookname") != "" :
        input += " -name=\"" + tmp.get("bookname") + "\""
    if tmp.get("author") != "" :
        input += " -author=\"" + tmp.get("author") + "\""
    keyword_set = set()
    for i in {"1", "2", "3", "4", "5"} :
        if tmp.get("keyword" + i) != "" :
            keyword_set.add(tmp.get("keyword" + i))
    if bool(keyword_set) :
        input += " -keyword=\"" + keyword_set.pop()
        while bool(keyword_set) :
            input += "|" + keyword_set.pop()
        input += "\""
    if input == "modify" :
        return jsonify({"result" : True})
    cpp_executable.stdin.write(input + '\n')
    cpp_executable.stdin.flush()
    if Output()[0] == "Success" :
        return jsonify({"result" : True})
    return jsonify({"result" : False})

@app.route('/CheckModifybook', methods = ['POST'])
def CheckModifybook() :
    tmp = request.get_json()
    cpp_executable.stdin.write("show -ISBN=" + tmp.get("ISBN") +'\n')
    cpp_executable.stdin.flush()
    if Output()[0] == "" :
        return jsonify({"result" : False})
    cpp_executable.stdin.write("select " + tmp.get("ISBN") + '\n')
    cpp_executable.stdin.flush()
    if Output()[0] != "Success" :
        return jsonify({"result" : False})

    input = "modify"
    if (tmp.get("new_ISBN") != "") :
        input += " -ISBN=" + tmp.get("new_ISBN")
    if tmp.get("bookname") != "" :
        input += " -name=\"" + tmp.get("bookname") + "\""
    if tmp.get("author") != "" :
        input += " -author=\"" + tmp.get("author") + "\""
    keyword_set = set()
    for i in {"1", "2", "3", "4", "5"} :
        if tmp.get("keyword" + i) != "" :
            keyword_set.add(tmp.get("keyword" + i))
    if bool(keyword_set) :
        input += " -keyword=\"" + keyword_set.pop()
        while bool(keyword_set) :
            input += "|" + keyword_set.pop()
        input += "\""
    if input == "modify" :
        return jsonify({"result" : True})
    cpp_executable.stdin.write(input + '\n')
    cpp_executable.stdin.flush()
    if Output()[0] == "Success" :
        return jsonify({"result" : True})
    return jsonify({"result" : False})

def GetBooks(output) :
    books = []
    if output[0] == "" :
        return books
    for str in output :
        book_info = {}
        for item in str.split() :
            key, value = item.split('=')
            book_info[key] = value
        books.append({
            "ISBN" : book_info.get("ISBN"),
            "bookname" : book_info.get("name"),
            "author" : book_info.get("author"),
            "keyword" : book_info.get("keyword"),
            "price" : book_info.get("price"),
            "stock" : book_info.get("stock")
        })
    return books

@app.route('/CheckQuerybookISBN', methods = ['POST'])
def CheckQuerybookISBN() :
    tmp = request.get_json()
    cpp_executable.stdin.write("show -ISBN=" + tmp.get("ISBN") + '\n')
    cpp_executable.stdin.flush()
    output = Output()
    if output[0] == "Invalid" :
        return jsonify({"result" : False})
    return jsonify({"result" : True, "books" : GetBooks(output)})

@app.route('/CheckQuerybookBookname', methods = ['POST'])
def CheckQuerybookBookname() :
    tmp = request.get_json()
    cpp_executable.stdin.write("show -name=\"" + tmp.get("bookname") + "\"\n")
    cpp_executable.stdin.flush()
    output = Output()
    if output[0] == "Invalid" :
        return jsonify({"result" : False})
    return jsonify({"result" : True, "books" : GetBooks(output)})

@app.route('/CheckQuerybookAuthor', methods = ['POST'])
def CheckQuerybookAuthor() :
    tmp = request.get_json()
    cpp_executable.stdin.write("show -author=\"" + tmp.get("author") + "\"\n")
    cpp_executable.stdin.flush()
    output = Output()
    if output[0] == "Invalid" :
        return jsonify({"result" : False})
    return jsonify({"result" : True, "books" : GetBooks(output)})

@app.route('/CheckQuerybookKeyword', methods = ['POST'])
def CheckQuerybookKeyword() :
    tmp = request.get_json()
    cpp_executable.stdin.write("show -keyword=\"" + tmp.get("keyword") + "\"\n")
    cpp_executable.stdin.flush()
    output = Output()
    if output[0] == "Invalid" :
        return jsonify({"result" : False})
    return jsonify({"result" : True, "books" : GetBooks(output)})

@app.route('/CheckQueryAll', methods = ['GET'])
def CheckQueryAll() :
    cpp_executable.stdin.write("show\n")
    cpp_executable.stdin.flush()
    output = Output()
    if output[0] == "Invalid" :
        return jsonify({"result" : False})
    return jsonify({"result" : True, "books" : GetBooks(output)})

if __name__ == '__main__' :
    app.run(debug = True)