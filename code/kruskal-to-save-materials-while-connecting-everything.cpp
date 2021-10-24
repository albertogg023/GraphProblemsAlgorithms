#include <stdlib.h>  // Función exit
#include <string.h>  // Función memset
#include <cmath>  // Funciones pow y floor
#include <iostream>  // Variables cin y cout
using namespace std;


//////////////////////////////////////////////////////////////
////////////           TIPOS DEFINIDOS         ///////////////
//////////////////////////////////////////////////////////////

struct punto{
  int x;
  int y;  
};


//////////////////////////////////////////////////////////////
////////////        VARIABLES GLOBALES        ////////////////
//////////////////////////////////////////////////////////////

int numNodos; // Número de nodos del grafo
punto * Coor; // Array con las coordenadas de cada nodo
int * MasCercano; // Array con el nodo más cercano a cada nodo
int * MenorCoste; // Array con el valor de la distancia mínima del nodo más cercano


//////////////////////////////////////////////////////////////
////////////     FUNCIONES DEL PROGRAMA       ////////////////
//////////////////////////////////////////////////////////////

// PROCEDIMIENTO QUE LEE LAS COORDENADAS DE CADA UNO DE LOS NODOS DEL GRAFO DE LA ENTRADA
void leerGrafo (void){
  cin >> numNodos;
  if(numNodos < 0){ // si el número de nodos incorrectos
    cerr << "Número de nodos inválido" << endl; // imprimimos mensaje de error
    exit(0);  // abortamos el programa
  }
  // Creamos un array en memoria dinámica que almacena las coordenadas de cada nodo
  Coor = new punto[numNodos+1]; // reservamos 'numNodos+1' posiciones para que la gestión de los índices sea más fácil
  int x, y;
  for(int i = 1; i <= numNodos; ++i){
    cin >> x >> y;
    Coor[i] = punto{x, y};
  }
}

// FUNCIÓN QUE DEVUELVE LA DISTANCIA EUCLIDEA ENTRE DOS NODOS
int calculaDistanciaEuclidia(punto punto1, punto punto2){
  return floor( pow(pow(punto1.x-punto2.x, 2) + pow(punto1.y-punto2.y, 2), 0.5) );
}

// PROCEDIMIENTO QUE INICIALIZA LAS VARIABLES PARA EL ALGORITMO DE PRIM
void inicializaPrim(int nodoComienzo){
  MasCercano = new int[numNodos+1]; // reservamos 'numNodos+1' posiciones para que la gestión de los índices sea más fácil
  MenorCoste = new int[numNodos+1]; // reservamos 'numNodos+1' posiciones para que la gestión de los índices sea más fácil
  for(int v = 1; v <= numNodos; ++v){
    MasCercano[v] = nodoComienzo; // inicialmente todos los nodos tienen como más cercano a nodoComienzo
    MenorCoste[v] = calculaDistanciaEuclidia(Coor[nodoComienzo], Coor[v]);  // inicialmente todos los nodos tienen como distancia mínima, la distancia entre el nodo y nodoComienzo
  }
  MenorCoste[nodoComienzo] = -1;  // lo añadimos a la solución, haciéndolo '-1' para evitar volver a cogerlo
}

// FUNCIÓN QUE DEVUELVE EL NODO QUE NO HAYA SIDO CÓGIDO Y QUE SE ENCUENTRE A MENOR DISTANCIA DE OTRO
int obtenerNodoDistanciaMinima(){
  int nodoDistanciaMin = 0;	// seleccionamos un nodo mínimo inicial no válido para sustituirlo en el bucle cuando una celda cumpla la condición
  int distanciaMin = 100000000;	// seleccionamos un valor mínimo inicial muy grande, para que el nodo mínimo inicial sea fácilmente sustitutible
  for(int v = 1; v <= numNodos; ++v)
    if(MenorCoste[v] != -1 && MenorCoste[v] < distanciaMin){ // si no ha sido cogido ya y su distancia es menor al mínimo anterior
      nodoDistanciaMin = v;
      distanciaMin = MenorCoste[v];
    }
  MenorCoste[nodoDistanciaMin] = -1;  // lo añadimos a la solución, haciéndolo '-1' para evitar volver a cogerlo
  return nodoDistanciaMin;
}

// PROCEDIMIENTO QUE EJECUTA EL ALGORITMO DE PRIM
void prim(int nodoComienzo){
  inicializaPrim(nodoComienzo);
  int v;
  for(int i = 2; i <= numNodos; ++i){ // lo ejecutamos numNodos-1 veces (ya que inicialmente el nodoComienzo está en la solución)
    v = obtenerNodoDistanciaMinima();  // obtenemos el nodo con la distancia mínima a otro nodo que no esté en la solución
    // Para cada nodo que no está en la solución recalculamos sus distancias mínimas y sus nodos más cercanos
    for(int w = 1; w <= numNodos; ++w)
      if( MenorCoste[w] != 1 && calculaDistanciaEuclidia(Coor[v], Coor[w]) < MenorCoste[w]){  // si el nodo no está en la solución y la distancia desde el último nodo añadido a él es menor que su anterior menor distancia
        MasCercano[w] = v;  // su nodo más cercano ahora es el último nodo añadido a la solución
        MenorCoste[w] = calculaDistanciaEuclidia(Coor[v], Coor[w]); // su distancia mínima ahora es el la que hay entre último nodo añadido a la solución y él
      }
  }
}

// FUNCIÓN QUE DEVUELVE EL COSTE TOTAL DEL ÁRBOL DE EXPANSIÓN MÍNIMO DE UN GRAFO A TRAVÉS DEL ALGORITMO DE PRIM
int costeArbolExpansionMinimo(int nodoComienzo){
  prim(nodoComienzo);
  int costeTotal = 0; // coste total del árbol de expansión mínimo
  for(int v = 1; v <= numNodos; ++v)  // para cada nodo
    costeTotal += calculaDistanciaEuclidia(Coor[v], Coor[MasCercano[v]]); // sumamos el coste de la arista que lo une con su más cercano
  delete[] MasCercano;  // liberamos el array usado para el algoritmo de Prim
  delete[] MenorCoste;  // liberamos el array usado para el algoritmo de Prim
  return costeTotal;
}


//////////////////////////////////////////////////////////////
////////////        PROGRAMA PRINCIPAL        ////////////////
//////////////////////////////////////////////////////////////

int main (void){
  int numCasos;
  cin >> numCasos;
  for(int i = 1; i <= numCasos; ++i){
   	leerGrafo();  // leemos los datos de entrada del caso
    cout << costeArbolExpansionMinimo(1) << endl; // imprimimos el coste del árbol de expansión mínimo de un grafo empezando por el nodo '1'
    delete[] Coor;  // liberamos el array con las coordenadas de los nodos
  }
}