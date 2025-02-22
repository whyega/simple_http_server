# Simple HTTP Server
Проект Simple HTTP Server представляет собой пример простого многопоточного HTTP-сервера, написанного на C++. Сервер может обрабатывать GET-запросы и возвращать содержимое указанного файла. Проект включает в себя исходный код сервера, а также скрипт на Python для удобного запуска.

## Особенности
* Многопоточная обработка запросов.
* Поддержка GET-запросов.
* Возможность указания порта и файла для обслуживания.
* Кроссплатформенность (поддержка Windows и Unix-подобных систем).

## Структура проекта
* http_server.hpp и http_server.cpp: Основной код HTTP-сервера.
* socket.hpp и socket.cpp: Класс Socket для работы с сокетами.
* main.cpp: Точка входа в программу, где происходит парсинг аргументов командной строки и запуск сервера.
* test.py: Скрипт на Python для сборки и запуска сервера.
* CMakeLists.txt: Файл для сборки проекта с помощью CMake.

## Зависимости
* [gabime/spdlog](https://github.com/gabime/spdlog) Библиотека для логирования.
* [jarro2783/cxxopts](https://github.com/jarro2783/cxxopts) Библиотека для парсинга аргументов командной строки
* CMake: Система сборки.

## Сборка и запуск
1. Сборка с помощью CMake
2. Убедитесь, что у вас установлен CMake и компилятор C++ (например, g++ или MSVC).
3. Создайте директорию для сборки:
```bash
mkdir build
cd build
```
4. Запустите CMake для настройки проекта:
```bash
cmake ..
```
5. Соберите проект:
```bash
cmake --build .
```

## Запуск сервера
После сборки вы можете запустить сервер вручную, указав порт и файл для обслуживания:
```bash
./simple_http_server --port 8080 --file path/to/your/file.html
```
## Запуск с помощью скрипта
Для удобства можно использовать скрипт на Python (test.py), который автоматически собирает и запускает сервер:
```bash
python test.py
```
Скрипт выполняет следующие действия:
1. Настраивает проект с помощью CMake.
2. Собирает проект.
3. Запускает сервер на порту 8080, обслуживая файл assets/index.html.

## Пример использования
Запустите сервер, указав порт и файл:
```bash
./simple_http_server --port 8080 --file index.html
```
Откройте браузер и перейдите по адресу http://localhost:8080. Вы увидите содержимое файла index.html.
