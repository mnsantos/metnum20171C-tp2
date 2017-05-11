#include "matriz.h"
#include <iostream>

using namespace std;

class PCA {

    public:
        PCA();
        PCA(Matriz& imagenes, vector<double>& labels, int vecinos, int alfa);
        vector<double> autovalores();
        int clasificar(vector<double>& imagen, int metodo);

    private:
        vector<double> autovalores;
        vector< vector<double> > autovectores;
        vector< vector<double> > imagenesTransformadas;
        int alfa;
        int vecinos;
        vector<double> labels;

        //metodos privados auxiliares
        Matriz calcularX(Matriz& imagenes);
        vector<double> calcularMedia(Matriz& M);
        vector<double> tc(vector<double>& imagen);

        int metodo1(vector<double>& xprima);
        int metodo2(vector<double>& xprima);


};
