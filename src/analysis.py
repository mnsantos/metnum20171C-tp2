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
from sklearn.metrics import confusion_matrix, precision_recall_fscore_support
from random import sample
from collections import namedtuple

Resultado = namedtuple("Resultado", "alfa k_vecinos metodo folds_k tiempo_autovectores tiempo_transformaciones tiempo_clasificacion y_true y_pred")

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

def readFile(filename, folds_k):
	resultados = []
	labels = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
	with open(filename) as f:
		alfas = []
		cant_alfas, cant_vecinos, cant_imagenes_test = [int(n) for n in f.readline().split()]
		for a in range(0, cant_alfas):
			alfa = [int(n) for n in f.readline().split()][0]
			tiempo_autovectores, tiempo_transformaciones = [float(n) for n in f.readline().split()]
			vecinos = []
			for v in range(0,cant_vecinos):
				k_vecinos = [int(n) for n in f.readline().split()][0]
				metodos = []
				for metodo in range(1,3):
					tiempo_clasificacion = [float(n) for n in f.readline().split()][0]
					y_true = []
					y_pred = []
					i = 1
					for calsificacion in range(0,cant_imagenes_test):
						try:
							i +=1
							x, y = [int(n) for n in f.readline().split()]
							y_true.append(x)
							y_pred.append(y)
						except:
							print("falla en la itereacion de resultados")
							sys.exit(1)
					r = Resultado(alfa, k_vecinos, metodo, folds_k, tiempo_autovectores, tiempo_transformaciones, tiempo_clasificacion, y_true, y_pred)
					resultados.append(r)
	return resultados

def leerArchivoResultado(alfa, folds_k, n):
	resultadosArchivo = []
	folds = kfolds(folds_k, n)
	numTest = 1
	for fold in folds:
		archivoResultado = 'results/test_'+str(alfa)+'_'+str(folds_k)+'_'+str(n)+'_'+str(numTest)+'.out'
		print 'leyendo: ', archivoResultado 
		resultadosArchivo += readFile(archivoResultado, folds_k)
		numTest += 1
	return resultadosArchivo

def promediarPorFold(resultadosParciales):
	resultadosTotales = {}
	promediar = {}
	for resultado in resultadosParciales:
		key = str(resultado.alfa)+" "+str(resultado.k_vecinos)+" "+str(resultado.metodo)+" "+str(resultado.folds_k)
		if promediar.get(key, 0) == 0:
			promediar[key] = []
		promediar[key].append(resultado)
	for key in promediar.keys():
		resultadosTotales[key] = promediarResultados(promediar[key])
	return resultadosTotales

def promediarResultados(resultados):
	tiempo_autovectores = 0
	tiempo_transformaciones = 0
	tiempo_clasificacion = 0
	y_true = []
	y_pred = []
	for resultado in resultados:
		tiempo_autovectores += resultado.tiempo_autovectores
		tiempo_transformaciones += resultado.tiempo_transformaciones
		tiempo_clasificacion += resultado.tiempo_clasificacion
		y_true += resultado.y_true
		y_pred += resultado.y_pred
	tiempo_autovectores / len(resultados)
	tiempo_transformaciones / len(resultados)
	tiempo_clasificacion / len(resultados)
	r = Resultado(resultados[0].alfa, resultados[0].k_vecinos, resultados[0].metodo, resultados[0].folds_k, tiempo_autovectores, tiempo_transformaciones, tiempo_clasificacion, y_true, y_pred)
	return r

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
	resultadosParciales = []
	for alfa in alfas:
		for folds_k in folds:
			resultadosParciales += leerArchivoResultado(alfa, folds_k, n)
	resultadosTotales = promediarPorFold(resultadosParciales)
	return resultadosTotales

def cumpleRestricciones(key, restricciones):
	rest_alfa, rest_k_vecinos, rest_metodo, rest_folds_k  = restricciones.split()
	alfa, k_vecinos, metodo, folds_k  = key.split()
	res = True
	res &= ((rest_alfa == "*") or (rest_alfa == alfa))
	res &= ((rest_k_vecinos == "*") or (rest_k_vecinos == k_vecinos))
	res &= ((rest_folds_k == "*") or (rest_folds_k == folds_k))
	res &= ((rest_metodo == "*") or (rest_metodo == metodo))
	return res

def graficarResultados(resultadosTotales,restricciones):
	graficar = [[],[],[],[]]
	labels = range(1,42)
	y_true = []
	y_pred = []
	labelsGrafico = ["alfa", "k_vecinos", "metodo", "folds_k"]
	datoAMostrar = 0
	for rest in restricciones.split():
		if rest == "*":
			break
		datoAMostrar+=1
	for key in resultadosTotales.keys():
		if(cumpleRestricciones(key,restricciones)):
			y_true = resultadosTotales[key].y_true
			y_pred = resultadosTotales[key].y_pred
			alfa = resultadosTotales[key].alfa
			k_vecinos = resultadosTotales[key].k_vecinos
			folds_k = resultadosTotales[key].folds_k
			metodo = resultadosTotales[key].metodo
			graficar[0].append((alfa, precision_recall_fscore_support(y_true, y_pred, average='micro')[0]))
			graficar[1].append((k_vecinos, precision_recall_fscore_support(y_true, y_pred, average='micro')[0]))
			graficar[2].append((metodo, precision_recall_fscore_support(y_true, y_pred, average='micro')[0]))
			graficar[3].append((folds_k, precision_recall_fscore_support(y_true, y_pred, average='micro')[0]))
	if datoAMostrar < 4:
		graficar[datoAMostrar].sort(key=lambda tup: tup[0])
		plt.scatter(*zip(*graficar[datoAMostrar]))
		plt.ylabel('Precision')
		plt.xlabel(labelsGrafico[datoAMostrar])
		plt.show()
	else:
		confMatrix = confusion_matrix(y_true,y_pred,labels)
		plot_confusion_matrix(confMatrix, labels, False, restricciones)
		plt.show()

def main(argv):
	alfas = [100]
	folds = [2, 3, 4, 5, 6, 7, 8, 9]
	#folds = [2, 4, 6, 8]
	n = 10
	#restricciones = "alfa k_vecinos metodo k_folds"
	restricciones = "`* 2 2 6"
	resultados = leerResultados(alfas, folds, n)
	graficarResultados(resultados,restricciones)


if __name__ == "__main__":
	main(sys.argv[1:])
