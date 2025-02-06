#include <algorithm>
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <iterator>
#include <limits>

#define SUCCESS 0 
#define ERROR 1


void printProgramName(std::string name)
{ 
    if (name.size() > 0)
        std::cout << name << std::endl;
    else
        std::cout << "Program name" << std::endl; 
};

std::ifstream openInputFile()
{
    std::string fileName;
    std::ifstream file;

    do 
    {
        std::cin >> fileName;
        fileName += ".txt";
        file.open(fileName);
        if (!file.is_open()) 
            std::cerr << "It's [" + fileName + "] correct name?" << std::endl;
    } 
    while (!file.is_open());
    std::cout << "File [" << fileName << "] - successfully opened" << std::endl;
    return file;
}
  
void readAdjacencyMatrix(std::ifstream &file, std::vector<std::vector<int>> &matrix, int length)
{
    for (int i = 0; i < length; ++i)
        for (int j = 0; j < length; ++j)
            if (!(file >> matrix[i][j])) 
                std::cout << "Error reading matrix element" << std::endl;
}

int main()
{
    std::string name = "Program - finding the minimum cost";
    printProgramName(name);

    // Объявляем файлы для работы
    std::cout << "Write name of input file" << std::endl;
    std::ifstream inputFile = openInputFile();

    // Читаем размер матриц смежности
    int numFactories;
    inputFile >> numFactories;
    if (numFactories < 0)
    {
        std::cout << "Matrix size must be more zero" << std::endl;
        return ERROR;
    }

    std::vector<std::vector<int>> matrixWay(numFactories, std::vector<int>(numFactories));
    std::vector<std::vector<int>> matrixVolume(numFactories, std::vector<int>(numFactories));

    readAdjacencyMatrix(inputFile, matrixWay, numFactories);
    readAdjacencyMatrix(inputFile, matrixVolume, numFactories);
    
    // Где-то объявляем рациональный путь, текущий путь и минимальный результат 
    int minCost = std::numeric_limits<int>::max();
    std::vector<size_t> sequence(numFactories);
    std::vector<size_t> bestSequence(numFactories);
    for (size_t i = 0; i < numFactories; ++i) 
    {
        sequence[i] = i;
        bestSequence[i] = minCost;
    }    

    std::cout << "\nStart processing...\n" << std::endl;
    // Запуск таймера
    auto start = std::chrono::high_resolution_clock::now();

    // Объявляем перебор лексико-граф. последовательности
    do 
    {
        int currentCost = 0;
        for (int i = 0; i < numFactories; ++i)
            for (int j = 0; j < numFactories; ++j)
                currentCost += matrixWay[i][j] * matrixVolume[sequence[i]][sequence[j]];

        if (currentCost < minCost) 
        {
            minCost = currentCost;
            bestSequence = sequence;
        }
    } 
    while (std::next_permutation(sequence.begin(), sequence.end()));

    // Остановка таймера
    auto end = std::chrono::high_resolution_clock::now();


    // Результат алгоритма
    std::cout << "Results:" << std::endl;
    std::cout << minCost << " - best cost" << std::endl;
    for(size_t i = 0; i < numFactories; ++i)
        std::cout << bestSequence[i] << " ";
    std::cout << "- best way" << std::endl;

    // Вычисление затраченного времени
    std::chrono::duration<double> processTime = end - start;
    std::cout << "Process time: " << processTime.count() << " sec. \n";
    
    return SUCCESS;
}