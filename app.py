from flask import Flask, jsonify, request
from flask_cors import CORS
import subprocess
import select

cpp_executable = subprocess.Popen(["./cmake-build-debug/code"], stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE, text = True)

app = Flask(__name__)

CORS(app)

def clear_cpp_output(cpp_executable):
    """清空 C++ 程序的输出缓冲区"""
    # 检查是否有多余的输出数据
    while True:
        # 使用 select 检查 stdout 是否有数据
        ready_to_read, _, _ = select.select([cpp_executable.stdout], [], [], 0)
        if not ready_to_read:
            break

        # 读取并忽略数据
        leftover = cpp_executable.stdout.readline().strip()

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
    output = cpp_executable.stdout.readline()
    clear_cpp_output(cpp_executable)
    if output != "\n" :
        return jsonify({"result" : False})

    cpp_executable.stdin.write("select " + tmp.get("ISBN") + '\n')
    cpp_executable.stdin.flush()
    output = cpp_executable.stdout.readline().strip()
    clear_cpp_output(cpp_executable)
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
    cpp_executable.stdin.write(input + '\n')
    cpp_executable.stdin.flush()
    output = cpp_executable.stdout.readline().strip()
    clear_cpp_output(cpp_executable)
    if output == "Success" :
        return jsonify({"result" : True})
    return jsonify({"result" : False})

if __name__ == '__main__' :
    app.run(debug = True)