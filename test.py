import os
import platform
import subprocess

def compile_and_run():
    print("Compiling the program...")
    compile_command = "cmake --build build --config Release"
    result = subprocess.run(compile_command, shell=True)

    if result.returncode == 0:
        current_os = platform.system()
        print("Compilation successful. Starting the server...")
        subprocess.run(["build/simple_http_server", "--port", "8080", "--file", "index.html"], shell=True)
    else:
        print("Compilation failed.")

if __name__ == "__main__":
    compile_and_run()