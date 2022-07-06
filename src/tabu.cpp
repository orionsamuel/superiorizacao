#include "tabu.hpp"

void tabu_search::firstSimulation(){
    srand((unsigned)time(0));

    CreateResultDir(0);

    double porosity = Rand_double(MIN_POROSITY, MAX_POROSITY);
    double permeability_1 = Rand_double(MIN_PERMEABILITY, MAX_PERMEABILITY);
    double permeability_2 = Rand_double(MIN_PERMEABILITY, MAX_PERMEABILITY);
    double permeability_3 = Rand_double(MIN_PERMEABILITY, MAX_PERMEABILITY);

    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            this->sBest.porosity[HEIGHT][WIDTH] = porosity;
            for(int k = 0; k < 3; k++){
                this->sBest.permeability[HEIGHT][WIDTH].permeability_1 = permeability_1;
                this->sBest.permeability[HEIGHT][WIDTH].permeability_2 = permeability_2;
                this->sBest.permeability[HEIGHT][WIDTH].permeability_3 = permeability_3;
            }
        }
    }

    this->bestCandidate = sBest;

    WriteSimulationFile(0, 0, simulationFile, fileName, sBest);
}

void tabu_search::init(){
    CreateOutputDir();

    string oilInputResult = ReadFileInput(inputOil);
    string waterInputResult = ReadFileInput(inputWater);
    string gasInputResult = ReadFileInput(inputGas);

    this->realResults = ConvertStringInputToDoubleResult(waterInputResult, oilInputResult, gasInputResult); 

    firstSimulation(); 

    // for(int i = 0; i < HEIGHT; i++){
    //     for(int j = 0; j < WIDTH; j++){
    //         cout << this->sBest.porosity[HEIGHT][WIDTH] << endl;
    //         for(int k = 0; k < 3; k++){
    //             cout << this->sBest.permeability[HEIGHT][WIDTH].permeability_1 << endl;
    //             cout << this->sBest.permeability[HEIGHT][WIDTH].permeability_2 << endl;
    //             cout << this->sBest.permeability[HEIGHT][WIDTH].permeability_3 << endl;
    //         }
    //     }
    // }

}