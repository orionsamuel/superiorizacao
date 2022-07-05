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
#include <iterator>
#include "functions.hpp"

using namespace std;

class gradient: public functions{
    private:
    individual inputImage;
    int suavityImage [WIDTH][HEIGHT];

};