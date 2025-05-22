#!/bin/bash

# Настройки компилятора
CXX="g++"
CXXFLAGS="-std=c++17 -O2"
OUTPUT="start"

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'
YELLOW='\033[0;33m'
# YELLOW='\033[1;33m'  # ярко-жёлтый

writeSuccess() 
{
  echo -e "${GREEN}Сборка успешна${NC}"
  ./$OUTPUT
}

writeProblems()
{
  echo -e "${YELLOW}Ошибка сборки${YELLOW}"
  exit 1
}

writeError() 
{
  echo -e "${RED}Ошибка компиляции${NC}"
  exit 1
}

# Найти все .cpp файлы (рекурсивно)
SOURCES=$(find . -name "*.cpp" -type f | tr '\n' ' ')

if [ -z "$SOURCES" ]; then
    writeProblems
fi

# Компиляция
$CXX $CXXFLAGS $SOURCES -o $OUTPUT

# Проверка успешности компиляции
if [ $? -eq 0 ]; then
    writeSuccess
else
    writeError
fi