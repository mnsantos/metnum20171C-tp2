#include "pca.h"
#include <math.h>
#include <map>
#include <algorithm>

using namespace std;

extern Config CONFIG;
extern double CORRIDA_ACTUAL;
extern vector< vector<double> > TIEMPOS_TRANSFORMACIONES;
extern vector< vector<double> > TIEMPOS_AUTOVECTORES;
extern vector< vector<double> > TIEMPOS_CLASIFICAR;
extern double TIEMPO_ARMADO;

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

vector<double> productoPorEscalar(vector<double>& v, double e) {
    vector<double> res;
    for (int i=0; i<v.size(); i++) {
        res.push_back(e*v[i]);
    }
    return res;
}

double norma_2(vector<double>& a) {
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
    //cout << "n " << n << " m " << m << endl;
    Matriz X = Matriz(n, m);
    for(int i=0; i<n; i++){
        X[i] = restarVectores(imagenes[i], media);
        for (int h=0; h<X[i].size(); h++){
            X[i][h] = X[i][h] / sqrt(n-1);
        }
    }
    return X;
}

vector<double> PCA::getAutovalores() {
    return autovalores;
}

vector< vector<double> > PCA::getAutovectores() {
    return autovectores;
}

PCA::PCA(Matriz& imagenes, vector<int>& labels, int alfa, int metodoAutovectores) {
    map<int, double> distanciaLimite;
    clock_t inicio, final;
    inicio = clock();
    cout << "calculando media" << endl;
    vector<double> media = calcularMedia(imagenes);
    cout << "calculando X" << endl;
    Matriz X = calcularX(imagenes, media);
    cout << "trasponiendo X" << endl;
    Matriz X_t = X.trasponer();
    Matriz M;
    if (metodoAutovectores == 1) {
        // Metodo lento
        cout << "multiplicando X_t * X" << endl;
        M = X_t.productoOptimizado(X_t);
    } else {
        // Metodo rapido
        cout << "multiplicando X * X_t" << endl;
        M = X.productoOptimizado(X);

    }
    cout << "calculando autovectores..." << endl;
    pair<vector<double>, vector<vector<double> > > pair;
    final = clock();
    double total = (double(final - inicio) / CLOCKS_PER_SEC * 1000);
    TIEMPO_ARMADO = total;
    for (int i=0; i<CONFIG.corridas; i++) {
        pair = M.calcularAutovectores(alfa);
        CORRIDA_ACTUAL ++;
    }

    CORRIDA_ACTUAL = 0;
    this -> autovalores = pair.first;
    this -> autovectores = pair.second;
    this -> alfa = alfa;
    this -> labels = labels;
    if (metodoAutovectores == 2) {
        int h = 0;
        for (int i = 0; i < alfa; ++i) {
            if ((CONFIG.testEnabled && (i % CONFIG.saltoAlfa == 0)) || i==0) {
              //cout << "inicio clock" << endl; 
              inicio = clock();
            }
            //cout << "transformando autovector " << i << endl;
            double e = sqrt(autovalores[i]) / autovalores[i];
            vector<double> v = X_t * autovectores[i];
            this -> autovectores[i] = productoPorEscalar(v,e);  
            
            if (CONFIG.testEnabled && ((i % CONFIG.saltoAlfa) == (CONFIG.saltoAlfa - 1))){
                final = clock();
                double total = (double(final - inicio) / CLOCKS_PER_SEC * 1000);
                TIEMPOS_AUTOVECTORES[CORRIDA_ACTUAL][h] += total;
                h++;
            }
        }
    }


    if (CONFIG.testEnabled) {
        int alfas = alfa / CONFIG.saltoAlfa;
        for (int i=0; i<CONFIG.corridas; i++) {
            for (int h=1; h<=alfas; h++) {
                int alfa_actual = h*CONFIG.saltoAlfa;
                //cout << "Transformando base de train para alfa=" << alfa_actual << endl;
                inicio = clock();
                for (int j=0; j<imagenes.filas(); j++){
                    //cout << "transformando imagen " <<  j << endl;
                    //aplico transformacion caracteristica a cada una de las imagenes de base
                    //cout << "Aplicando tc para alfa=" << alfa_actual << endl; 
                    imagenesTransformadas[alfa_actual].push_back(tc(imagenes[j], alfa_actual));
                } 
                final = clock();
                double total = (double(final - inicio) / CLOCKS_PER_SEC * 1000);
                //cout << "Total para alfa=" << alfa_actual << ": " << total << endl;
                TIEMPOS_TRANSFORMACIONES[CORRIDA_ACTUAL].push_back(total);
                //maxDistPorPromedio(alfa_actual);
            }
            CORRIDA_ACTUAL ++;
        }
    } else {
        for (int j=0; j<imagenes.filas(); j++){
            //aplico transformacion caracteristica a cada una de las imagenes de base
            imagenesTransformadas[alfa].push_back(tc(imagenes[j], alfa));
        }
        //maxDistPorPromedio(alfa);
    }
}

