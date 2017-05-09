#ifndef PARAMETROS_H
#define PARAMETROS_H

#include <utility>
#include <iostream>
#include "matriz.h"
#include <fstream>
#include <stdio.h>
#include <map>

using namespace std;

struct Parametros {
	//variables
	string infile; //archivo de entrada
	string path; // directorio donde estan las imagenes (relativo a la ejecucion del programa)
	int f; // filas de cada imagen
	int c; // columnas de cada imagen
	int p; // cantidad de personas
	int nimgp; // cantidad de imagenes por persona
	int k; // cantidad de componentes principales
	int ntest; //cantidad de imagenes de test
};

#endif
