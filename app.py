from flask import Flask, jsonify, request
from flask_cors import CORS
import subprocess
import select

cpp_executable = subprocess.Popen(["./cmake-build-debug/code"], stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE, text = True)

app = Flask(__name__)

CORS(app)
def Output1() :
    n = cpp_executable.stdout.readline().strip()
    if n == "Invalid" :
        return []
    n = int(n)
    ls = []
    for i in range(n) :
        ls.append(cpp_executable.stdout.readline().strip())
    return ls

@app.route('/GetUser', methods = ['GET'])
def GetUser() :
    cpp_executable.stdin.write("info\n")
    cpp_executable.stdin.flush()
    output = cpp_executable.stdout.readline().split()
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
    output = cpp_executable.stdout.readline().strip()
    if output == "Success" :
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
    output = cpp_executable.stdout.readline().strip()
    if output == "Success" :
        return jsonify({"result" : True})
    return jsonify({"result" : False})

@app.route('/CheckPassword', methods = ['POST'])
def CheckPassword() :
    tmp = request.get_json()
    cpp_executable.stdin.write("passwd " + tmp.get("new_userID") + " " + tmp.get("password") + " " + tmp.get("new_password") + '\n')
    cpp_executable.stdin.flush()
    output = cpp_executable.stdout.readline().strip()
    if output == "Success" :
        return jsonify({"result" : True})
    return jsonify({"result" : False})

@app.route('/CheckUseradd', methods = ['POST'])
def CheckUseradd() :
    tmp = request.get_json()
    cpp_executable.stdin.write("useradd " + tmp.get("new_userID") + " " + tmp.get("password") + " " + tmp.get("new_privilege") + " " + tmp.get("new_name") + '\n')
    cpp_executable.stdin.flush()
    output = cpp_executable.stdout.readline().strip()
    if output == "Success" :
        return jsonify({"result" : True})
    return jsonify({"result" : False})

@app.route('/CheckDelete', methods = ['POST'])
def CheckDelete() :
    tmp = request.get_json()
    cpp_executable.stdin.write("delete " + tmp.get("delete_userID") + '\n')
    cpp_executable.stdin.flush()
    output = cpp_executable.stdout.readline().strip()
    if output == "Success" :
        return jsonify({"result" : True})
    return jsonify({"result" : False})

@app.route('/CheckAddbook', methods = ['POST'])
def CheckAddbook() :
    tmp = request.get_json()
    cpp_executable.stdin.write("show -ISBN=" + tmp.get("ISBN") + '\n')
    cpp_executable.stdin.flush()
    ls = Output1()
    if len(ls) != 0 :
        return jsonify({"result" : False})

    cpp_executable.stdin.write("select " + tmp.get("ISBN") + '\n')
    cpp_executable.stdin.flush()
    output = cpp_executable.stdout.readline().strip()
    if output != "Success" :
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
    output = cpp_executable.stdout.readline().strip()
    if output == "Success" :
        return jsonify({"result" : True})
    return jsonify({"result" : False})

@app.route('/CheckModifybook', methods = ['POST'])
def CheckModifybook() :
    tmp = request.get_json()
    cpp_executable.stdin.write("show -ISBN=" + tmp.get("ISBN") +'\n')
    cpp_executable.stdin.flush()
    ls = Output1()
    if len(ls) == 0 :
        return jsonify({"result" : False})
    cpp_executable.stdin.write("select " + tmp.get("ISBN") + '\n')
    cpp_executable.stdin.flush()
    output = cpp_executable.stdout.readline().strip()
    if output != "Success" :
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
    output = cpp_executable.stdout.readline().strip()
    if output == "Success" :
        return jsonify({"result" : True})
    return jsonify({"result" : False})

def GetBooks(output) :
    books = []
    if len(output) == 0 :
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
    output = Output1()
    return jsonify({"result" : True, "books" : GetBooks(output)})

@app.route('/CheckQuerybookBookname', methods = ['POST'])
def CheckQuerybookBookname() :
    tmp = request.get_json()
    cpp_executable.stdin.write("show -name=\"" + tmp.get("bookname") + "\"\n")
    cpp_executable.stdin.flush()
    output = Output1()
    return jsonify({"result" : True, "books" : GetBooks(output)})

@app.route('/CheckQuerybookAuthor', methods = ['POST'])
def CheckQuerybookAuthor() :
    tmp = request.get_json()
    cpp_executable.stdin.write("show -author=\"" + tmp.get("author") + "\"\n")
    cpp_executable.stdin.flush()
    output = Output1()
    return jsonify({"result" : True, "books" : GetBooks(output)})

@app.route('/CheckQuerybookKeyword', methods = ['POST'])
def CheckQuerybookKeyword() :
    tmp = request.get_json()
    cpp_executable.stdin.write("show -keyword=\"" + tmp.get("keyword") + "\"\n")
    cpp_executable.stdin.flush()
    output = Output1()
    return jsonify({"result" : True, "books" : GetBooks(output)})

@app.route('/CheckQueryAll', methods = ['GET'])
def CheckQueryAll() :
    cpp_executable.stdin.write("show\n")
    cpp_executable.stdin.flush()
    output = Output1()
    return jsonify({"result" : True, "books" : GetBooks(output)})

@app.route('/CheckImport', methods = ['POST'])
def CheckImport() :
    tmp = request.get_json()
    cpp_executable.stdin.write("show -ISBN=" + tmp.get("ISBN") +'\n')
    cpp_executable.stdin.flush()
    ls = Output1();
    if len(ls) == 0 :
        return jsonify({"result" : False})
    cpp_executable.stdin.write("select " + tmp.get("ISBN") + '\n')
    cpp_executable.stdin.flush()
    output = cpp_executable.stdout.readline().strip()
    if output != "Success" :
        return jsonify({"result" : False})
    cpp_executable.stdin.write("import " + tmp.get("quantity") + " " + tmp.get("cost") + '\n')
    cpp_executable.stdin.flush()
    output = cpp_executable.stdout.readline().strip()
    if output == "Success" :
        return jsonify({"result" : True})
    return jsonify({"result" : False})

@app.route('/CheckBuy', methods = ['POST'])
def CheckBuy() :
    tmp = request.get_json()
    cpp_executable.stdin.write("buy " + tmp.get("ISBN") + " " + tmp.get("quantity") + '\n')
    cpp_executable.stdin.flush()
    output = cpp_executable.stdout.readline().strip()
    if output == "Invalid" :
        return jsonify({"result" : False})
    return jsonify({"result" : True, "cost" : output})

@app.route('/CheckFinance', methods = ['GET'])
def CheckFinance() :
    cpp_executable.stdin.write("report finance\n");
    cpp_executable.stdin.flush()
    output = Output1()
    ls = []
    for x in output :
        y = []
        y.append(x[0:7].strip())
        y.append(x[7:42].strip())
        y.append(x[42:142].strip())
        y.append(x[142:162].strip())
        y.append(x[162:182].strip())
        ls.append(y)
    return jsonify({"info" : ls})

@app.route('/CheckLog', methods = ['GET'])
def CheckLog() :
    cpp_executable.stdin.write("log\n");
    cpp_executable.stdin.flush()
    output = Output1()
    ls = []
    for x in output :
        y = []
        y.append(x[0:7].strip())
        y.append(x[7:42].strip())
        y.append(x[42:142].strip())
        ls.append(y)
    return jsonify({"info" : ls})

if __name__ == '__main__' :
    app.run(debug = True)