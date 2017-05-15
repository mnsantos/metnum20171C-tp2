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
	archivoEntrada.open(archivoImagen.c_str());
	cout << archivoImagen.c_str() << endl;
	if(archivoEntrada.fail()){
		cout << "Falla Archivo de imagen: " << archivoImagen << endl;
		exit(0);
	} else {
		unsigned char n;
		while(archivoEntrada >> n) {
			//cout << "char : " << n << endl;
			//cout << "uint : " << (unsigned int) n << endl;
			vectorImagen.push_back((double) n);
		}
		//cout << "size: " << vectorImagen.size() << endl;
	}
	return vectorImagen;
}

Matriz cargarImagenesTrain(ifstream &archivoEntrada, Parametros params){
	Matriz imagenesTrain;
	vector<double> labels;
	string directorioPersona;
	string numeroImagen;
	for (int i = 0; i < params.p; ++i)
	{
		//i-esima persona
		archivoEntrada >> directorioPersona;
		for (int j = 0; j < 1; ++j)
		{
			//j-esima imagen
			archivoEntrada >> numeroImagen;
			vector<double> vectorArchivoImagen = leerArchivoImagen(params.path + directorioPersona + numeroImagen +".pgm", params);
			imagenesTrain.agregarFila(vectorArchivoImagen);
			labels.push_back(atof(directorioPersona.substr(2,-1).c_str()));
		}
	}
	//imagenesTrain.agregarFila(labels);
	return imagenesTrain;
}

Matriz cargarImagenesTest(ifstream &archivoEntrada, Parametros params){
	Matriz imagenesTest;
	string imagenTest;
	vector<double> labels;
	int sujeto;
	archivoEntrada >> params.ntest;
	for (int i = 0; i < params.ntest; ++i)
	{
		archivoEntrada >> imagenTest;
		archivoEntrada >> sujeto;
		vector<double> vectorArchivoImagen = leerArchivoImagen(imagenTest, params);
		imagenesTest.agregarFila(vectorArchivoImagen);
		labels.push_back(sujeto);
	}
	//imagenesTest.agregarFila(labels);
	return imagenesTest;
}

vector<pair<int, int> > clasificarImagenes(Matriz imagenes, vector<double> labels, PCA pca){
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
	string infile = argv[1];
	ifstream archivoEntrada;
	archivoEntrada.open(infile.c_str());
	if(archivoEntrada.fail()){
		cout << "Falla Archivo de entrada" << endl;
		exit(0);
	}

	Parametros params = leerParametros(archivoEntrada, infile);

	Matriz A = cargarImagenesTrain(archivoEntrada, params);

	/*
	cout << "inicio A" << endl;
	cout << A << endl;
	cout << "final A" << endl;
	*/
	//vector<double> labels = A.ultFila();
	//A.quitarUltFila();

	//PCA pca_ = PCA(A, labels, vecinos, alpha);

	Matriz T = cargarImagenesTest(archivoEntrada, params);

	//labels = T.ultFila();
	//T.quitarUltFila();

	//vector<pair<int, int> > clasificadas = clasificarImagenes(T, labels, pca);

	//salida = pca_.autovalores();

	cout << "salida" << endl;
	return 0;
}