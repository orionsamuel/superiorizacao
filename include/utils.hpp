#include <string>

using namespace std;

const string inputOil = "../Input/oleo.txt";
const string inputWater = "../Input/agua.txt";
const string inputGas = "../Input/gas.txt";
const string simulationFile = "../Input/SPE1CASE1.DATA";
const string fileName = "SPE1CASE1";

#define N_ITERATIONS 100
#define STOP 40

#define MIN_POROSITY 0.1
#define MAX_POROSITY 0.3

#define MIN_PERMEABILITY 50.0
#define MAX_PERMEABILITY 500.0

#define WATER_WEIGHT 0.2
#define OIL_WEIGHT 0.5
#define GAS_WEIGHT 0.3

#define TOTAL_CELLS 300

#define N_METRICS 3

#define HEIGHT 30
#define WIDTH 10
#define SIZE 30
#define TABU_SIZE 50

struct perm{
    double permeability_1;
    double permeability_2;
    double permeability_3;
};

struct individual{
    double porosity[HEIGHT][WIDTH];
    perm permeability[HEIGHT][WIDTH];
    double error_rank;
};

struct result{
    double water;
    double oil;
    double gas;
};


