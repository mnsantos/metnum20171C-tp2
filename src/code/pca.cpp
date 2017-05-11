#include "pca.h"

using namespace std;

vector<double> restarVectores(vector<double>& a, vector<double>& b) {
    vector<double> res;
    for (int i=0; i<a.size(); i++) {
            res.push_back(a[i] - b[i]);
    }
    return res;
}

double productoInterno(vector<double>& u, vector<double>& v) {
    double sum = 0.0;
    for (unsigned int i = 0; i < u.size(); i++) {
            sum += u[i] * v[i];
    }
    return sum;
}

double norma2(vector<double>& a) {
    double sum = 0.0;
    for (int i=0; i<a.size(); i++) {
        sum = sum + pow(a[i],2);
    }
    return sqrt(sum);
}

vector<double> PCA::calcularMedia(Matriz& M) {
    vector<double> medias;
    for(int i=0; i<M.columnas(); i++){
        double media = 0;
        for(int j=0; j<M.filas(); j++){
            media += M[j][i];
        }
        medias.push_back(media / M.filas());
    }
    return medias;
}

Matriz PCA::calcularX(Matriz& imagenes, vector<double>& media) {
    int n = imagenes.filas();
    int m = imagenes.columnas();
    Matriz X = Matriz(n, m);
    for(int i=0; i<n; i++){
        X[i] = restarVectores(imagenes[i], media);
        for (int h=0; h<X[i].size(); h++){
            X[i][h] = X[i][h] / sqrt(n-1);
        }
    }
    return X;
}

vector<double> PCA::autovalores() {
    return autovalores;
}

PCA::PCA(Matriz& imagenes, vector<double>& labels, int vecinos, int alfa) {
    vector<double> media = calcularMedia(imagenes);
    Matriz X = calcularX(imagenes, media);
    Matriz X_t = X.trasponer();
    Matriz M = X_t * X;
    pair<vector<double>, vector<vector<double> > > pair = M.calcularAutovectores();
    this -> autovalores = pair.first;
    this -> autovectores = pair.second;
    this -> alfa = alfa;
    this -> labels = labels;
    this -> vecinos = vecinos;
    //aplico transformacion caracteristica a cada una de las imagenes de base
    for (int j=0; j<imagenes.size(); j++){
        imagenesTransformadas.push_back(tc(imagenes[j]));
    }
}

vector<double> PCA::tc(vector<double>& imagen) {
    int n = autovectores.size();
    vector<double> Y;
    for (int i = 0; i < n; ++i) {
        Y.push_back(productoInterno(autovectores[i], imagen));
    }
    return Y;
}

int PCA::clasificar(vector<double> &imagen, int metodo){
    vector<double> xprima = tc(imagen);
    int res;
    switch (metodo) {  
        case 1:  
            res = metodo1(xprima);  
            break;  
        case 2:  
            res = metodo2(xprima, vecinos);  
            break;
    } 
    return res;
}

// Me quedo con el mas cercano de todos...
int PCA::metodo1(vector<double>& xprima) {
    vector<double> distancias;
    for (int i=0; i<imagenesTransformadas; i++) {
        distancias.push_back(distancia(xprima, imagenesTransformadas[i]));
    }
    double min_distancia = 99999999
    double indice;
    for (int i=0; i<distancias.size(); i++) {
        if (distancias[i] < min_distancia) {
            min_distancia = distancias[i];
            indice = i;
        }
    }
    return labels[i];
}

// kNN
int PCA::metodo2(vector<double>& xprima, int vecinos) {
    //TODO: implementar metodo2
    vector<pair<double, int> > pairs;
    for (int i=0; i<imagenesTransformadas; i++) {
        pairs.push_back(make_pair(distancia(xprima, imagenesTransformadas[i]), labels[i]));
    }
    sort(pairs.begin(), pair.end());
    vector<double> labels;
    for (int i=0; i<vecinos; i++) {
        labels.push_back(pairs[i].second);
    }
    
}

double PCA::distancia(vector<double>& xprima, vector<double>& imagenTransformada) {
    return norma2(restarVectores(xprima, imagenTransformada));
}


/*

vector<int> indices;
    vector<double> distancias;
    buscar(vecinos, imagenesTransformadas, xprima, indices, distancias);
    int ret = votar(10, labels, indices, distancias);

*/