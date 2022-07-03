import pandas as pd
import numpy as np
import sklearn as sk
import random as rd
from sklearn.mixture import GaussianMixture
from sklearn import preprocessing
import scipy.stats
import os
import math
from sklearn import metrics
import matplotlib.pyplot as plt
from sklearn.metrics import mean_absolute_percentage_error

permeabilityX1 = []
permeabilityX2 = []
permeabilityX3 = []
permeabilityY1 = []
permeabilityY2 = []
permeabilityY3 = []
permeabilityZ1 = []
permeabilityZ2 = []
permeabilityZ3 = []
error = [[],[]]

total_cells = 300

min_por = 0.1
max_por  = 0.3
min_per = 50.0
max_per = 500.0

def fill_cell():
    for i in range(100):
        permeabilityX1.append(round(rd.uniform(min_per,max_per),2))
        permeabilityX2.append(round(rd.uniform(min_per,max_per),2))
        permeabilityX3.append(round(rd.uniform(min_per,max_per),2))
        permeabilityY1.append(round(rd.uniform(min_per,max_per),2))
        permeabilityY2.append(round(rd.uniform(min_per,max_per),2))
        permeabilityY3.append(round(rd.uniform(min_per,max_per),2))
        permeabilityZ1.append(round(rd.uniform(min_per,max_per),2))
        permeabilityZ2.append(round(rd.uniform(min_per,max_per),2))
        permeabilityZ3.append(round(rd.uniform(min_per,max_per),2))

def dataframe_create():
    permx = pd.DataFrame(list(zip(permeabilityX1, permeabilityX2, permeabilityX3)), columns =['px1', 'px2', 'px3'])
    permy = pd.DataFrame(list(zip(permeabilityY1, permeabilityY2, permeabilityY3)), columns =['py1', 'py2', 'py3'])
    permz = pd.DataFrame(list(zip(permeabilityZ1, permeabilityZ2, permeabilityZ3)), columns =['pz1', 'pz2', 'pz3'])
    
    return permx, permy, permz

def cluster_create():
    permx, permy, permz = dataframe_create()
    gmmX = GaussianMixture(n_components=3, covariance_type='full', random_state=rd.randint(1,1024))
    gmmY = GaussianMixture(n_components=3, covariance_type='full', random_state=rd.randint(1,1024))
    gmmZ = GaussianMixture(n_components=3, covariance_type='full', random_state=rd.randint(1,1024))
    gmmX.fit(permx)
    gmmY.fit(permy)
    gmmZ.fit(permz)

    X = permx.values
    Y = permy.values
    Z = permz.values

    silhouette = []

    silhouette.append(metrics.silhouette_score(permx, gmmX.predict(permx)))
    silhouette.append(metrics.silhouette_score(permy, gmmY.predict(permy)))
    silhouette.append(metrics.silhouette_score(permz, gmmZ.predict(permz)))
    
    return X, Y, Z, gmmX, gmmY, gmmZ, np.mean(silhouette)

def centers_create():
    X, Y, Z, gmmX, gmmY, gmmZ, silhouette = cluster_create()

    centerX = np.empty(shape=(gmmX.n_components, X.shape[1]))
    centerY = np.empty(shape=(gmmY.n_components, Y.shape[1]))
    centerZ = np.empty(shape=(gmmZ.n_components, Z.shape[1]))

    for i in range(gmmX.n_components):
        densityX = scipy.stats.multivariate_normal(cov=gmmX.covariances_[i], mean=gmmX.means_[i]).logpdf(X)
        centerX[i, :] = X[np.argmax(densityX)]
    for i in range(gmmY.n_components):
        densityY = scipy.stats.multivariate_normal(cov=gmmY.covariances_[i], mean=gmmY.means_[i]).logpdf(Y)
        centerY[i, :] = Y[np.argmax(densityY)]
    for i in range(gmmZ.n_components):
        densityZ = scipy.stats.multivariate_normal(cov=gmmZ.covariances_[i], mean=gmmZ.means_[i]).logpdf(Z)
        centerZ[i, :] = Z[np.argmax(densityZ)]


    return centerX, centerY, centerZ, silhouette

    #print(round(centers[0][0]),round(centers[1][0]),round(centers[2][0]))

def file_create(n, centerX, centerY, centerZ, porosity):
    inputFile = open("../Input/SPE1CASE1.DATA", "r")
    outputFile = open("../Output/"+str(n)+"-SPE1CASE1.DATA", "w")
    count = 0;
    for line in inputFile:
        if(count == 92):
            outputFile.write("    "+str(total_cells)+"*"+str(porosity)+" /\n")
        elif(count == 96):
            outputFile.write("    100*"+str(centerX[0][0])+" 100*"+str(centerX[1][0])+" 100*"+str(centerX[2][0])+" /\n")
        elif(count == 100):
            outputFile.write("    100*"+str(centerY[0][0])+" 100*"+str(centerY[1][0])+" 100*"+str(centerY[2][0])+" /\n")
        elif(count == 105):
            outputFile.write("    100*"+str(centerZ[0][0])+" 100*"+str(centerZ[1][0])+" 100*"+str(centerZ[2][0])+" /\n")
        else:
            outputFile.write(line)

        count = count + 1
            
    outputFile.close()
    inputFile.close()

