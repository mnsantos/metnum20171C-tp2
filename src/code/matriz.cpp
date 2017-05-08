#include "matriz.h"
#include <stdexcept>
#include <math.h>
#include <cmath>  
#include <cstdlib>

using namespace std;

Matriz::Matriz(){
}

Matriz::Matriz(vector<double> &v) {
  cantFilas = v.size();
  cantColumnas = 1;
  for (int i=0; i<cantFilas; i++){
    data.push_back(vector<double>(1,v[i]));
  }
}

Matriz::Matriz(int filas, int columnas) {
  cantFilas = filas;
  cantColumnas = columnas;
  vector<double> fila = vector<double>(columnas,0);
  for(int i=0; i<filas; i++){
    data.push_back(fila);
  }
}

int Matriz::filas() {
  return cantFilas;
}

int Matriz::columnas() {
  return cantColumnas;
}

Matriz Matriz::trasponer() {
  Matriz C = Matriz(cantColumnas, cantFilas);
  for (int i=0; i<cantColumnas; i++){
    for (int j=0; j<cantFilas; j++){
      C[i][j] = data[j][i];
    }
  }
  return C;
}

vector<double>& Matriz:: operator [](int i) {
  return data[i];
}

Matriz Matriz:: operator +(Matriz& m2) {
  if (this -> filas() != m2.filas() || this -> columnas() != m2.columnas()){
    throw runtime_error("Las matrices deben ser del mismo tamaño");
  }
  Matriz C = Matriz(this -> filas(), this -> columnas());
  for (int i=0; i<this -> filas(); i++){
    for (int j=0; j<this -> columnas(); j++){
      C[i][j] = data[i][j] + m2[i][j];
    }
  }
  return C;
}

Matriz Matriz:: operator -(Matriz& m2) {
  if (this -> filas() != m2.filas() || this -> columnas() != m2.columnas()){
    throw runtime_error("Las matrices deben ser del mismo tamaño");
  }
  Matriz C = Matriz(this -> filas(), this -> columnas());
  for (int i=0; i<this -> filas(); i++){
    for (int j=0; j<this -> columnas(); j++){
      C[i][j] = data[i][j] - m2[i][j];
    }
  }
  return C;
}

Matriz Matriz:: operator *(Matriz& m2) {
  if (this -> columnas() != m2.filas()){
    throw runtime_error("No se pueden multiplicar matrices de estos tamaños");
  }

  Matriz C = Matriz(this -> filas(), m2.columnas());
  for(int i=0;i<this->filas();i++){
    for(int j=0;j<m2.columnas();j++){
      C[i][j]=0;
      for(int k=0;k<this->columnas();k++){
        C[i][j]=C[i][j]+(data[i][k] * m2[k][j]);
      }
    }
  }
  return C;
}

Matriz Matriz:: operator *(vector<double>& v) {
  if (this -> columnas() != v.size()){
    throw runtime_error("No se puede multiplicar esta matriz por el vector recibido como parametro");
  }

  Matriz C = Matriz(this -> filas(), 1);
  for(int i=0;i<this->filas();i++){
    C[i][0]=0;
    for(int k=0;k<this->columnas();k++){
      C[i][0]=C[i][0]+(data[i][k] * v[k]);
    }
  }
  return C;
}

Matriz Matriz:: operator *(double n) {
  Matriz C = Matriz(this -> filas(), this->columnas());
  for(int i=0;i<this->filas();i++){
    for(int j=0;j<this->columnas();j++){
      C[i][j] = data[i][j] * n;
    }
  }
  return C;
}

bool Matriz::esTriangularSuperior(){
  for (int i=0; i<this->filas(); i++){
    for (int j=0; j<i-1; j++){
      if (data[i][j] != 0){
        return false;
      }
    }
  }
  return true;
}

