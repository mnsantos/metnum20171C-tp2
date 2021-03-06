#include "matriz.h"
#include <iostream>
#include <map>

using namespace std;

class PCA {

    public:
        PCA();
        PCA(Matriz& imagenes, vector<int>& labels, int alfa, int metodoAutovectores);
        vector<double> getAutovalores();
        vector< vector<double> > getAutovectores();
        int clasificarUsandoMetodo1(vector<double>& imagen, int alfa, int vecinos);
        int clasificarUsandoMetodo2(vector<double>& imagen, int alfa, int vecinos);

    private:
        vector<double> autovalores;
        vector< vector<double> > autovectores;
        //vector< vector<double> > imagenesTransformadas;
        map<int, vector< vector<double> > > imagenesTransformadas;
        int alfa;
        vector<int> labels;
        map<int, double> distanciaLimite;

        //metodos privados auxiliares
        Matriz calcularX(Matriz& imagenes, vector<double>& media);
        vector<double> calcularMedia(Matriz& M);
        vector<double> tc(vector<double>& imagen, int alfa);
        double maxDistEntreImagenes(int alfa_actual);
        double maxDistPorPromedio(int alfa_actual);

        double distancia(vector<double>& xprima, vector<double>& imagenTransformada);
};
