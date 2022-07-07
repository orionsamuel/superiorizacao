#include "../include/functions.hpp"

void functions::Simulation(int idIteration, int size, string file){
    system(Command("cp ../summaryplot.py ../Output/"+to_string(idIteration)));

    for(int i = 0; i < size; i++){
        cout << "Executando a simulação no indivíduo " << i << " da iteração " << idIteration << endl;
        system(Command("mpirun -np 4 flow ../Output/"+to_string(idIteration)+"/"+to_string(i)+"-"+file+".DATA >> out.txt"));
        system(Command("python3 ../Output/"+to_string(idIteration)+"/summaryplot.py WOPR:PROD WWPR:PROD WGPR:PROD ../Output/"+to_string(idIteration)+"/"+to_string(i)+"-"+file+".DATA >> out.txt"));
        system(Command("mv WOPR:PROD.txt ../Output/"+to_string(idIteration)+"/oleo/"+to_string(i)+".txt"));
        system(Command("mv WWPR:PROD.txt ../Output/"+to_string(idIteration)+"/agua/"+to_string(i)+".txt"));
        system(Command("mv WGPR:PROD.txt ../Output/"+to_string(idIteration)+"/gas/"+to_string(i)+".txt"));
        system(Command("rm ../Output/"+to_string(idIteration)+"/"+to_string(i)+"-"+file+".DBG"));
        system(Command("rm ../Output/"+to_string(idIteration)+"/"+to_string(i)+"-"+file+".EGRID"));
        system(Command("rm ../Output/"+to_string(idIteration)+"/"+to_string(i)+"-"+file+".INFOSTEP"));
        system(Command("rm ../Output/"+to_string(idIteration)+"/"+to_string(i)+"-"+file+".INIT"));
        system(Command("rm ../Output/"+to_string(idIteration)+"/"+to_string(i)+"-"+file+".PRT"));
        system(Command("rm ../Output/"+to_string(idIteration)+"/"+to_string(i)+"-"+file+".SMSPEC"));
        system(Command("rm ../Output/"+to_string(idIteration)+"/"+to_string(i)+"-"+file+".UNRST"));
        system(Command("rm ../Output/"+to_string(idIteration)+"/"+to_string(i)+"-"+file+".UNSMRY"));
    }

    system(Command("rm  ../Output/"+to_string(idIteration)+"/summaryplot.py"));
}

double functions::Rand_double(double min, double max){
    random_device rd;
    default_random_engine eng(rd());
    uniform_real_distribution<double>distr(min, max);

    double num = distr(eng);
    
    num = floor(num *100) / 100;

    return num;
}

const vector<string> functions::split(const string& s, const char& c){
    string buff{""};
	vector<string> v;
	
	for(auto n:s){
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}

	if(buff != "") v.push_back(buff);
	
	return v;
}

void functions::CreateOutputDir(){
    DIR* dp = opendir("../Output/");

    if(dp == NULL){
        system("mkdir ../Output/");
    }else{
        system("rm -r -f ../Output/*");
    }
}

string functions::ReadFileInput(string file){
    ifstream fileStream(file, ios::in);
    
    string line, content;

    while(!fileStream.eof()){
        getline(fileStream, line);
        content += line;
        content += " ";
    }

    fileStream.close();

    return content;
}

vector<result> functions::ConvertStringInputToDoubleResult(string water, string oil, string gas){
    vector<string> waterSplit{split(water, ' ')};
    vector<string> oilSplit{split(oil, ' ')};
    vector<string> gasSplit{split(gas, ' ')};

    vector<result> results;
    
    for(int i = 0; i < waterSplit.size(); i++){
        result partialResult;
        partialResult.water = stod(waterSplit[i]);
        partialResult.oil = stod(oilSplit[i]);
        partialResult.gas = stod(gasSplit[i]);
        results.push_back(partialResult);
    }

    return results;

}

const char* functions::Command(string inputCommand){
    const char* command = (char*) inputCommand.c_str();
    return command;
}

void functions::CreateResultDir(int idIteration){
    string command = "../Output/"+to_string(idIteration);
    const char* file = (char*) command.c_str();
    DIR* dp = opendir(file);

    if(dp == NULL){;
        system(Command("mkdir ../Output/"+to_string(idIteration)));
        system(Command("mkdir ../Output/"+to_string(idIteration)+"/oleo"));
        system(Command("mkdir ../Output/"+to_string(idIteration)+"/agua"));
        system(Command("mkdir ../Output/"+to_string(idIteration)+"/gas"));
    }else{
        const char* rm = Command("rm -f ../Output/"+to_string(idIteration)+"/*");
        system(rm);
    }

}

void functions::WriteSimulationFile(int idIteration, int iterator, string inputFile, string file, individual sCandidate){
    ifstream input(inputFile, ios::in);
    ofstream output("../Output/"+to_string(idIteration)+"/"+to_string(iterator)+"-"+file+".DATA", ios::out);
    string line;
    int count = 0;

    while(!input.eof()){
        getline(input, line);
        if(count == 92){
            output << "    " << TOTAL_CELLS << "*" << sCandidate.porosity[0][0] << " /"  << endl;
        }else if(count == 96){
            output << "    " << "100*" << sCandidate.permeability[0][0].permeability_1 << " 100*" << sCandidate.permeability[0][0].permeability_2 << " 100*" << sCandidate.permeability[0][0].permeability_3 << " /" << endl;
        }else if(count == 100){
            output << "    " << "100*" << sCandidate.permeability[0][0].permeability_1 << " 100*" << sCandidate.permeability[0][0].permeability_2 << " 100*" << sCandidate.permeability[0][0].permeability_3 << " /"  << endl;
        }else if(count == 105){
            output << "    " << "100*" << sCandidate.permeability[0][0].permeability_1 << " 100*" << sCandidate.permeability[0][0].permeability_2 << " 100*" << sCandidate.permeability[0][0].permeability_3 << " /"  << endl;
        }else{
            output << line << endl;
        }

        count++;
    }

    input.close();
    output.close();
    
}

void functions::WriteErrorFile(int idIteration, individual sCandidate){
    ofstream errorFile("../Output/"+to_string(idIteration)+"/error.txt", ios::app);
    
    errorFile << sCandidate.error_rank << endl;

    errorFile.close();
}

double functions::activationFunction(string waterOutputResult, string oilOutputResult, string gasOutputResult, vector<result> results, int idIteration, int iterator){
    double rank;

    string waterResult = ReadFileInput(waterOutputResult);
    string oilResult = ReadFileInput(oilOutputResult);
    string gasResult = ReadFileInput(gasOutputResult);

    vector<result> simulateResults;

    simulateResults = ConvertStringInputToDoubleResult(waterResult, oilResult, gasResult);

    for(int i = 0; i < N_METRICS; i++){
        if(i == 0){
            for(int j = 0; j < simulateResults.size(); j++){
                rank += pow((results[j].water - simulateResults[j].water),2);
            }
            rank *= WATER_WEIGHT;
        }if(i == 1){
            for(int j = 0; j < simulateResults.size(); j++){
                rank += pow((results[j].oil - simulateResults[j].oil),2);
            }
            rank *= GAS_WEIGHT;
        }else if(i == 1){
            for(int j = 0; j < simulateResults.size(); j++){
                rank += pow((results[j].gas - simulateResults[j].gas),2);
            }
            rank *= OIL_WEIGHT;
        }           
    }

    rank = sqrt((rank / (simulateResults.size() * N_METRICS)));

    return rank;
}
