#include "matriz.h"
#include <stdexcept>
#include <math.h>
#include <cmath>  
#include <cstdlib>

using namespace std;

extern Config CONFIG;
extern vector< vector<double> > TIEMPOS_AUTOVECTORES;
extern double CORRIDA_ACTUAL;

Matriz::Matriz(){
}

Matriz::Matriz(vector<double> &v) {
  cantFilas = 1;
  cantColumnas = v.size();
  data.push_back(v);
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

void Matriz::agregarFila(vector<double> & v){
  data.push_back(v);
  cantFilas++;
  cantColumnas = v.size();
}

void Matriz::quitarUltFila(){
  data.pop_back();
  cantFilas--;
}

vector<double> Matriz::ultFila(){
  return data.back();
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

vector<double>& Matriz:: operator [](int i){
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
      for(int k=0;k<this->columnas();k++){
        C[i][j] += data[i][k] * m2[k][j];
      }
    }
  }
  return C;
}

double producto(vector<double> &u, vector<double> &v) {
  double sum = 0;
  for (unsigned int i = 0; i < u.size(); i++) {
    sum += u[i] * v[i];
  }
  return sum;
}

Matriz Matriz::productoOptimizado(Matriz& m2) {
  unsigned int m = cantFilas;
  unsigned int n = cantColumnas;
  unsigned int o = m2.filas();
  unsigned int p = m2.columnas();
  Matriz C = Matriz(m, o);
  for(unsigned int i=0; i < m ; i++){
    for(unsigned int j=0; j < o ; j++){
      C[i][j] = producto(data[i],m2[j]);
    }
  }
  return C;
}

vector<double> Matriz:: operator *(vector<double>& v) {
  vector<double> res = vector<double>(v.size(),0);
  for(int i=0;i<this->filas();i++){
    res[i]=producto(data[i], v);
  }
  return res;
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

bool Matriz::esTriangularSuperior() {
  for (int i=0; i<this->filas(); i++){
    for (int j=0; j<i-1; j++){
      if (data[i][j] != 0){
        return false;
      }
    }
  }
  return true;
}

bool Matriz::esTriangularInferior() {
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

double norma2(vector<double>& v) {
  double sum = 0.0;
  for (int i=0; i<v.size(); i++) {
    sum = sum + pow(v[i],2);
  }
  return sqrt(sum);
}

double norma1(vector<double>& v) {
  double sum = 0.0;
  for (int i=0; i<v.size(); i++) {
    sum = sum + abs(v[i]);
  }
  return sum;
}

double normaInf(vector<double>& v) {
  double max = 0.0;
  for (int i=0; i<v.size(); i++) {
    if (max < abs(v[i])) {
      max = v[i];
    }
  }
  return max;
}


pair<double, vector<double> > Matriz::metodoPotencia() {
  //MetodoPotencia(B,x 0 ,niter)
  //cout << (*this) << endl;
  //v ← x0
  vector<double> v = randomVector(cantColumnas);
  //Para i = 1, . . . , niter
  for (int i = 0; i < niter; i++) {
    v = this->operator*(v);
    //cout << v << endl;
    //norma 2
    double norma = norma2(v);
    for (int j=0; j<v.size(); j++) {
      v[j] = v[j] / norma;
    }
  }

  //Matriz v_t = Matriz(v);
  //Matriz v1 = v_t.trasponer();
  vector<double> Axv = (*this) * v;
  double lambda = producto(v,Axv) / producto(v,v);

  //Devolver λ, v .
  return make_pair(lambda, v);
}

pair<vector<double>, vector<vector<double> > > Matriz::calcularAutovectores(int alfa)  {
  vector<double> autovalores;
  vector< vector<double> > autovectores;
  Matriz A = (*this);
  clock_t inicio, final;
  for (int i = 0; i < alfa; ++i) {
    if ((CONFIG.testEnabled && (i % CONFIG.saltoAlfa == 0)) || i==0) {
      //cout << "inicio clock" << endl; 
      inicio = clock();
    }
    cout << "calculando autovector " << i << endl;
    pair<double, vector<double> > tuple = A.metodoPotencia();
    autovalores.push_back(tuple.first);
    //cout << tuple.first << endl;
    autovectores.push_back(tuple.second);
    if (CONFIG.testEnabled && ((i % CONFIG.saltoAlfa) == (CONFIG.saltoAlfa - 1))){
      //cout << "fin clock" << endl; 
      final = clock();
      double total = (double(final - inicio) / CLOCKS_PER_SEC * 1000);
      //cout << "Total: " << total;
      //cout << "corrida acutal : " << CORRIDA_ACTUAL << endl;
      //cout << "size: " << TIEMPOS_AUTOVECTORES.size() << endl;
      TIEMPOS_AUTOVECTORES[CORRIDA_ACTUAL].push_back(total);
    }
    A = A.deflacion(tuple.first, tuple.second);
    //cout << A << endl;
  }
  return make_pair(autovalores, autovectores);
}

Matriz Matriz::productoVec(vector<double>& v) {
  int n = v.size();
  Matriz res = Matriz(n,n);
  for (int i=0; i<n; i++) {
    vector<double> v1 = vector<double>(n,0);
    for (int j=0; j<n; j++){
      v1[j] = v[i] * v[j];
    }
    res[i] = v1; 
  }
  return res;
}

Matriz Matriz::deflacion(double autovalor, vector<double>& autovector)  {
  //cout << autovector << endl;
  //cout << autovalor << endl;
  /*
  Matriz autovector_t = Matriz(autovector);
  Matriz autovectorM = autovector_t.trasponer();
  Matriz B = autovectorM * autovector_t;*/
  //cout<< B << endl;
  Matriz B = productoVec(autovector);
  Matriz BxAutovalor = B * autovalor;
  //cout << BxAutovalor << endl;
  return ((*this) - BxAutovalor);
}


vector<double> Matriz::randomVector(unsigned int n) {
  vector<double> v = vector<double>(n, 0);
  srand(clock());
  for (unsigned int i = 0; i < n; i++) {
    v[i] = (rand() % 100) / 100.0;
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
