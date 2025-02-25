import os
import subprocess
import platform

def compile_and_run():
    print("Configuring the project...")
    configure_command = ["cmake", "-B", "build"]
    result = subprocess.run(configure_command)

    if result.returncode != 0:
        print("Configuration failed.")
        return

    print("Compiling the program...")
    compile_command = ["cmake", "--build", "build", "--config", "Release"]
    result = subprocess.run(compile_command)

    if result.returncode != 0:
        print("Compilation failed.")
        return
        
    print("Compilation successful. Starting the server...")
    
    # Определяем путь к серверу в зависимости от ОС
    server_path = os.path.join("build", "simple_http_server")
    
    # Запуск сервера
    server_command = [server_path, "--port", "8080", "--file", "assets/index.html"]
    result = subprocess.run(server_command)
    
    if result.returncode != 0:
        print("Failed to start the server.")

if __name__ == "__main__":
    compile_and_run()