def simulation(n, centerX, centerY, centerZ, porosity):
    file_create(n, centerX, centerY, centerZ, porosity)
    os.system("mpirun -np 4 flow ../Output/"+str(n)+"-SPE1CASE1.DATA >> out.txt")
    os.system("python3 ../Output/summaryplot.py WOPR:PROD WWPR:PROD WGPR:PROD "
            +"../Output/"+str(n)+"-SPE1CASE1.DATA")
    os.system("mv WOPR:PROD.txt ../Output/oleo/"+str(n)+".txt")
    os.system("mv WWPR:PROD.txt ../Output/agua/"+str(n)+".txt")
    os.system("mv WGPR:PROD.txt ../Output/gas/"+str(n)+".txt")
    os.system("rm ../Output/"+str(n)+"-SPE1CASE1.DBG")
    os.system("rm ../Output/"+str(n)+"-SPE1CASE1.EGRID")
    os.system("rm ../Output/"+str(n)+"-SPE1CASE1.INFOSTEP")
    os.system("rm ../Output/"+str(n)+"-SPE1CASE1.INIT")
    os.system("rm ../Output/"+str(n)+"-SPE1CASE1.PRT")
    os.system("rm ../Output/"+str(n)+"-SPE1CASE1.SMSPEC")
    os.system("rm ../Output/"+str(n)+"-SPE1CASE1.UNRST")
    os.system("rm ../Output/"+str(n)+"-SPE1CASE1.UNSMRY")

def objeticve_funcion(n):
    water_result = []
    realWater = []
    oil_result = []
    realOil = []
    gas_result = []
    realGas = []

    real_result_water = open("../Input/agua.txt", "r")
    real_result_oil =  open("../Input/oleo.txt", "r")
    real_result_gas =  open("../Input/gas.txt", "r")

    for line in real_result_water:
        realWater.append(float(line))

    for line in real_result_oil:
        realOil.append(float(line))

    for line in real_result_gas:
        realGas.append(float(line))

    real_result_water.close()
    real_result_oil.close()
    real_result_gas.close()

    water = []
    oil = []
    gas = []
        
    inputFile_Water = open("../Output/agua/"+str(n)+".txt", "r")
    inputFile_Oil = open("../Output/oleo/"+str(n)+".txt", "r")
    inputFile_Gas = open("../Output/gas/"+str(n)+".txt", "r")

    for line in inputFile_Water:
        water.append(float(line))

    water_result.append(water)

    inputFile_Water.close()

    for line in inputFile_Oil:
        oil.append(float(line))

    oil_result.append(oil)
    inputFile_Oil.close()

    for line in inputFile_Gas:
        gas.append(float(line))

    gas_result.append(oil)
    inputFile_Gas.close()

    rank = 0
    for j in range(3):
        if(j == 0):
            for k in range(len(realWater)-5):
                rank = rank + math.pow((realWater[k] - water_result[0][k]),2)
            rank = rank * 0.2
        elif(j == 1):
            for k in range(len(realOil)-5):
                rank = rank + math.pow((realOil[k] - oil_result[0][k]),2)
            rank = rank * 0.5
        else:
            for k in range(len(realGas)-5):
                rank = rank + math.pow((realGas[k] - gas_result[0][k]),2)
            rank = rank * 0.3

    rank = math.sqrt(rank / (len(realWater) * 3))

    return rank

def chart_plot():
    center1, center3, center4, center5, centerPorosity, silhouette = centers_create()

    fig = plt.figure()
    ax = fig.add_subplot(projection='3d')
    ax.scatter(porosity1, porosity3, porosity4, porosity5)
    ax.set_xlabel('Porosity (%)')
    ax.set_ylabel('Porosity (%)')
    ax.set_zlabel('Porosity (%)')
    ax.scatter(centerPorosity[:,0], centerPorosity[:,1], centerPorosity[:,2], s=100)

    '''plt.scatter(permeability3x, permeability3y, permeability3z)
    plt.scatter(centers[:,0], centers[:,1], s=20)
    plt.show()'''

    plt.savefig('porosity.png')

def init(n):
    if(not os.path.exists("../Output")):
        os.system("mkdir ../Output")
        os.system("cp summaryplot.py ../Output/")
    else:
        os.system("rm -r -f ../Output/*")
        
    os.system("mkdir ../Output/agua")
    os.system("mkdir ../Output/oleo")
    os.system("mkdir ../Output/gas")
    os.system("cp summaryplot.py ../Output/")
    os.system("rm -f out.txt")
    
    for i in range(n):
        fill_cell()
        centerX, centerY, centerZ, silhouette = centers_create()
        simulation(i, centerX, centerY, centerZ, round(rd.uniform(0.1,0.3),2))
        error_result = objeticve_funcion(i)
        print("Executou a simulação: "+str(i))
        error_file = open("../Output/error.txt", "a")
        error_file.write(str(error_result) + "\n")
        error_file.close()
            
  
    print("Finish!")

if __name__ == '__main__':
    init(10)
