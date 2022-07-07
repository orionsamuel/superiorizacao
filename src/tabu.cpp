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

    WriteSimulationFile(0, 0, simulationFile, fileName, this->sBest);
    
    Simulation(0, 1, fileName);
    sBest.error_rank = Fitness(0, 0, this->sBest);

    WriteErrorFile(0, this->sBest);

    this->tabuList.push_back(sBest);

    this->bestCandidate = sBest;
}

void tabu_search::OthersSimulations(int idIteration){
    GetNeighbors(this->bestCandidate);

    CreateResultDir(idIteration);

    for(int i = 0; i < SIZE; i++){
        WriteSimulationFile(idIteration, i, simulationFile, fileName, this->sNeighborhood[i]);
    }

    Simulation(idIteration, SIZE, fileName);
    for(int i = 0; i < SIZE; i++){
        this->sNeighborhood[i].error_rank = Fitness(idIteration, i, this->sNeighborhood[i]);
    }

    for(int i = 0; i < SIZE; i++){
        WriteErrorFile(idIteration, this->sNeighborhood[i]);
    }

    this->bestCandidate = sNeighborhood[0];

    for(int i = 0; i < SIZE; i++){
        if(!Contains(sNeighborhood[i]) && sNeighborhood[i].error_rank < this->bestCandidate.error_rank){
            this->bestCandidate = sNeighborhood[i];
        }
    }

    if(this->bestCandidate.error_rank < this->sBest.error_rank){
        this->sBest = this->bestCandidate;
    }

    this->tabuList.push_back(this->bestCandidate);

    if(tabuList.size() > TABU_SIZE){
        tabuList.erase(tabuList.begin());
    }

}

double tabu_search::Fitness(int idIteration, int iterator, individual sCandidate){
    string oilOutputResult = "../Output/"+to_string(idIteration)+"/oleo/"+to_string(iterator)+".txt";
    string waterOutputResult = "../Output/"+to_string(idIteration)+"/agua/"+to_string(iterator)+".txt";
    string gasOutputResult = "../Output/"+to_string(idIteration)+"/gas/"+to_string(iterator)+".txt";
    double error = activationFunction(waterOutputResult, oilOutputResult, gasOutputResult, realResults, idIteration);
    
    return error;
}

void tabu_search::GetNeighbors(individual bestCandidate){
    for(int i = 0; i < SIZE; i++){
        if(i < (SIZE / 2)){
            double porosity = Rand_double(bestCandidate.porosity[0][0], bestCandidate.porosity[0][0]+0.03);
            double permeability_1 = Rand_double(bestCandidate.permeability[0][0].permeability_1, bestCandidate.permeability[0][0].permeability_1+30);
            double permeability_2 = Rand_double(bestCandidate.permeability[0][0].permeability_2, bestCandidate.permeability[0][0].permeability_2+30);
            double permeability_3 = Rand_double(bestCandidate.permeability[0][0].permeability_3, bestCandidate.permeability[0][0].permeability_2+30);
            for(int j = 0; j < HEIGHT; j++){
                for(int k = 0; k < WIDTH; k++){
                    this->sNeighborhood[i].porosity[j][k] = Min(porosity, MAX_POROSITY);
                    this->sNeighborhood[i].permeability[j][k].permeability_1 = Min(permeability_1, MAX_PERMEABILITY);
                    this->sNeighborhood[i].permeability[j][k].permeability_2 = Min(permeability_2, MAX_PERMEABILITY);
                    this->sNeighborhood[i].permeability[j][k].permeability_3 = Min(permeability_3, MAX_PERMEABILITY);
                }
            }
        }else{
            double porosity = Rand_double(bestCandidate.porosity[0][0]-0.03, bestCandidate.porosity[0][0]);
            double permeability_1 = Rand_double(bestCandidate.permeability[0][0].permeability_1-30, bestCandidate.permeability[0][0].permeability_1);
            double permeability_2 = Rand_double(bestCandidate.permeability[0][0].permeability_2-30, bestCandidate.permeability[0][0].permeability_2);
            double permeability_3 = Rand_double(bestCandidate.permeability[0][0].permeability_3-30, bestCandidate.permeability[0][0].permeability_3);
            for(int j = 0; j < HEIGHT; j++){
                for(int k = 0; k < WIDTH; k++){
                    this->sNeighborhood[i].porosity[j][k] = Max(porosity, MIN_POROSITY);
                    this->sNeighborhood[i].permeability[j][k].permeability_1 = Max(permeability_1, MIN_PERMEABILITY);
                    this->sNeighborhood[i].permeability[j][k].permeability_2 = Max(permeability_2, MIN_PERMEABILITY);
                    this->sNeighborhood[i].permeability[j][k].permeability_3 = Max(permeability_3, MIN_PERMEABILITY);
                }
            }
        }
    }
}

bool tabu_search::Contains(individual sCandidate){
    bool contains = false;

    for(int i = 0; i < this->tabuList.size(); i++){
        if((sCandidate.permeability[0][0].permeability_1 == this->tabuList[i].permeability[0][0].permeability_1) and
        (sCandidate.permeability[0][0].permeability_2 == this->tabuList[i].permeability[0][0].permeability_2) and
        (sCandidate.permeability[0][0].permeability_3 == this->tabuList[i].permeability[0][0].permeability_3)){
            contains = true;
        }
    }

    return contains;
}

void tabu_search::SaveTabuList(){
    CreateResultDir(N_ITERATIONS+1);

    for(int i = 0; i < tabuList.size(); i++){
        WriteSimulationFile(N_ITERATIONS+1, i, simulationFile, fileName, this->tabuList[i]);
    }

    Simulation(N_ITERATIONS+1, this->tabuList.size(), fileName);
    for(int i = 0; i < tabuList.size(); i++){
        this->tabuList[i].error_rank = Fitness(N_ITERATIONS+1, i, this->tabuList[i]);
    }

    for(int i = 0; i < tabuList.size(); i++){
        WriteErrorFile(N_ITERATIONS+1, this->tabuList[i]);
    }
}

void tabu_search::SaveBest(){
    CreateResultDir(N_ITERATIONS+2);

    WriteSimulationFile(N_ITERATIONS+2, 0, simulationFile, fileName, this->sBest);

    Simulation(N_ITERATIONS+2, 1, fileName);
    sBest.error_rank = Fitness(N_ITERATIONS+2, 0, this->sBest);

    WriteErrorFile(N_ITERATIONS+2, this->sBest);
}

void tabu_search::Init(){
    CreateOutputDir();

    string oilInputResult = ReadFileInput(inputOil);
    string waterInputResult = ReadFileInput(inputWater);
    string gasInputResult = ReadFileInput(inputGas);

    this->realResults = ConvertStringInputToDoubleResult(waterInputResult, oilInputResult, gasInputResult); 

    FirstSimulation();
    int count = 1;
    while(count <= N_ITERATIONS && this->sBest.error_rank > STOP){
        OthersSimulations(count);
        count++;
    }

    SaveTabuList();

    SaveBest();

}