bool Matriz::esTriangularInferior(){
  for (int i=0; i<cantFilas; i++){
    for (int j=i+1; j<cantColumnas; j++){
      if (data[i][j] != 0){
        return false;
      }
    }
  }
  return true;
}

Matriz Matriz::cholesky() {
  Matriz L = Matriz(cantFilas, cantColumnas);
  L[0][0] = sqrt(data[0][0]);
  for (int i=1; i<cantFilas; i++){
    L[i][0] = data[i][0] / L[0][0];
  }
  for (int j=1; j<cantFilas; j++){
    double acum1 = 0;
    for (int k=0; k<j; k++){
      acum1 = acum1 + pow(L[j][k], 2.0);
    }
    L[j][j] = sqrt(data[j][j] - acum1);
    for (int i=j+1; i<cantFilas; i++){
      double acum2 = 0;
      for (int k=0; k<j; k++){
        acum2 = acum2 + (L[i][k] * L[j][k]);
      }
      L[i][j] = (data[i][j] - acum2) / L[j][j];
    }
  }
  return L;
}

double norma2(Matriz& v) {
  double sum = 0.0;
  for (int i=0; i<v.filas(); i++) {
    sum = sum + pow(v[i][0],2);
  }
  return sqrt(sum);
}

double norma1(Matriz& v) {
  double sum = 0.0;
  for (int i=0; i<v.filas(); i++) {
    sum = sum + abs(v[i][0]);
  }
  return sum;
}

double normaInf(Matriz& v) {
  double max = 0.0;
  for (int i=0; i<v.filas(); i++) {
    if (max < abs(v[i][0])) {
      max = v[i][0];
    }
  }
  return max;
}


pair<double, Matriz> Matriz::metodoPotencia() {
  //MetodoPotencia(B,x 0 ,niter)
  //cout << (*this) << endl;
  //v ← x0
  Matriz v = randomVector(cantColumnas);
  //Para i = 1, . . . , niter
  for (int i = 0; i < niter; i++) {
    v = this->operator*(v);
    //cout << v << endl;
    //norma 2
    double norma = norma2(v);
    for (int j=0; j<v.filas(); j++) {
      v[j][0] = v[j][0] / norma;
    }
  }

  Matriz v_t = v.trasponer();
  double lambda = (v_t * (*this) * v)[0][0] / (v_t * v)[0][0];

  //Devolver λ, v .
  return make_pair(lambda, v);
}

pair<vector<double>, vector<vector<double> > > Matriz::calcularAutovectores(int alfa) {
  vector<double> autovalores;
  vector< vector<double> > autovectores;
  Matriz A = (*this);
  for (int i = 0; i < alfa; ++i) {
    pair<double, Matriz> tuple = A.metodoPotencia();
    autovalores.push_back(tuple.first);
    //cout << tuple.first << endl;
    autovectores.push_back(tuple.second.trasponer()[0]);
    A = A.deflacion(tuple.first, tuple.second);
    //cout << A << endl;
  }
  return make_pair(autovalores, autovectores);
}

Matriz Matriz::deflacion(double autovalor, Matriz autovector) {
  //cout << autovector << endl;
  //cout << autovalor << endl;
  Matriz autovector_t = autovector.trasponer();
  //cout << autovector_t << endl;
  Matriz B = autovector * autovector_t;
  //cout<< B << endl;
  Matriz BxAutovalor = B * autovalor;
  //cout << BxAutovalor << endl;
  return ((*this) - BxAutovalor);
}


Matriz Matriz::randomVector(unsigned int n) {
  Matriz v = Matriz(n, 1);
  srand(clock());
  for (unsigned int i = 0; i < n; i++) {
    v[i][0] = (rand() % 100) / 100.0;
  }
  return v;
}

ostream& operator<<(ostream& os, Matriz& m) {
  for (int i=0; i<m.filas(); i++){
    for (int j=0; j<m.columnas(); j++){
      os << m[i][j] << " ";
    }
    os << "\n";
  }
  return os;
}
