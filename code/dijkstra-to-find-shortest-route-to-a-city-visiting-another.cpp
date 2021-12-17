#include <stdlib.h>  // Funcion exit
#include <iostream>  // Variables cin y cout
using namespace std;


//////////////////////////////////////////////////////////////
////////////        VARIABLES GLOBALES        ////////////////
//////////////////////////////////////////////////////////////

int numNodos; // Número de nodos del grafo   
int numAristas; // Número de aristas del grafo
int ** C; // Matriz de costes
int nodoComienzo; // Nodo por el que empezar
int nodoIntermedio; // Nodo por el que hay que pasar
int nodoDestino;  // Nodo al que llegar
int * D;  // Matriz de distancias del algoritmo de Dijkstra
int * P;  // Matriz de paso del algoritmo de Dijkstra
bool * S; // Matriz de nodos con solución del algoritmo de Dijkstra


//////////////////////////////////////////////////////////////
////////////     FUNCIONES DEL PROGRAMA       ////////////////
//////////////////////////////////////////////////////////////

// CREAMOS LA MATRIZ DE COSTES DEL GRAFO DE LA ENTRADA
void leerMatrizCostes (void){
  // Creamos una matriz para las aristas (si el peso es '-1' es que no existe conexión entre los nodos)
  C = new int*[numNodos+1]; // reservamos 'numNodos+1' posiciones para que la gestión de los índices sea más fácil
  for(int i = 1; i <= numNodos; ++i){
    C[i] = new int[numNodos+1]; // reservamos 'numNodos+1' posiciones para que la gestión de los índices sea más fácil
    for(int j = 1; j <= numNodos; ++j)
      C[i][j] = -1;  // inicializamos sus aristas a 'no existe conexión'
  }
  // Asignamos las aristas leidas de la entrada
  int v, w, p;
  for(int i = 1; i <= numAristas; ++i){
    cin >> v >> w >> p;
    C[v][w] = p;
    C[w][v] = p;
  }
}

// LEEMOS LOS DATOS DEL CASO
void leerDatos(void){
  cin >> numNodos;
  cin >> numAristas;
  cin >> nodoComienzo >> nodoDestino >> nodoIntermedio;
  if(numNodos < 0 || numAristas < 0 || nodoComienzo < 0 || nodoDestino < 0 || nodoIntermedio < 0){ // si alguno de los parámetros es incorrecto
    cerr << "Datos incorrectos, vuelva a introducir los datos por favor" << endl; // imprimimos mensaje de error
    exit(0);  // abortamos el programa
  }
  else leerMatrizCostes();
}

// INICIALIZAMOS LAS VARIABLES NECESARIAS PARA EL ALGORITMO DE DIJKSTRA
void inicializacionDijkstra(int nodoComienzo){
  for(int v = 1; v <= numNodos; ++v){
    S[v] = false; // nodos que se encuentran en la solución
    P[v] = nodoComienzo; // nodos de paso inicialmente son el nodo de comienzo
    D[v] = C[nodoComienzo][v]; // costes de caminos mínimos son inicialmente los costes entre el nodo y el nodo de comienzo
  }
  S[nodoComienzo] = true; // el nodo de comienzo se añade a la solución inicialmente
}

// OBTENEMOS EL NODO QUE TIENE LA DISTANCIA MÍNIMA DEL ARRAY DE DISTANCIAS USADO EN EL ALGORITMO DE DIJKSTRA
int obtenerNodoConDistanciaMinima(){
  int nodoMin = 1;	// seleccionamos un nodo mínimo inicial que puede ser no válido
  int valorMin = 100000000;	// seleccionamos un valor mínimo inicial muy grande, para que el nodo mínimo inicial sea fácilmente sustitutible
  for(int w = 1; w <= numNodos; ++w){ // procedemos a encontrar el mínimo de facto
    if( (!S[w]) && D[w] != -1 && D[w] < valorMin){
      valorMin = D[w];
      nodoMin = w;
    }
  }
  return nodoMin;
}

// EJECUTAMOS EL ALGORITMO DE DIJKSTRA PARA OBTENER EL COSTE DEL CAMINO MÍNIMO ENTRE DOS NODOS
int costeCaminoMinimoEntreParNodos(int nodoComienzo, int nodoDestino){
  inicializacionDijkstra(nodoComienzo); // inicializamos las variables necesarias
  int v;
  for(int i = 1; (!S[nodoDestino]) && i <= numNodos; ++i){  // véase que si obtenemos el camino del nodo a encontrar, detenemos el bucle
    v = obtenerNodoConDistanciaMinima();  // obtenemos el nodo con la distancia mínima calculada que no esté en la solución
    S[v] = true;  // lo añadimos a la solución
    // Para cada nodo adyacente a este recalculamos sus distancias en el caso de que mejoren
    for(int w = 1; w <= numNodos; ++w)
      if(C[v][w] != -1)  // si es adyacente
        if( (!S[w]) && ( D[w] == -1 || D[v] + C[v][w] < D[w]) ){ // si 'w' no está en la solución y el camino es más corto pasando por 'v' actualizamos
          D[w] = D[v] + C[v][w];
          P[w] = v;
        } 
  }
  if(!S[nodoDestino]) return -1;	// no hay camino entre el par de nodos
  else return D[nodoDestino];	// si hay camino
}


//////////////////////////////////////////////////////////////
////////////        PROGRAMA PRINCIPAL        ////////////////
//////////////////////////////////////////////////////////////

int main (void){
  int numCasos;
  cin >> numCasos;
  for(int i = 1; i <= numCasos; ++i){
   	leerDatos();  // leemos los datos de entrada del caso
    D = new int[numNodos+1];
  	P = new int[numNodos+1];
  	S = new bool[numNodos+1];
  	int costeMinParNodos = costeCaminoMinimoEntreParNodos(nodoComienzo, nodoIntermedio);
  	if(costeMinParNodos != -1){ // si hemos encontrado un camino entre el nodo de comienzo y el nodo intermedio
  		int costeMinTotal = costeMinParNodos;
  		costeMinParNodos = costeCaminoMinimoEntreParNodos(nodoIntermedio, nodoDestino);
  		if(costeMinParNodos != -1){ // si hemos encontrado un camino entre el nodo intermedio y el nodo destino
  			costeMinTotal += costeMinParNodos;
  			cout << costeMinTotal << endl;
  		}
  		else cout << "IMPOSIBLE" << endl; // en caso de no encontrar camino entre el nodo intermedio y el nodo destino
  	}
  	else cout << "IMPOSIBLE" << endl;  // en caso de no encontrar camino entre el nodo de comienzo y el nodo intermedio
    // Liberamos todo lo reservado dinámicamente
    for(int j = 1; j <= numNodos; ++j)
    	delete[] C[j];
    delete[] C;
    delete[] D;
  	delete[] P;
  	delete[] S;
  }
}