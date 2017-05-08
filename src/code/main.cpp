#include "matriz.h"
#include <stdlib.h>

using namespace std;


int main(int argc, char **argv) {
  Matriz m = Matriz(3,3);
  m[0][0] = -4;
  m[0][1] = -3;
  m[0][2] = -3;

  m[1][0] = 0;
  m[1][1] = -1;
  m[1][2] = 0;

  m[2][0] = 6;
  m[2][1] = 6;
  m[2][2] = 5;

  //cout<<m<<endl;
  double res = m.calcularAutovectores(3).first[2];
  cout << res << endl;
 
}

