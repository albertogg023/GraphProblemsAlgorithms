#include <stdlib.h>  // Funcion exit
#include <string.h>  // Funcion memset
#include <iostream>  // Variables cin y cout
#include <queue>  // Cola de Nodos
using namespace std;
#define MAX_NODOS 26


//////////////////////////////////////////////////////////////
////////////        VARIABLES GLOBALES        ////////////////
//////////////////////////////////////////////////////////////

int numNodos; // Número de nodos del grafo
int numAristas; // Número de aristas del grafo
bool G[MAX_NODOS][MAX_NODOS]; // Matriz de adyacencia
bool visitados[MAX_NODOS];  // Array de marcas de nodos visitados


//////////////////////////////////////////////////////////////
////////////     FUNCIONES DEL PROGRAMA       ////////////////
//////////////////////////////////////////////////////////////

// PROCEDIMIENTO PARA LEER UN GRAFO DE LA ENTRADA
void leeGrafo (void){
  cin >> numNodos >> numAristas;
  if(numNodos < 0 || numNodos > MAX_NODOS) { // si el número de nodos es negativo o mayor que el máximo
    cerr << "Número de nodos (" << numNodos << ") no vúlido\n";
    exit(0);
  }
  memset( G, 0, sizeof(G) );  // inicializamos a '0' todas las celdas de la matriz
  char c1, c2;
  for(int i = 0; i < numAristas; i++) {
    cin >> c1 >> c2;
    G[c1-'A'][c2-'A']= true;
  }
}

// PROCEDIMIENTO ITERATIVO QUE REALIZA LA BÚSQUEDA EN PRIMERO EN ANCHURA DE UN NODO
void bpa(int v){
  queue<int> colaNodos;  // creamos una cola de nodos
  visitados[v]= true; 
  colaNodos.push(v);  // insertamos el primer nodo en la cola
  int nodoActual;
  while( !colaNodos.empty() ){
    nodoActual = colaNodos.front(); // nos guardamos el nodo el cual hay que imprimir tanto él como sus adyacentes
    colaNodos.pop();  // lo sacamos de la cola
    cout << char(nodoActual+'A');  // lo imprimimos
    for(int w = 0; w < numNodos; ++w){  // para cada nodo
      if( (!visitados[w]) && G[nodoActual][w] ){ // si no ha sido visitado y es adyacente
        visitados[w] = true;
        colaNodos.push(w);  // lo insertamos en la cola
      }
    }
  }
}

// PROCEDIMIENTO PRINCIPAL DE LA BUSQUEDA PRIMERO EN ANCHURA
void busquedaPA (void){
  memset( visitados, 0, sizeof(visitados) );  // inicializamos a '0' todas las posiciones del array visitados
  for(int v = 0; v < numNodos; ++v)
    if(!visitados[v])
      bpa(v);
  cout << endl;
}


//////////////////////////////////////////////////////////////
////////////        PROGRAMA PRINCIPAL        ////////////////
//////////////////////////////////////////////////////////////

int main (void){
  int numCasos;
  cin >> numCasos;
  for(int i = 0; i < numCasos; ++i) {
    leeGrafo();
    busquedaPA();
  }
}