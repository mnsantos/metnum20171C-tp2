#include "parametros.h"
#include "pca.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

Parametros leerParametros(ifstream &archivoEntrada, string infile){
	Parametros params;
	params.infile = infile;
	archivoEntrada >> params.path;
	archivoEntrada >> params.f;
	archivoEntrada >> params.c;
	archivoEntrada >> params.p;
	archivoEntrada >> params.nimgp;
	archivoEntrada >> params.k;
	return params;
}

vector<double> leerArchivoImagen(string archivoImagen, Parametros params){
	string linea;
	ifstream archivoEntrada;
	vector<double> vectorImagen;
	archivoImagen += ".dat";
	archivoEntrada.open(archivoImagen.c_str(), ios::binary);
	int count = 0;
	if(archivoEntrada.fail()){
		cout << "Falla Archivo de imagen: " << archivoImagen << endl;
		exit(0);
	} else {
		for (std::string line; std::getline(archivoEntrada, line); ){
			// process "line"
			std::stringstream ss(line);
			std::string token;

			while(std::getline(ss, token, ',')) {
				vectorImagen.push_back(atoi(token.c_str()));
			}
		}

	}
	return vectorImagen;
}

Matriz cargarImagenesTrain(ifstream &archivoEntrada, Parametros params, vector<int>* labels){
	Matriz imagenesTrain;
	string directorioPersona;
	string numeroImagen;
	for (int i = 0; i < params.p; ++i)
	{
		//i-esima persona
		archivoEntrada >> directorioPersona;
		for (int j = 0; j < params.nimgp; ++j)
		{
			//j-esima imagen
			archivoEntrada >> numeroImagen;
			vector<double> vectorArchivoImagen = leerArchivoImagen(params.path + directorioPersona + numeroImagen +".pgm", params);
			imagenesTrain.agregarFila(vectorArchivoImagen);
			labels->push_back(atof(directorioPersona.substr(1, directorioPersona.size()).c_str()));
		}
	}
	return imagenesTrain;
}

Matriz cargarImagenesTest(ifstream &archivoEntrada, Parametros params, vector<int>* labels){
	Matriz imagenesTest;
	string imagenTest;
	int sujeto;
	archivoEntrada >> params.ntest;
	for (int i = 0; i < params.ntest; ++i)
	{
		archivoEntrada >> imagenTest;
		archivoEntrada >> sujeto;
		vector<double> vectorArchivoImagen = leerArchivoImagen(imagenTest, params);
		imagenesTest.agregarFila(vectorArchivoImagen);
		labels->push_back(sujeto);
	}
	return imagenesTest;
}

vector<pair<int, int> > clasificarImagenes(Matriz imagenes, vector<int> labels, PCA pca){
	vector<pair<int, int> > resultados;
	for (int i = 0; i < imagenes.filas(); ++i)
	{
		int asignado = pca.clasificar(imagenes[i], 1);
		int label = labels[i];
		pair<int, int> res = make_pair(label, asignado);
		resultados.push_back(res);
		
	}
	return resultados;
}

int main(int argc, char const *argv[])
{
	int vecinos = 1;
	int alpha = 1;
	string infile = argv[1];
	ifstream archivoEntrada;
	archivoEntrada.open(infile.c_str());
	if(archivoEntrada.fail()){
		cout << "Falla Archivo de entrada" << endl;
		exit(0);
	}

	Parametros params = leerParametros(archivoEntrada, infile);

	vector<int>* labelsTrain = new vector<int>();
	
	Matriz A = cargarImagenesTrain(archivoEntrada, params, labelsTrain);

/*	cout << "inicio A" << endl;
	cout << A << endl;
	cout << "final A" << endl;*/

	//PCA pca_ = PCA(A, (*labelsTrain), vecinos, alpha);
	vector<int>* labelsTest = new vector<int>();

	Matriz T = cargarImagenesTest(archivoEntrada, params, labelsTest);

	//vector<pair<int, int> > clasificadas = clasificarImagenes(T, (*labelsTest), pca_);

	//salida = pca_.getAutovalores();
	
	delete labelsTrain;
	delete labelsTest;

	cout << "salida" << endl;
	return 0;
}