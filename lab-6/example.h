#include "Randoms.cpp"

class ACO
{
public:
	ACO(int nAnts,
		int numCities,
		double alpha,
		double beta,
		double qpar,
		double pheromonesEvaporation,
		double randMaxPheromone,
		int initCity);
	virtual ~ACO();

	// Инициализация
	void init();

	// Строим связь и прокладываем начальный ферамон (скорее всего не надо)
	void connectCITIES(int cityi, int cityj);

	// Задаем координаты города
	void setCITYPOSITION(int city, double x, double y);

	// Вывод
	void printPHEROMONES();
	void printGRAPH();
	void printRESULTS();

	void optimize(int ITERATIONS);

private:
	double distance(int cityi, int cityj);
	bool exists(int cityi, int cityc);
	bool vizited(int antk, int c);
	double PHI(int cityi, int cityj, int antk);

	double length(int antk);

	int city();
	void route(int antk);
	int valid(int antk, int iteration);

	void updatePHEROMONES();

	int NUMBEROFANTS, NUMBEROFCITIES, INITIALCITY;
	double ALPHA, BETA, Q, RO, TAUMAX;

	double BESTLENGTH;
	int* BESTROUTE;

	int **GRAPH, **ROUTES;
	double **CITIES, **PHEROMONES, **DELTAPHEROMONES, **PROBS;

	Randoms* randoms;
};
