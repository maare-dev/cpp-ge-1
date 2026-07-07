#!/bin/bash
export PROJECT_NAME="cpp-ge-1_0.0.1"

# Удаляем старый кэш, чтобы избежать конфликтов генераторов
rm -r build

# Конфигурируем и собираем.
# Оператор && гарантирует: если cmake упадет, скрипт прервется
CC=clang CXX=clang++ cmake -G Ninja -B build -DPROJECT_NAME="$PROJECT_NAME" && cmake --build build

# Проверяем статус сборки (0 - успех, всё остальное - ошибка)
if [ $? -ne 0 ]; then
    echo -e "\n\033[1;31m[ОШИБКА] Сборка провалилась. Запуск невозможен.\033[0m"
    exit 1
fi

# Предлагаем запуск только при успешной сборке
run=''
read -r -p "run? (Y/n)" run
case "$run" in
    ""|[Yy])
        clear
        "./build/$PROJECT_NAME"
        ;;
    *)
        echo "cancelled"
        ;;
esac
