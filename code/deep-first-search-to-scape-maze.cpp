#include <stdlib.h>  // Funcion exit
#include <string.h>  // Funcion memset
#include <iostream>  // Variables cin y cout
#include <sstream> // separador de cadenas
#include <list> // Listas de adyacencia
#include <queue>  // Cola con el recorrido realizado
using namespace std;
#define MAX_NODOS 20000


//////////////////////////////////////////////////////////////
////////////        VARIABLES GLOBALES        ////////////////
//////////////////////////////////////////////////////////////

int numNodos; // Número de nodos del grafo
list<int> G[MAX_NODOS]; // Array de listas de adyacencia
bool visitados[MAX_NODOS];  // Marcas de nodos visitados
bool llegadoFinal = false;  // Indicador de si hemos podido llegar al final del laberinto
queue <int> recorrido;  // Cola que almacena el recorrido realizado


//////////////////////////////////////////////////////////////
////////////     FUNCIONES DEL PROGRAMA       ////////////////
//////////////////////////////////////////////////////////////

// PROCEDIMIENTO PARA LEER UN GRAFO DE LA ENTRADA
void leeGrafo (void){
  memset( visitados, 0, sizeof(visitados) );  // inicializamos a '0' todas las posiciones del array visitados
  cin >> numNodos;  // leemos número de nodos de este caso
  string linea; // variable para almacenar las lineas leídas
  getline(cin, linea);  // consumimos el resto de la línea
  if (numNodos < 0 || numNodos > MAX_NODOS) { // si el numero de nodos es negativo o mayor que el máximo
    cerr << "Número de nodos (" << numNodos << ") no válido" << endl; // imprimimos mensaje de error
    exit(0);  // abortamos el programa
  }
  else{ // si el número de nodos es válido
    for(int v = 0; v < numNodos; ++v) {  // para todos los nodos de este caso
      getline(cin, linea);  // leemos sus nodos adyacentes
      istringstream nodosAdyacentes(linea);
      int w;
      while(nodosAdyacentes >> w) // para cada nodo adyacente
        G[v].push_back(w-1); // lo inserteamos en su celda correspondiente
    }
  }
}

// PROCEDIMIENTO RECURSIVO PARA ESCAPAR DEL LABERINTO USANDO LA ESTRATEGIA DE GARBANCITO PERO UN POCO MÁS REFINADA 
void bgr(int v){
  // CASO BASE
  if(v == numNodos-1){  // hemos llegado al final del laberinto
    visitados[v] = true;  // ponemos el nodo como visitado
    recorrido.push(v);  // añadimos el nodo a nuestro recorrido
    llegadoFinal = true;
    return;
  }
  // CASO GENERAL
  visitados[v] = true;  // ponemos el nodo como visitado
  recorrido.push(v);  // añadimos el nodo a nuestro recorrido
  list<int>::iterator it = G[v].begin();
  while( it != G[v].end() && (!llegadoFinal) ){  // mientras haya nodos adyacentes y no haya llegado al final del laberinto
    if(!visitados[*it]){   // llamamos de nuevo a la función recursiva para el primer nodo adyacente que no haya sido visitado
      bgr(*it);
      if(!llegadoFinal)
      	recorrido.push(v);  // si no se consigue llegar al final del laberinto a través del adyacente probado, añadimos el nodo a nuestro recorrido (ya que hemos tenido que volver a él)
    }
    ++it; // lo intentamos con su siguiente adyacente
  }
}

// PROCEDIMIENTO PRINCIPAL PARA ESCAPAR DEL LABERINTO USANDO LA ESTRATEGIA DE GARBANCITO PERO UN POCO MÁS REFINADA 
void busquedaGarbancitoRefinada(void){
  llegadoFinal = false;
  bgr(0);
  if(llegadoFinal){ // hemos llegado al final del laberinto
    cout << recorrido.size() << endl;
    while( !recorrido.empty() ){  // mostramos y sacamos los nodos de la cola
      cout << recorrido.front() + 1 << endl;
      recorrido.pop();
    }
  }
  else{ // no hemos llegado al final del laberinto
    while( !recorrido.empty() )  // vaciamos la cola
      recorrido.pop();
    cout << "INFINITO" << endl;
  }
  for(int v = 0; v < MAX_NODOS; ++v)
    G[v].clear(); // vaciamos las listas de nuestro array
}


//////////////////////////////////////////////////////////////
////////////        PROGRAMA PRINCIPAL        ////////////////
//////////////////////////////////////////////////////////////

int main (void){
  int numCasos;
  cin >> numCasos;
  for(int i = 1; i <= numCasos; ++i) {
    leeGrafo();
    cout << "Caso " << i << endl;
    busquedaGarbancitoRefinada();
  }
}