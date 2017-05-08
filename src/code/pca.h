#include "matriz.h"
#include <iostream>

using namespace std;

class PCA {

    public:
        PCA();
        PCA(Matriz& imagenes, vector<double>& labels, int vecinos, int alfa);
        int clasificar(vectorReal &v);

    private:
        vector<double> autovectores;
        vector< vector<double> > autovalores;
        Matriz imagenesTransformadas;
        int alfa;
        int vecinos;
        vector<double> labels;

        //metodos privados auxiliares
        Matriz calcularX(Matriz& imagenes);


};
