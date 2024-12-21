from flask import Flask, jsonify
import subprocess

cpp_executable = subprocess.Popen(["./cmake-build-debug/gen"], stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE, text = True)

while True:

    cpp_executable.stdin.write("a\n")
    cpp_executable.stdin.flush()

    print("!!!")

    output = cpp_executable.stdout.readline()

    print("???")

    print(output)

    if cpp_executable.poll() == 0 :
        break

