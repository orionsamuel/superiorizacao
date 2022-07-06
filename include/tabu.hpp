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
#include <queue>
#include "functions.hpp"

using namespace std;

class tabu_search: public functions{
    private:
    int suavityImage[HEIGHT][WIDTH];
    individual sBest;
    individual bestCandidate;
    queue<individual> tabuList;
    individual sNeighborhood[SIZE];
    vector<result> realResults;

    public:
    void init();
    void getNeighbors(individual bestCandidate);

};