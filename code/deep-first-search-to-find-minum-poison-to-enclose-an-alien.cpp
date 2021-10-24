#include <stdlib.h>  // Funcion exit 
#include <string.h>  // Funcion memset
#include <iostream>  // Variables cin y cout
using namespace std;
#define LARGO_MAX 1000  // Largo máximo de la celda
#define ANCHURA_MAX 1000  // Anchura máxima de la celda


//////////////////////////////////////////////////////////////
////////////        VARIABLES GLOBALES        ////////////////
//////////////////////////////////////////////////////////////

int anchuraCelda; // Ancho de la celda  
int largoCelda; // Largo de la celda
int alturaVeneno; // Altura a la que se encuentra el veneno
int G[ANCHURA_MAX+1][LARGO_MAX+1]; // Matriz de alturas de las casillas, ponemos LARGO_MAX+1 y ANCHURA_MAX+1 para que la gestión de los índices sea más fácil
bool visitadas[ANCHURA_MAX+1][LARGO_MAX+1]; // Matriz de marcas de casillas visitadas, ponemos LARGO_MAX+1 y ANCHURA_MAX+1 para que la gestión de los índices sea más fácil
int alturaMinima; // Altura de la casilla más baja por la que se ha pasado en el camino tomado para escapar


//////////////////////////////////////////////////////////////
////////////     FUNCIONES DEL PROGRAMA       ////////////////
//////////////////////////////////////////////////////////////

// PROCEDIMIENTO PARA LEER UN GRAFO DE LA ENTRADA
void leeGrafo(void){
  cin >> anchuraCelda >> largoCelda;  // leemos las dimensiones de la celda
  if(anchuraCelda < 0 || anchuraCelda > ANCHURA_MAX || largoCelda < 0 || largoCelda > LARGO_MAX) { // si la anchura o el largo no son válidos
    cerr << "Anchura o largo inválido" << endl; // imprimimos mensaje de error
    exit(0);  // abortamos el programa
  }
  // Leemos la altura de todas las casillas de la celda
  int h;
  for(int x = 1; x <= anchuraCelda; ++x)
    for(int y = 1; y <= largoCelda; ++y){
      cin >> h;
      G[x][y] = h;
    }
}

// PROCEDIMIENTO RECURSIVO DE LA BÚSQUEDA PRIMERO EN PROFUNDIDAD DE UNA CASILLA DADA PARA COMPROBAR SI SE PUEDE ESCAPAR DE LA CELDA
void bpp(int x, int y){
  if(visitadas[anchuraCelda][largoCelda]) return; // ha podido escapar de la celda
  visitadas[x][y] = true; // ponemos la casilla como visitada para evitar en el resto de llamadas visitarla de nuevo
  if(G[x][y] < alturaMinima) alturaMinima = G[x][y];  // si la altura de la casilla es más baja que la guardada como más baja, la actualizamos
  if(x == anchuraCelda && y == largoCelda) return;  // si la casilla es la salida de la celda
  // Probamos por el resto de casilla adyacentes (no diagonales) si: la casilla es válida, no ha sido probada, la diferencia de alturas difiere en '1' o '0', y el veneno no impide el paso
  if( !(x < 1 || x > anchuraCelda || y+1 < 1 || y+1 > largoCelda) && !visitadas[x][y+1] && abs(G[x][y] - G[x][y+1]) <= 1 && G[x][y+1] >= alturaVeneno )  bpp(x, y+1);  // arriba
  if( !(x+1 < 1 || x+1 > anchuraCelda || y < 1 || y > largoCelda) && !visitadas[x+1][y] && abs(G[x][y] - G[x+1][y]) <= 1 && G[x+1][y] >= alturaVeneno )  bpp(x+1, y);  // derecha
  if( !(x-1 < 1 || x-1 > anchuraCelda || y < 1 || y > largoCelda) && !visitadas[x-1][y] && abs(G[x][y] - G[x-1][y]) <= 1 && G[x-1][y] >= alturaVeneno )  bpp(x-1, y);  // izquierda
  if( !(x < 1 || x > anchuraCelda || y-1 < 1 || y-1 > largoCelda) && !visitadas[x][y-1] && abs(G[x][y] - G[x][y-1]) <= 1 && G[x][y-1] >= alturaVeneno )  bpp(x, y-1);  // abajo
}

// PROCEDIMIENTO QUE MEDIANTE LA BÚSQUEDA PRIMERO EN PROFUNDIDAD CALCULA LA ALTURA MÍNIMA A LA QUE HAY QUE SITUAR EL VENENO PARA QUE NO ESCAPE EL ALIEN
void alturaMinimaVeneno(int x, int y){
  alturaVeneno = 0; // por si sin veneno pudiese escapar
  visitadas[anchuraCelda][largoCelda] = true;  // ponemos inicialmente la salida de la celda como visitada para entrar al bucle
  while(visitadas[anchuraCelda][largoCelda]){ // mientras pueda escapar de la celda
    alturaMinima = G[x][y]; // la altura mínima es inicialmente la altura de la casilla inicial
    memset( visitadas, false, sizeof(visitadas) );  // inicializamos a 'false' todas las posiciones de la matriz visitadas
    if(G[x][y] >= alturaVeneno && G[anchuraCelda][largoCelda] >= alturaVeneno) bpp(x, y); // si el veneno no nos impide posicionarnos en la casilla inicial o en la salida de la celda
    if(visitadas[anchuraCelda][largoCelda])  alturaVeneno = alturaMinima + 1; // invalidamos el camino que ha tomado, poniendo el veneno a una altura más alta que su casilla más baja
  }
}


//////////////////////////////////////////////////////////////
////////////        PROGRAMA PRINCIPAL        ////////////////
//////////////////////////////////////////////////////////////

int main (void){
  int numCasos;
  cin >> numCasos;
  for(int i = 1; i <= numCasos; ++i){
    leeGrafo();
    alturaMinimaVeneno(1, 1); // comprobamos la altura mínima necesaria para hacer que no pueda escapar de la celda, estando situado inicialmente en la casilla (1,1)
    cout << "Caso #" << i << ": " << alturaVeneno << endl;
  }
}