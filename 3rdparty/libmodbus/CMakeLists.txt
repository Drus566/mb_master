cmake_minimum_required(VERSION 3.10)

# Установка имени проекта и версии
project(libmodbus VERSION 3.1.11)

set(CMAKE_CXX_STANDARD 11)  # Установите нужный стандарт (например, 11, 14, 17 или 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Обязательно использовать указанный стандарт
set(CMAKE_CXX_EXTENSIONS OFF)  # Отключить расширения компилятора

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# Enable position independant code for dynamic library generation
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Исходные файлы
set(SOURCES src/modbus.c
            src/modbus-data.c
            src/modbus-rtu.c
            src/modbus-tcp.c)

# Создание библиотеки
add_library(modbus-shared SHARED ${SOURCES})
add_library(modbus-static STATIC ${SOURCES})

target_include_directories(modbus-shared PUBLIC src)
target_include_directories(modbus-static PUBLIC src)

add_executable(use_modbus examples/main.cpp)
target_link_libraries(use_modbus PUBLIC modbus-static)

# Флаги компилятора
if(MSVC)
    add_compile_options(/W4) # Уровень предупреждений для MSVC
else()
    add_compile_options(-Wall -Wextra -Wpedantic) # Предупреждения для GCC/Clang
endif()

# Проверка необходимых заголовков и библиотек
# find_package(Threads REQUIRED)

# Проверка специфических системных функций или библиотек (пример для Windows)
if(WIN32)
    target_link_libraries(modbus PRIVATE ws2_32)
endif()