vector<double> PCA::tc(vector<double>& imagen, int alfa) {
    vector<double> Y;
    for (int i = 0; i < alfa; ++i) {
        Y.push_back(productoInterno(autovectores[i], imagen));
    }
    return Y;
}

// kNN
int PCA::clasificarUsandoMetodo1(vector<double> &imagen, int alfa, int vecinos) {
    vector<pair<double, int> > pairs;
    vector<double> xprima = tc(imagen, alfa);
    for (int i=0; i<imagenesTransformadas[alfa].size(); i++) {
        pairs.push_back(make_pair(distancia(xprima, imagenesTransformadas[alfa][i]), labels[i]));
    }
    sort(pairs.begin(), pairs.end());
/*    if (pairs[0].first > distanciaLimite[alfa] && pairs[0].first < (3.0/2.0) * distanciaLimite[alfa])
    {
        cout << "la cara no pertenece a la base" << endl;
        return -1;
    }
    if (pairs[0].first > (3.0/2.0) * distanciaLimite[alfa])
    {
        cout << "la imagen no es una cara" << endl;
        return -1;
    }*/
    //cout << "distancia de la imagen test: " << pairs[0].first << endl;
    map<int, int> reps;
    map<int, int>::iterator it;
    for (int i=0; i<vecinos; i++) {
        int label = pairs[i].second;
        it = reps.find(label);
        if (it == reps.end()) {
            reps[label] = 1;
        } else {
            reps[label] = reps[label] + 1; 
        }
    }
    int max_reps = 0;
    int max_label = -1;
    for (it=reps.begin(); it!=reps.end(); ++it) {
        if (it -> second > max_reps) {
            max_reps = it -> second;
            max_label = it -> first;
        }
    }
    //cout << "La imagen es: " << max_label << endl;
    return max_label;
}

// kNN ponderado
int PCA::clasificarUsandoMetodo2(vector<double> &imagen, int alfa, int vecinos) {
    vector<pair<double, int> > pairs;
    vector<double> xprima = tc(imagen, alfa);
    for (int i=0; i<imagenesTransformadas[alfa].size(); i++) {
        pairs.push_back(make_pair(distancia(xprima, imagenesTransformadas[alfa][i]), labels[i]));
    }
    sort(pairs.begin(), pairs.end());
/*    if (pairs[0].first > distanciaLimite[alfa] && pairs[0].first < (1.1) * distanciaLimite[alfa])
    {
        cout << "la cara no pertenece a la base" << endl;
        return -1;
    }
    if (pairs[0].first > (1.1) * distanciaLimite[alfa])
    {
        cout << "la imagen no es una cara" << endl;
        return -1;
    }*/
    //cout << "distancia de la imagen test: " << pairs[0].first << endl;
    map<int, double> weights;
    map<int, double>::iterator it;
    for (int i=0; i<vecinos; i++) {
        int label = pairs[i].second;
        it = weights.find(label);
        if (it == weights.end()) {
            weights[label] = 1 / pairs[i].first;
        } else {
            weights[label] += 1 / pairs[i].first; 
        }
    }
    double max_weigth = 0;
    int max_label = -1;
    for (it=weights.begin(); it!=weights.end(); ++it) {
        if (it -> second > max_weigth) {
            max_weigth = it -> second;
            max_label = it -> first;
        }
    }
    //cout << "La imagen es: " << max_label << endl;
    return max_label;
}

double PCA::distancia(vector<double>& xprima, vector<double>& imagenTransformada) {
    vector<double> resta = restarVectores(xprima, imagenTransformada);
    return norma_2(resta);
}

double PCA::maxDistEntreImagenes(int alfa_actual){
    double maxDist = 0;
    vector<vector<double> > imagenes = imagenesTransformadas[alfa_actual];
    for (int i = 0; i < imagenes.size(); ++i)
    {
        for (int j = 0; j < imagenes.size(); ++j)
        {
            double distanciaActual = distancia(imagenes[i], imagenes[j]);
            if (distanciaActual >= maxDist)
            {
                maxDist = distanciaActual;
                cout << "imagen 1: " << labels[i] << "imagen 2: " << labels[j] << endl;
            }
        }
    }
    //cout << "Distancia Límite: " << maxDist << endl;
    distanciaLimite[alfa_actual] = maxDist;
    return maxDist;
}

double PCA::maxDistPorPromedio(int alfa_actual){
    double maxDist = 0;
    vector<vector<double> > imagenes = imagenesTransformadas[alfa_actual];
    vector<double> imagenPromedio = vector<double>(imagenes[0].size());
    for (int i = 0; i < imagenes.size(); ++i)
    {
        for (int j = 0; j < imagenPromedio.size(); ++j)
        {
            imagenPromedio[j] += (imagenes[i][j] / imagenes.size());
        }
    }
    for (int j = 0; j < imagenes.size(); ++j)
        {
            double distanciaActual = distancia(imagenPromedio, imagenes[j]);
            maxDist += (distanciaActual / imagenes.size());
        }
    //cout << "Distancia Límite: " << maxDist << endl;
    distanciaLimite[alfa_actual] = maxDist;
    return maxDist;
}