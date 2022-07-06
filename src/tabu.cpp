#include "tabu.hpp"

void tabu_search::FirstSimulation(){
    srand((unsigned)time(0));

    CreateResultDir(0);

    double porosity = Rand_double(MIN_POROSITY, MAX_POROSITY);
    double permeability_1 = Rand_double(MIN_PERMEABILITY, MAX_PERMEABILITY);
    double permeability_2 = Rand_double(MIN_PERMEABILITY, MAX_PERMEABILITY);
    double permeability_3 = Rand_double(MIN_PERMEABILITY, MAX_PERMEABILITY);

    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            this->sBest.porosity[i][j] = porosity;
            this->sBest.permeability[i][j].permeability_1 = permeability_1;
            this->sBest.permeability[i][j].permeability_2 = permeability_2;
            this->sBest.permeability[i][j].permeability_3 = permeability_3;
        }
    }

    this->tabuList.push(sBest);

    this->bestCandidate = sBest;

    WriteSimulationFile(0, 0, simulationFile, fileName, sBest);

    Simulation(0, fileName);
    Fitness(0);
}

void tabu_search::OthersSimulations(int idIteration){
    GetNeighbors(this->bestCandidate);
}

void tabu_search::GetNeighbors(individual bestCandidate){
    for(int i = 0; i < SIZE; i++){
        if(i <= (SIZE / 2)){
            double porosity = Rand_double(bestCandidate.porosity[0][0], MAX_POROSITY);
            double permeability_1 = Rand_double(bestCandidate.permeability[0][0].permeability_1, MAX_PERMEABILITY);
            double permeability_2 = Rand_double(bestCandidate.permeability[0][0].permeability_2, MAX_PERMEABILITY);
            double permeability_3 = Rand_double(bestCandidate.permeability[0][0].permeability_3, MAX_PERMEABILITY);
            for(int j = 0; j < HEIGHT; i++){
                for(int k = 0; k < WIDTH; j++){
                    this->sNeighborhood[i].porosity[j][k] = porosity;
                    this->sNeighborhood[i].permeability[j][k].permeability_1 = permeability_1;
                    this->sNeighborhood[i].permeability[j][k].permeability_2 = permeability_2;
                    this->sNeighborhood[i].permeability[j][k].permeability_3 = permeability_3;
                }
            }
        }else{
            double porosity = Rand_double(MIN_POROSITY, bestCandidate.porosity[0][0]);
            double permeability_1 = Rand_double(MIN_PERMEABILITY, bestCandidate.permeability[0][0].permeability_1);
            double permeability_2 = Rand_double(MIN_PERMEABILITY, bestCandidate.permeability[0][0].permeability_2);
            double permeability_3 = Rand_double(MIN_PERMEABILITY, bestCandidate.permeability[0][0].permeability_3);
            for(int j = 0; j < HEIGHT; i++){
                for(int k = 0; k < WIDTH; j++){
                    this->sNeighborhood[i].porosity[j][k] = porosity;
                    this->sNeighborhood[i].permeability[j][k].permeability_1 = permeability_1;
                    this->sNeighborhood[i].permeability[j][k].permeability_2 = permeability_2;
                    this->sNeighborhood[i].permeability[j][k].permeability_3 = permeability_3;
                }
            }
        }
    }
}

void tabu_search::Fitness(int idIteration){
    if(idIteration == 0){
        string oilOutputResult = "../Output/"+to_string(idIteration)+"/oleo/"+to_string(0)+".txt";
            string waterOutputResult = "../Output/"+to_string(idIteration)+"/agua/"+to_string(0)+".txt";
            string gasOutputResult = "../Output/"+to_string(idIteration)+"/gas/"+to_string(0)+".txt";
            this->sBest.error_rank = activationFunction(waterOutputResult, oilOutputResult, gasOutputResult, realResults, idIteration, 0);
    }else{
        for(int i = 0; i < SIZE; i++){
            string oilOutputResult = "../Output/"+to_string(idIteration)+"/oleo/"+to_string(i)+".txt";
            string waterOutputResult = "../Output/"+to_string(idIteration)+"/agua/"+to_string(i)+".txt";
            string gasOutputResult = "../Output/"+to_string(idIteration)+"/gas/"+to_string(i)+".txt";
            this->sNeighborhood[i].error_rank = activationFunction(waterOutputResult, oilOutputResult, gasOutputResult, realResults, idIteration, i);
        }
    }
}

void tabu_search::Init(){
    CreateOutputDir();

    string oilInputResult = ReadFileInput(inputOil);
    string waterInputResult = ReadFileInput(inputWater);
    string gasInputResult = ReadFileInput(inputGas);

    this->realResults = ConvertStringInputToDoubleResult(waterInputResult, oilInputResult, gasInputResult); 

    FirstSimulation();
    int count = 1;
    while(count <= N_ITERATIONS or this->sBest.error_rank > STOP){
        OthersSimulations(count);
    }

}