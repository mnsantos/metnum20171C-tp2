#include "parametros.h"
#include "pca.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <iomanip>
#include <map>

using namespace std;

struct Key {
	int alfa;
	int metodo;
	int vecinos;

	Key(int a, int m, int v) {
		alfa = a;
		metodo = m;
		vecinos = v;
	}
};

bool operator<(const Key& l, const Key& r) {
	return (l.alfa < r.alfa || (l.alfa == r.alfa && l.metodo < r.metodo) || (l.alfa == r.alfa && l.metodo == r.metodo && l.vecinos < r.vecinos));
}

double CORRIDA_ACTUAL = 0;
Config CONFIG;
double TIEMPO_ARMADO;
vector< vector<double> > TIEMPOS_AUTOVECTORES;
vector< vector<double> > TIEMPOS_TRANSFORMACIONES;
map<Key,vector<double> > TIEMPOS_CLASIFICACIONES;

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
		for (string line; getline(archivoEntrada, line); ){
			// process "line"
			stringstream ss(line);
			string token;

			while(getline(ss, token, ',')) {
				vectorImagen.push_back(atoi(token.c_str()));
			}
		}

	}
	return vectorImagen;
}

Matriz cargarImagenesTrain(ifstream &archivoEntrada, Parametros params, vector<int>* labels){
	Matriz imagenesTrain = Matriz(0,0);
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
	Matriz imagenesTest = Matriz(0,0);
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

void leerConfig() {
	ifstream archivoEntrada;
	archivoEntrada.open("config/config.txt");
	if(archivoEntrada.fail()){
		cout << "No se pudo cargar la configuracion" << endl;
		exit(0);
	}
	archivoEntrada >> CONFIG.testEnabled;
	archivoEntrada >> CONFIG.corridas;
	archivoEntrada >> CONFIG.saltoAlfa;
	archivoEntrada >> CONFIG.vecinos;
	archivoEntrada >> CONFIG.saltoVecinos;
	archivoEntrada >> CONFIG.metodoAutovectores;
	//incializando vectores
	vector<double> initVal1;
	vector<double> initVal2;
	TIEMPOS_AUTOVECTORES.resize(CONFIG.corridas, initVal1);
	TIEMPOS_TRANSFORMACIONES.resize(CONFIG.corridas, initVal2);

	//cout << "size: " << TIEMPOS_AUTOVECTORES[0].size() << endl;
}


void escribirSalida(vector<double>& salida, ofstream& archivoSalida) {
	for (int i=0; i<salida.size(); i++) {
		archivoSalida << setprecision(6) << fixed << sqrt(salida[i]) << endl;
	}
}

map<Key,vector<pair<int, int> > > clasificarImagenes(Matriz& imagenes, vector<int>& labels, PCA & pca, int alfa){
	map<Key,vector<pair<int, int> > > resultados;
	clock_t inicio, final;
	int alfas = alfa / CONFIG.saltoAlfa;
	for (int j=0; j<imagenes.filas(); j++){
		for (int h=1; h<=alfas; h++){
			for (int metodo=1; metodo<=2; metodo++) {
				for (int vecinos=1; vecinos<=CONFIG.vecinos; vecinos+=CONFIG.saltoVecinos) {
					for (int corrida_actual=0; corrida_actual<CONFIG.corridas; corrida_actual++) {
						inicio = clock();
						int alfa_actual = h*CONFIG.saltoAlfa;
						Key key = Key(alfa_actual, metodo, vecinos);
						//cout << "Clasificando alfa=" << alfa_actual << ", metodo=" << metodo << ", vecinos=" << vecinos << endl;
						int asignado;
						if (metodo==1) {
							asignado = pca.clasificarUsandoMetodo1(imagenes[j], alfa_actual, vecinos);
						} else if (metodo==2) {
							asignado = pca.clasificarUsandoMetodo2(imagenes[j], alfa_actual, vecinos);
						}
						final = clock();
						double total = (double(final - inicio) / CLOCKS_PER_SEC * 1000);
						int label = labels[j];
						pair<int, int> res = make_pair(label, asignado);
						resultados[key].push_back(res);	
						if (j==imagenes.filas()-1) {
							TIEMPOS_CLASIFICACIONES[key].push_back(total);
						}
					}	
				}			
			}
		}
	}
	return resultados;
}

void escribirResultados(map<Key,vector<pair<int, int> > >& resultados, ofstream& archivoSalida, int cantImagenesTest, int alfa) {
	int alfas = alfa / CONFIG.saltoAlfa;
	archivoSalida << alfas << " " << CONFIG.vecinos << " " << cantImagenesTest << endl;

	//calculando tiempos
	vector<double> tiempoAutovectores;
	vector<double> tiempoTransformaciones;		
	for (int j=0; j<alfas; j++) {
		double sumAutovectores = 0;
		double sumTransformaciones = 0;
		for (int corrida_actual=0; corrida_actual<CONFIG.corridas; corrida_actual++) {
			sumAutovectores += TIEMPOS_AUTOVECTORES[corrida_actual][j];
			sumTransformaciones += TIEMPOS_TRANSFORMACIONES[corrida_actual][j];
		}
		tiempoAutovectores.push_back(sumAutovectores / CONFIG.corridas);
		tiempoTransformaciones.push_back(sumTransformaciones / CONFIG.corridas);
	}
	double tiempoAutovectoresAcum = 0;

	for (int h=0; h<alfas; h++) {
		int alfa_actual = (h+1)*CONFIG.saltoAlfa;
		archivoSalida << alfa_actual << endl;

		/*cout << "Tiempos autovectores:" << endl;
      	for (int j=0; j<TIEMPOS_AUTOVECTORES[0].size(); j++) {
        	cout << TIEMPOS_AUTOVECTORES[0][j] <<  " ";
      	}*/

		double tiempoAutovectoresAlfa = tiempoAutovectores[h] + tiempoAutovectoresAcum;
		tiempoAutovectoresAcum += tiempoAutovectoresAlfa;
		archivoSalida << fixed << TIEMPO_ARMADO + tiempoAutovectoresAlfa << " " << tiempoTransformaciones[h] << endl;
		for (int vecinos=1; vecinos<=CONFIG.vecinos; vecinos+=CONFIG.saltoVecinos) {
			archivoSalida << vecinos << endl;
			Key key = Key(alfa_actual, 1, vecinos);
			double tiempoClasificaciones = 0;
			for (int corrida_actual=0; corrida_actual<CONFIG.corridas; corrida_actual++) {
				tiempoClasificaciones += TIEMPOS_CLASIFICACIONES[key][corrida_actual];
			}
			archivoSalida << tiempoClasificaciones / CONFIG.corridas << endl;
			for (int i=0; i<cantImagenesTest; i++) {
				archivoSalida << resultados[key][i].first << " " << resultados[key][i].second << endl;
			}
			key = Key(alfa_actual, 2, vecinos);
			tiempoClasificaciones = 0;
			for (int corrida_actual=0; corrida_actual<CONFIG.corridas; corrida_actual++) {
				tiempoClasificaciones += TIEMPOS_CLASIFICACIONES[key][corrida_actual];
			}
			archivoSalida << tiempoClasificaciones / CONFIG.corridas << endl;
			for (int i=0; i<cantImagenesTest; i++) {
				//cout << "Asignado por metodo 2 " << resultados[key][i].second << endl;
				archivoSalida << resultados[key][i].first << " " << resultados[key][i].second << endl;
			}
		}
	}
}

// PARA CORRER TESTS DE LA CATEDRA
void modo_normal(int argc, char const *argv[]) {
	cout << "corriendo en modo normal" << endl;
	string infile = argv[1];
	string outfile = argv[2];
	ifstream archivoEntrada;
	ofstream archivoSalida;
	archivoEntrada.open(infile.c_str());
  	archivoSalida.open(outfile.c_str());

	if(archivoEntrada.fail()){
		cout << "Falla Archivo de entrada" << endl;
		exit(0);
	}

	Parametros params = leerParametros(archivoEntrada, infile);

	vector<int>* labelsTrain = new vector<int>();
	
	Matriz imagenesTrain = cargarImagenesTrain(archivoEntrada, params, labelsTrain);

	PCA pca_ = PCA(imagenesTrain, (*labelsTrain), params.k, CONFIG.metodoAutovectores);
	vector<int>* labelsTest = new vector<int>();

	vector<double> autovalores = pca_.getAutovalores();
	vector<vector<double> > autovectores = pca_.getAutovectores();

	/*for(int i=0; i<autovalores.size(); i++) {
		cout << autovalores[i] << endl;
	}

	for(int i=0; i<autovectores.size(); i++) {
		for(int j=0; j<autovectores[i].size(); j++) {
			cout << autovectores[i][j] << " ";
		}
		cout << endl;
	}*/

	escribirSalida(autovalores, archivoSalida);

	delete labelsTrain;
	delete labelsTest;

}

// PARA EXPERIMENTACION
void modo_experimentacion(int argc, char const *argv[]) {
	cout << "corriendo en modo experimentacion" << endl;
	string infile = argv[1];
	string outfile = argv[2];
	ifstream archivoEntrada;
	ofstream archivoSalida;
	archivoEntrada.open(infile.c_str());
  	archivoSalida.open(outfile.c_str());

	if(archivoEntrada.fail()){
		cout << "Falla Archivo de entrada" << endl;
		exit(0);
	}

	Parametros params = leerParametros(archivoEntrada, infile);

	vector<int>* labelsTrain = new vector<int>();
	
	Matriz imagenesTrain = cargarImagenesTrain(archivoEntrada, params, labelsTrain);

	PCA pca_ = PCA(imagenesTrain, (*labelsTrain), params.k, CONFIG.metodoAutovectores);
	vector<int>* labelsTest = new vector<int>();

	Matriz imagenesTest = cargarImagenesTest(archivoEntrada, params, labelsTest);
	map<Key,vector<pair<int, int> > > resultados = clasificarImagenes(imagenesTest, (*labelsTest), pca_, params.k);

	cout << "escribiendo resultados..." << endl;

	escribirResultados(resultados, archivoSalida, imagenesTest.filas(), params.k);

	delete labelsTrain;
	delete labelsTest;
}


int main(int argc, char const *argv[]) {
	leerConfig();
	if (CONFIG.testEnabled) {
		modo_experimentacion(argc, argv);
	} else {
		modo_normal(argc, argv);
	}
	return 0;
}