#!/usr/bin/python

import sys
import getopt
import subprocess
import time
import platform
import os.path
import numpy as np
import sklearn

np.set_printoptions(threshold=np.inf, linewidth=np.inf)

caras = 'ImagenesCarasRed'
#caras = 'ImagenesCaras'

def createTestingFile(alpha, folds_k, n):
		folds = kfolds(folds_k, n)
		numTest = 1
		for fold in folds:
			fname = 'tests/test_'+str(alpha)+'_'+str(folds_k)+'_'+str(n)+'_'+str(numTest)+'.in'
			print 'escribiendo: ', fname
			with open(fname, 'wr+') as f:
				nimgp = 0
				training = ''
				testing = []
				#para cada fold hay que crear un archivo de entrada
				for x in xrange(0,10):
					if fold[x] == True:
						training += ' '+str(x+1)
						nimgp += 1
					else:
						testing.append(x+1)
				#Seleccionadas las imagenes de training y las de test
				#se procede a escribir el contenido del archivo de entrada
				#La primera linea lleva los datos de siempre, con el numero de imagenes del fold y el numero de componentes principales alpha
				f.write('../data/'+caras+'/ 23 28 41 '+str(nimgp)+' '+str(alpha)+'\n')
				for x in xrange(1,42):
					f.write('s'+str(x)+'/'+training+'\n')
				ntest = len(testing) * 41
				f.write(str(ntest)+'\n')
				for x in xrange(1,42):
					for y in testing:
						f.write('../data/'+caras+'/s'+str(x)+'/'+str(y)+'.pgm '+str(x)+'\n')
			numTest += 1


def crearArchivosTesting(alphas, folds, n):
	for alpha in alphas:
		for folds_k in folds:
			createTestingFile(alpha, folds_k, n)

def callTestingFile(alpha, folds_k, n):
		folds = kfolds(folds_k, n)
		numTest = 1
		for fold in folds:
			archivoEntrada = 'tests/test_'+str(alpha)+'_'+str(folds_k)+'_'+str(n)+'_'+str(numTest)+'.in'
			archivoSalida = 'results/test_'+str(alpha)+'_'+str(folds_k)+'_'+str(n)+'_'+str(numTest)+'.out'
			args = ['./tp', archivoEntrada, archivoSalida]
			print 'ejecutando: ', archivoEntrada 
			process = subprocess.call(args)
			numTest += 1
	
def ejecutarArchivosTesting(alphas, folds, n):
	for alpha in alphas:
		for folds_k in folds:
			callTestingFile(alpha, folds_k, n)

def kfolds(k, n):
	num_folds = k
	subset_size = n/num_folds
	trainings = []
	for i in range(0, k):
		training = np.array([True]*n)
		training[i*subset_size:][:subset_size] = False
		trainings.append(training)
	return trainings

def main(argv):
	alphas = [60]
	folds = [2, 4, 6, 8]
	n = 10
	crearArchivosTesting(alphas, folds, n)
	ejecutarArchivosTesting(alphas, folds, n)


if __name__ == "__main__":
	main(sys.argv[1:])