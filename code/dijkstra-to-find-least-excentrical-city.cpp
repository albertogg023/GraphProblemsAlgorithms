#include <stdlib.h>  // Funcion exit
#include <iostream>  // Variables cin y cout
using namespace std;


//////////////////////////////////////////////////////////////
////////////        VARIABLES GLOBALES        ////////////////
//////////////////////////////////////////////////////////////

int numNodos;  // Número de nodos
int C[200][200]; // Matriz de costes
string N[200];  // Array con los nombres asociados a cada nodo
int D[200];  // Array de distancias del algoritmo de Dijkstra
int P[200];  // Array de paso del algoritmo de Dijkstra
bool S[200]; // Array de nodos con solución del algoritmo de Dijkstra


//////////////////////////////////////////////////////////////
////////////     FUNCIONES DEL PROGRAMA       ////////////////
//////////////////////////////////////////////////////////////

// LEEMOS LOS NOMBRES DE LAS CIUDADES DE LA ENTRADA
void leerNombresCiudades(void){
  string nombre;
  getline(cin, nombre); // para consumir el resto de la línea por el cin anterior
  for(int i = 0; i < numNodos; ++i){
    getline(cin, nombre);
    N[i] = nombre;
  }
}

// CREAMOS LA MATRIZ DE COSTES DEL GRAFO DE LA ENTRADA
void leerMatrizCostes (void){
  // Creamos una matriz para las aristas (si el peso es '-1' es que no existe conexión entre los nodos)
  for(int i = 0; i < numNodos; ++i){
    for(int j = 0; j < numNodos; ++j)
      C[i][j] = -1;  // inicializamos sus aristas a 'no existe conexión'
  }
  // Asignamos las aristas leidas de la entrada
  int v, w, p;
  cin >> v >> w >> p;
  while(v != 0 || w != 0 || p != 0){
    C[v][w] = p;
    C[w][v] = p;
    cin >> v >> w >> p;
  }
}

// LEEMOS LOS DATOS DEL CASO
void leerDatos(void){
  cin >> numNodos;
  if(numNodos < 0){ // si el número de nodos
    cerr << "Número de nodos incorrectos" << endl; // imprimimos mensaje de error
    exit(0);  // abortamos el programa
  }
  leerNombresCiudades();
  leerMatrizCostes();
}

// INICIALIZAMOS LAS VARIABLES NECESARIAS PARA EL ALGORITMO DE DIJKSTRA
void inicializacionDijkstra(int nodoComienzo){
  for(int v = 0; v < numNodos; ++v){
    S[v] = false; // nodos que se encuentran en la solución
    P[v] = nodoComienzo; // nodos de paso inicialmente son el nodo de comienzo
    D[v] = C[nodoComienzo][v]; // costes de caminos mínimos son inicialmente los costes entre el nodo y el nodo de comienzo
  }
  S[nodoComienzo] = true; // el nodo de comienzo se añade a la solución inicialmente
}

// OBTENEMOS EL NODO QUE TIENE LA DISTANCIA MÍNIMA DEL ARRAY DE DISTANCIAS USADO EN EL ALGORITMO DE DIJKSTRA
int obtenerNodoConDistanciaMinima(void){
  int nodoMin = 0;  // seleccionamos un nodo mínimo inicial que puede ser no válido
  int valorMin = 100000000; // seleccionamos un valor mínimo inicial muy grande, para que el nodo mínimo inicial sea fácilmente sustitutible
  for(int v = 0; v < numNodos; ++v){ // procedemos a encontrar el mínimo de facto
    if( (!S[v]) && D[v] != -1 && D[v] < valorMin){
      valorMin = D[v];
      nodoMin = v;
    }
  }
  return nodoMin;
}

// OBTENEMOS LA MAYOR DISTANCIA DEL ARRAY DE DISTANCIAS USADO EN EL ALGORITMO DE DIJKSTRA
int obtenerMayorDistancia(void){
  int distanciaMaxima = -1; // seleccionamos un valor mínimo inicial muy grande, para que el nodo mínimo inicial sea fácilmente sustitutible
  for(int v = 0; v < numNodos; ++v) // procedemos a encontrar el mínimo de facto
    if(D[v] > distanciaMaxima)
      distanciaMaxima = D[v];
  return distanciaMaxima;
}

// EJECUTAMOS EL ALGORITMO DE DIJKSTRA PARA OBTENER LA MAYOR DISTANCIA ENTRE PLANETAS EN EL CAMINO MÁS MÍNIMO ENTRE DOS PLANETAS DADOS
void dijkstra(int nodoComienzo){
  inicializacionDijkstra(nodoComienzo); // inicializamos las variables necesarias
  int v;
  for(int i = 0; i < numNodos; ++i){
    v = obtenerNodoConDistanciaMinima();  // obtenemos el nodo con la distancia mínima calculada que no esté en la solución
    S[v] = true;  // lo añadimos a la solución
    // Para cada nodo adyacente a este recalculamos sus distancias en el caso de que mejoren
    for(int w = 0; w < numNodos; ++w)
      if(C[v][w] != -1)  // si es adyacente
        if( (!S[w]) && ( D[w] == -1 || D[v] + C[v][w] < D[w]) ){ // si 'w' no está en la solución y el camino es más corto pasando por 'v' actualizamos
          D[w] = D[v] + C[v][w];
          P[w] = v;
        } 
  }
}


//////////////////////////////////////////////////////////////
////////////        PROGRAMA PRINCIPAL        ////////////////
//////////////////////////////////////////////////////////////

int main (void){
  int numCasos;
  int menorExcentricidad;
  int nodoMenosExcentrico;
  int mayorDistanciaNodo;
  cin >> numCasos;
  for(int i = 0; i < numCasos; ++i){
    leerDatos();  // leemos los datos de entrada del caso
    menorExcentricidad = 100000000; // seleccionamos inicialmente una excentricidad muy grande para que sea fácilmente sustituible
    nodoMenosExcentrico = 0;  // ponemos inicialmente el primer nodo como el menos excéntrico
    for(int v = 0; v < numNodos; ++v){
      dijkstra(v);  // ejecutamos el algoritmo de dijkstra para el nodo
      mayorDistanciaNodo = obtenerMayorDistancia(); // obtenemos el camino más largo del nodo
      if(mayorDistanciaNodo < menorExcentricidad){  // si su camino más largo es más corto que el guardado como más corto lo sustituimos
        nodoMenosExcentrico = v;
        menorExcentricidad = mayorDistanciaNodo;
      }
      else if(mayorDistanciaNodo == menorExcentricidad && N[v] < N[nodoMenosExcentrico]){ // si su camino más largo es igual que el guardado como más corto y es menor lexicográficamente lo sustituimos
        nodoMenosExcentrico = v;
        menorExcentricidad = mayorDistanciaNodo;
      }
    }
    cout << N[nodoMenosExcentrico] << endl << menorExcentricidad << endl; // imprimimos la ciudad menos excéntrica y el valor de su distancia máxima
  }
}