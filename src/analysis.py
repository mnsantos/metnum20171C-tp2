#!/usr/bin/python

from __future__ import division
import sys
import getopt
import platform
import itertools
import os.path
import matplotlib.pyplot as plt
import numpy as np
from numpy import ravel, reshape, swapaxes
import scipy.io
from sklearn import svm
from sklearn.metrics import confusion_matrix, accuracy_score
from random import sample
from collections import namedtuple

Resultado = namedtuple("Resultado", "tiempo_autovectores tiempo_transformaciones tiempo_clasificacion precision_recall_fscore_support confMatrix")

def plot_confusion_matrix(cm, classes,
                          normalize=False,
                          title='Confusion matrix',
                          cmap=plt.cm.Blues):
    plt.imshow(cm, interpolation='nearest', cmap=cmap)
    plt.title(title)
    plt.colorbar()
    tick_marks = np.arange(len(classes))
    plt.xticks(tick_marks, classes, rotation=45)
    plt.yticks(tick_marks, classes)

    if normalize:
        cm = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]
        #print("Normalized confusion matrix")
    #else:
        #print('Confusion matrix, without normalization')

    #print(cm)

    thresh = cm.max() / 2.
    for i, j in itertools.product(range(cm.shape[0]), range(cm.shape[1])):
        plt.text(j, i, cm[i, j],
                 horizontalalignment="center",
                 color="white" if cm[i, j] > thresh else "black")

    plt.tight_layout()
    plt.ylabel('True label')
    plt.xlabel('Predicted label')

def readFile(filename, folds_k, n, numTest):
	labels = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
	resultados = []
	with open(filename) as f:
		alfas = []
		cant_alfas, cant_vecinos, cant_imagenes_test = f.readline().split()
		print(cant_alfas, cant_vecinos, cant_imagenes_test)
		for a in range(0, int(cant_alfas)):
			alfa = f.readline().split()
			print(alfa)
			tiempo_autovectores, tiempo_transformaciones = f.readline().split()
			print(tiempo_autovectores, tiempo_transformaciones)
			vecinos = []
			for v in range(0,int(cant_vecinos)):
				k_vecinos = f.readline().split()
				print(k_vecinos)
				metodos = []
				for metodo in range(0,2):
					tiempo_clasificacion = f.readline().split()
					print("tiempo clasificacion: " + tiempo_clasificacion[0])
					y_true = []
					y_pred = []
					i = 1
					for calsificacion in range(0,int(cant_imagenes_test)):
						try:
							print(str(i) + " de " + cant_imagenes_test)
							i +=1
							#print("esto deberia ser x, y")
							print(f.readline().split())
							# x, y = [int(n) for n in f.readline().split()]
							# print(x, y)
							# y_true.append(x)
							# y_pred.append(y)
						except:
							print("error occured in line " )#+ str(i))
							sys.exit(1)
					# confMatrix = confusion_matrix(y_true,y_pred,labels)
					# precision_recall_fscore_support = sklearn.metrics.precision_recall_fscore_support(y_true, y_pred, 1.0, labels)
					# r = Resultado(tiempo_autovectores, tiempo_transformaciones, tiempo_clasificacion, precision_recall_fscore_support, confMatrix)
					# key = alfa+'_'+k_vecinos+'_'+metodo+'_'+folds_k+'_'+numTest
					# resultados.put(key, r)
	return resultados

def leerArchivoResultado(alfa, folds_k, n):
		folds = kfolds(folds_k, n)
		numTest = 1
		for fold in folds:
			resultados = []
			archivoResultado = 'results/test_'+str(alfa)+'_'+str(folds_k)+'_'+str(n)+'_'+str(numTest)+'.out'
			print 'leyendo: ', archivoResultado 
			resultados.append(readFile(archivoResultado, folds_k, n, numTest))
			numTest += 1
		promediarResultados(resultados)

def promediarPorFold(cant_alfas, cant_vecinos, cant_imagenes_test):
	resultadosTotales = []
	for alfa in alfas:
		for vecino in vecinos:
			for metodo in metodos:
				metodos = []
				for fold in folds:
					resultadosFold = []
					for numTest in tests:
						key = alfa+'_'+vecino+'_'+metodo+'_'+fold+'_'+numTest
						#tengo que tener folds veces cada hoja, y promediar entre ellas.
						resultadosFold.append(resultados[key])
						#al final tengo que tener un arbol estandar, pero con una lista de resultados en el lugar de las hojas que habia antes que eran un solo resultado.
						#en esas hojas promedio esas listas.
						#termina quedando un arbol estandar de resultados
						#muestro los resultados
					resultadosTotales.append(promediarResultados(resultadosFold))

def promediarResultados(resultados):
	tiempo_autovectores = 0
	tiempo_transformaciones = 0
	tiempo_clasificacion = 0
	precision_recall_fscore_support = 0
	confMatrix = 0
	for resultado in resultados:
		tiempo_autovectores += resultado.tiempo_autovectores
		tiempo_transformaciones += resultado.tiempo_transformaciones
		tiempo_clasificacion += resultado.tiempo_clasificacion
		precision_recall_fscore_support += resultado.precision_recall_fscore_support
		confMatrix += resultado.confMatrix
	tiempo_autovectores / len(resultados)
	tiempo_transformaciones / len(resultados)
	tiempo_clasificacion / len(resultados)
	precision_recall_fscore_support / len(resultados)
	confMatrix / len(resultados)

def kfolds(k, n):
	num_folds = k
	subset_size = n/num_folds
	trainings = []
	for i in range(0, k):
		training = np.array([True]*n)
		training[i*subset_size:][:subset_size] = False
		trainings.append(training)
	return trainings

def leerResultados(alfas, folds, n):
	for alfa in alfas:
		for folds_k in folds:
			leerArchivoResultado(alfa, folds_k, n)

def main(argv):
	alfas = [60]
	folds = [2, 4, 6, 8]
	n = 10
	leerResultados(alfas, folds, n)


if __name__ == "__main__":
	main(sys.argv[1:])
