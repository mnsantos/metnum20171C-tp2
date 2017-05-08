#ifndef BASETRAIN_H
#define BASETRAIN_H

#include "matriz.h"
#include "parametros.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector.h>

using namespace std;

class BaseTrain {

private:
	//variables
	Parametros params; // parametros de ejecucion
	Matriz base; // matriz con las imagenes transformadas
	vector<int> id; // vector para identificar a que persona corresponde cada imagen
	Matriz autovalores; // vector que contiene los autovalores
	Matriz autovectores; // matriz que contiene los autovectores (cada fila es uno)

	//metodos
	vector<double> transformarImagen(vector<int> imagen); // aplica la transformacion a una imagen para buscar o almacenar

public:
	int buscar(vector<int> imagen); // transforma una imagen y busca la que mas se aproxime en la base
	void agregarImagen(vector<int> imagen, int id); //transforma una imagen y la almacena en la base, guardando el Id en el vector identificador

};

#endif
