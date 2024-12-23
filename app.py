from flask import Flask, jsonify, request
from flask_cors import CORS
import subprocess

cpp_executable = subprocess.Popen(["./cmake-build-debug/code"], stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE, text = True)

app = Flask(__name__)

CORS(app)

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

if __name__ == '__main__' :
    app.run(debug = True)