cmake_minimum_required(VERSION 3.28)
project(UniversalCppProject VERSION 1.0.0 LANGUAGES CXX)

# Требуем стандарт C++20 так как используются модули
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Включаем автоматическое сканирование зависимостей модулей для CMake
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# 1. Автоматический поиск всех исходных, заголовочных файлов и модулей
# Флаг CONFIGURE_DEPENDS заставляет CMake автоматически перезапускаться при добавлении новых файлов
file(GLOB_RECURSE ALL_SRCS CONFIGURE_DEPENDS
    "*.cpp"
    "*.hpp"
    "*.cppm"
)

# 2. Создание цели
add_executable($ONAME ${ALL_SRCS})

# 3. Добавление путей для #include, чтобы можно было использовать #include "..." для заголовков из подпапок
get_filename_component(ALL_SOURCE_DIRS "." ABSOLUTE)
target_include_directories($ONAME PRIVATE ${ALL_SOURCE_DIRS})
