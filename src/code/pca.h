#include "matriz.h"
#include <iostream>

using namespace std;

class PCA {

    public:
        PCA();
        PCA(Matriz& imagenes, vector<int>& labels, int vecinos, int alfa);
        vector<double> getAutovalores();
        int clasificar(vector<double>& imagen, int metodo);

    private:
        vector<double> autovalores;
        vector< vector<double> > autovectores;
        vector< vector<double> > imagenesTransformadas;
        int alfa;
        int vecinos;
        vector<int> labels;

        //metodos privados auxiliares
        Matriz calcularX(Matriz& imagenes, vector<double>& media);
        vector<double> calcularMedia(Matriz& M);
        vector<double> tc(vector<double>& imagen);

        double distancia(vector<double>& xprima, vector<double>& imagenTransformada);
        int metodo1(vector<double>& xprima);
        int metodo2(vector<double>& xprima, int vecinos);
};
