cmake_minimum_required(VERSION 3.10)
project(MyShape VERSION 1.0 LANGUAGES CXX)

# Включение межмодульной оптимизации
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_output)

# Настройка компилятора
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto -fno-exceptions -fno-rtti -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -pedantic -fsanitize=address,undefined")
endif()

# Вместо GLOB лучше явно перечислить исходные файлы
set(SOURCES
    class-aco/aco.cpp
    class-ant/ant.cpp
    class-graph/graph.cpp
    class-graph/matrix.cpp
    main.cpp
)

# Создание исполняемого файла с поддержкой IPO если доступно
if(ipo_supported)
    add_executable(start ${SOURCES})
    set_property(TARGET start PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    add_executable(start ${SOURCES})
    message(WARNING "IPO is not supported: ${ipo_output}")
endif()

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)