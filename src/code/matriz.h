#ifndef MATRIZ_H
#define MATRIZ_H
#include <vector>
#include <iostream>
#include "config.h"

using namespace std;

class Matriz {

private:
    vector< vector<double> > data;
    int cantFilas;
    int cantColumnas;
    vector<double> randomVector(unsigned int n);
    pair<double, vector<double> > metodoPotencia();
    Matriz deflacion(double autovalor, vector<double>& autovector);
    Matriz productoVec(vector<double>& v);
    static const int niter = 1000;
    static const double epsilon = 1;

public:
    //ruye una matriz de tamaño v * 1
    Matriz();
    Matriz(vector<double>& v);
    Matriz(int filas, int columnas);
    int filas();
    int columnas();
    void agregarFila(vector<double>& v);
    void quitarUltFila();
    vector<double> ultFila();
    Matriz cholesky() ;
    vector<double>& operator[] (int i);
    Matriz operator +(Matriz& m2);
    Matriz operator -(Matriz& m2);
    Matriz operator *(Matriz& m2);
    Matriz productoOptimizado(Matriz& m2);
    vector<double> operator *(vector<double>& v);
    Matriz operator *(double n);
    bool esTriangularSuperior();
    bool esTriangularInferior();
    Matriz trasponer();
    pair<vector<double>, vector<vector<double> > > calcularAutovectores(int alfa);
};

ostream& operator<<(ostream& os, Matriz& m);

#endif