#include <chrono>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

// Вывод названия программы в консоль
void PrintProgramName(const string& name)
{
	cout << (name.empty() ? "[Program name]" : name) << endl;
}

// Поиск следующей комбинации
bool NextCombinations(size_t dim, std::vector<size_t>& combination)
{
	size_t len = combination.size();
	int i = len - 1;

	while (i >= 0 && combination[i] == dim - len + i + 1)
	{
		--i;
	}

	if (i < 0)
	{
		return false;
	}

	++combination[i];

	for (size_t j = i + 1; j < len; ++j)
	{
		combination[j] = combination[j - 1] + 1;
	}
	return true;
}

int main()
{
	const string name = "Program - finding the minimum employees";
	PrintProgramName(name);

	set<size_t> projectSkills{ 1, 2, 3, 4, 5, 6, 7, 8 };
	vector<set<size_t>> employees = {
		{ 1, 2, 3 },
		{ 1, 2 },
		{ 5, 7 },
		{ 8, 1, 2, 5 },
		{ 6, 7 },
		{ 8, 1, 2, 3 },
		{ 4, 3, 1, 2 }
	};
	size_t numberEmployee = employees.size();

	// Запуск таймера
	auto start = chrono::high_resolution_clock::now();

	for (size_t i = 1; i <= numberEmployee; ++i)
	{
		vector<size_t> combination(i);
		for (size_t j = 0; j < i; ++j)
		{
			combination[j] = j + 1;
		}

		do
		{
			set<size_t> combinedSkills;
			for (size_t k : combination)
			{
				combinedSkills.insert(employees[k - 1].begin(), employees[k - 1].end());
			}

			if (combinedSkills == projectSkills)
			{
				cout << "Minimum employees found: ";
				for (size_t k : combination)
				{
					cout << k << " ";
				}
				cout << endl;

				// Остановка таймера
				auto end = std::chrono::high_resolution_clock::now();

				// Вычисление затраченного времени
				std::chrono::duration<double> processTime = end - start;
				std::cout << "Process time: " << processTime.count() << " sec. \n";

				return EXIT_SUCCESS;
			}
		} while (NextCombinations(numberEmployee, combination));
	}

	// Остановка таймера
	auto end = std::chrono::high_resolution_clock::now();

	// Вычисление затраченного времени
	std::chrono::duration<double> processTime = end - start;
	std::cout << "Process time: " << processTime.count() << " sec. \n";

	cout << "No combination of workers covers all skills." << endl;
	return EXIT_SUCCESS;
}