#include "basetrain.h"

using namespace std;

BaseTrain::BaseTrain(){}

BaseTrain::BaseTrain(string path){
	//leer desde archivo(s)
	//md5 del contenido del archivo de entrada para buscar si existe
}

BaseTrain::BaseTrain(Parametros params, Matriz autovalores, Matriz autovectores){
	this-params = params;
	this->autovalores = autovalores;
	this->autovectores = autovectores;
}

void BaseTrain::guardar(string path){
	//guardar en archivo(s)
	//md5 del contenido del archivo de entrada como nombre
}

void BaseTrain::agregarImagen(vector<int> imagen, int id){
	vector<double> t = transformarImagen(imagen);
	base.push_back(t);
	ids.push_back(id);
}

vector<double> BaseTrain::transformarImagen(vector<int> imagen){
	vector<double> t;

	return t;
}

int BaseTrain::buscar(vector<int> imagen){
	vector<double> t = transformarImagen(imagen);
	return knn(t);
}