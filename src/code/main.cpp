#include "parametros.h"
#include "basetrain.h"

int main(int argc, char const *argv[])
{
	ifstream &archivoEntrada;
	archivoEntrada.open(argv[1].c_str());
	if(archivoEntrada.fail()){
		cout << "Falla Archivo de entrada" << endl;
		exit(0);
	}

	Parametros params = leerParametros(ifstream &archivoEntrada);

	Matriz A = cargarImagenesTrain(ifstream &archivoEntrada, Parametros params);

	vector<int> Ids = A.back();
	A.pop_back();

	Matriz T = cargarImagenesTest(ifstream &archivoEntrada, Parametros params);

	//transformar imagenes con PCA

	//guardar transformacion

	//armar archivo de salida de autovalores

	//guardar transformaciones de las imagenes de entrada en base de entrenamiento

	//guardar base de entrenamiento en archivo
		
	//pasar imagenes test por base de entrenamiento

	//guardar resultados de imagenes test (clase correspondiente - clase asignada)

	return 0;
}

Parametros leerParametros(ifstream &archivoEntrada){
	Parametros params;
	params.infile = entrada;
	archivoEntrada >> params.path;
	archivoEntrada >> params.f;
	archivoEntrada >> params.c;
	archivoEntrada >> params.p;
	archivoEntrada >> params.nimgp;
	archivoEntrada >> params.k;
	return params;
}

Matriz cargarImagenesTrain(ifstream &archivoEntrada, Parametros params){
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
			vectorArchivoImagen = leerArchivoImagen(params.path + directorioPersona + numeroImagen);
			imagenesTrain.agregarFila(vectorArchivoImagen);
		}
	}
	return imagenesTrain;
}

Matriz cargarImagenesTest(ifstream &archivoEntrada, Parametros params){
	Matriz imagenesTest;
	string imagenTest;
	vector<int> sujetos;
	int sujeto;
	archivoEntrada >> params.ntest;
	for (int i = 0; i < ntest; ++i)
	{
		archivoEntrada >> imagenTest;
		archivoEntrada >> sujeto;
		vectorArchivoImagen = leerArchivoImagen(imagenTest);
		imagenesTest.agregarFila(vectorArchivoImagen);
		sujetos.push_back(sujeto);
	}
	imagenesTest.push_back(sujetos);
	return imagenesTest;
}

vector<int> leerArchivoImagen(string archivoImagen){
	ifstream &archivoEntrada;
	vector<int> vectorImagen = vector<int>(params.f * params.c);
	archivoEntrada.open(params.infile);
	if(archivoEntrada.fail()){
		cout << "Falla Archivo de imagen: " << archivoImagen << endl;
		exit(0);
	} else{
		archivoEntrada >> vectorImagen;
	}
	return vectorImagen;
}