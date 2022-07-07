#include <iostream>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include <random>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include "functions.hpp"

using namespace std;

class tabu_search: public functions{
    private:
    int suavityImage[HEIGHT][WIDTH];
    individual sBest;
    individual bestCandidate;
    vector<individual> tabuList;
    individual sNeighborhood[SIZE];
    vector<result> realResults;

    public:
    void Init();
    void FirstSimulation();
    void OthersSimulations(int idIterations);
    double Fitness(int idIteration, int iterator, individual sCandidate);
    void GetNeighbors(individual bestCandidate);
    bool Contains(individual sCandidate);
    void SaveTabuList();
    void SaveBest();

};