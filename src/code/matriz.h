#ifndef MATRIZ_H
#define MATRIZ_H
#include <vector>
#include <iostream>

using namespace std;

class Matriz {

private:
    vector< vector<double> > data;
    int cantFilas;
    int cantColumnas;
    Matriz randomVector(unsigned int n);
    pair<double, Matriz> metodoPotencia();
    Matriz deflacion(double autovalor, Matriz autovector);
    static const int niter = 100;
    static const double epsilon = 1;

public:
    //construye una matriz de tamaño v * 1
    Matriz();
    Matriz(vector<double>& v);
    Matriz(int filas, int columnas);
    int filas();
    int columnas();
    Matriz cholesky();
    vector<double>& operator[] (int i);
    Matriz operator +(Matriz& m2);
    Matriz operator -(Matriz& m2);
    Matriz operator *(Matriz& m2);
    Matriz operator *(vector<double>& v);
    Matriz operator *(double n);
    bool esTriangularSuperior();
    bool esTriangularInferior();
    Matriz trasponer();
    pair<vector<double>, vector<vector<double> > > calcularAutovectores(int alfa);
};

ostream& operator<<(ostream& os, Matriz& m);

#endif