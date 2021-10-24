#include <stdlib.h>  // Funcion exit
#include <string.h>  // Funcion memset
#include <iostream>  // Variables cin y cout
#include <list> // Listas de adyacencia
using namespace std;
#define MAX_NODOS 1000


//////////////////////////////////////////////////////////////
////////////        VARIABLES GLOBALES        ////////////////
//////////////////////////////////////////////////////////////

int numNodos; // Número de nodos del grafo
list<int> G[MAX_NODOS+1]; // Listas de adyacencia del grafo de la entrada
list<int> traspuestaG[MAX_NODOS+1]; // Listas de adyacencia del grafo de la entrada transpuestas
bool visitados[MAX_NODOS+1]; // Marcas de nodos visitados
int numPaso; // Contador para númerar los nodos en orden posterior
int nPosts[MAX_NODOS+1]; // Array para almacenar en que paso se han resuelto las llamadas recursivas del nodo en la bpp
list<list<int>> paginasSimplesOrdenadas;  // Lista de páginas simples ordenadas


//////////////////////////////////////////////////////////////
////////////     FUNCIONES DEL PROGRAMA       ////////////////
//////////////////////////////////////////////////////////////

// PROCEDIMIENTO QUE VACÍA LAS LISTAS DE ADYACENCIA USADAS
void vaciarListasAdyacencia(){
  for(int i = 1; i <= numNodos; ++i){
    G[i].clear();
    traspuestaG[i].clear();
  }
}
// PROCEDIMIENTO PARA LEER UN GRAFO DE LA ENTRADA
void leeGrafo(void){
  cin >> numNodos;  // leemos el número de nodos de este caso
  if(numNodos < 0 || numNodos > MAX_NODOS) { // si el número de nodos es negativo o mayor que el máximo
    cerr << "Numero de nodos (" << numNodos << ") no valido" << endl; // imprimimos mensaje de error
    exit(0);  // abortamos el programa
  }
  // Si el número de nodos es válido
  int w;
  for(int v = 1; v <= numNodos; ++v){ // para todos los nodos del caso leemos sus adyacentes
    cin >> w; // lectura primer elemento
    while(w != 0){ // comprobar
      G[v].push_back(w);
      cin >> w; // avanzar
    }
  }
}

// PROCEDIMIENTO RECURSIVO DE LA BÚSQUEDA PRIMERO EN PROFUNDIDAD DE UN NODO DADO
void bpp(int v){
  visitados[v] = true;  // ponemos el nodo como visitado
  list<int>::iterator it = G[v].begin();
  while( it != G[v].end() ){  // mientras haya nodos adyacentes
    if(!visitados[*it]) // llamamos de nuevo a la función recursiva con los nodos adyacentes no visitados
      bpp(*it);
    ++it;
  }
  nPosts[v] = numPaso; // asignamos en la matriz nPost, el paso del elemento 'v' (orden en el que se resuelve la llamada)
  ++numPaso;
}

// PROCEDIMIENTO PRINCIPAL DE LA BÚSQUEDA PIMERO EN PROFUNDIDAD
void busquedaPP(void){
  numPaso = 1;
  memset( visitados, false, sizeof(visitados) );  // inicializamos a 'false' todas las posiciones del array visitados
  memset( nPosts, 0, sizeof(nPosts) );  // inicializamos a '0' todas las posiciones del array nPosts
  for(int v = 1; v <= numNodos; ++v)
    if(!visitados[v])
      bpp(v);
}

// OBTENEMOS EL ÍNDICE DEL ARRAY nPosts CUYO CONTENIDO SEA EL MAYOR
int obtenerIndicePostMayor(void){
  int indiceMaximo = 0;
  for(int i = 1; i <= numNodos; ++i){
    if(visitados[i])
      nPosts[i] = 0;  // hacemos '0' aquellos que hayamos visitados
    else if(nPosts[i] > nPosts[indiceMaximo])
      indiceMaximo = i;
  }
  return indiceMaximo;
}

// PROCEDIMIENTO QUE INVIERTE UNA MATRIZ
void invertirMatriz(){
  list<int>::iterator it;
  for(int v = numNodos; v >= 1; --v){ // invertimos el grafo leído insertando los elementos de las listas de adyacencia que lo representan en orden inverso
    it = G[v].begin();
    while( it != G[v].end() ){
      traspuestaG[*it].push_front(v);
      ++it;
    }
  }
}

// FUNCIÓN QUE DEVUELVE EL ÁRBOL FUERTEMENTE CONEXO DE UN NODO DADO
// REALIZANDO PARA ESTE, UNA BÚSQUEDA PRIMERO EN PROFUNDIDAD DE FORMA TRANSPUESTA
list<int> bppTranspuesta(int v){
  list<int> nodosComponenteConexo;  // creamos una lista vacía que representa el árbol fuertemente conexo
  nodosComponenteConexo.push_back(v); // insertamos el nodo en la lista
  visitados[v] = true;  // lo ponemos como visitado
  
  list<int>::iterator it = traspuestaG[v].begin();
  while( it != traspuestaG[v].end() ){  // mientras haya nodos adyacentes
    if(!visitados[*it])   // si no ha sido visitado
      nodosComponenteConexo.merge( bppTranspuesta(*it) ); // añadimos los nodos que conforman los subárboles fuertemente conexos
    ++it;
  }
  return nodosComponenteConexo;
}

// PROCEDIMIENTO QUE CALCULA EN ORDEN, LOS COMPONENTES CONEXOS DE UN GRAFO
void calculaCFC(void){
  busquedaPP();
  invertirMatriz();
  // Inicializamos las variables necesarias
  paginasSimplesOrdenadas.clear();  // vaciamos la lista de páginas simples
  memset( visitados, false, sizeof(visitados) );  // inicializamos a 'false' todas las posiciones del array visitados
  visitados[0] = true;  // centinela para evitar que, una vez se hayan consumido los elementos válidos de nPosts, se haga la BPP
  list<list<int>> paginasSimplesSinOrdenar; // lista de web simples, cada web simple almacena las webs que la forman
  int indicePostMaximo = -1;
  // Añadimos a la lista de páginas simples cada una de estas, cada una teniendo la lista de las páginas que la forman
  for(int i = 1; i <= numNodos && indicePostMaximo != 0; i++){
    indicePostMaximo = obtenerIndicePostMayor();
    if(!visitados[indicePostMaximo]){  // si el maximo de nPosts es '0', se usaría la centinela para evitar entrar en el if
      paginasSimplesSinOrdenar.push_back( bppTranspuesta(indicePostMaximo) );
    }
  }
  // Ordenamos la lista de forma ordenada en nuestra variable global 'listaPaginasOrdenadas'
  list<list<int>>::iterator itNoOrdenada = paginasSimplesSinOrdenar.begin();  // iterador de la lista no ordenada
  while( itNoOrdenada != paginasSimplesSinOrdenar.end() ){  // recorrremos la lista de listas no ordenadas, y para cada elemento, vemos donde le tocaría insertarse en la lista ordenada
    list<list<int>>::iterator itOrdenada = paginasSimplesOrdenadas.begin();
    while( itOrdenada != paginasSimplesOrdenadas.end() && (*itNoOrdenada).front() > (*itOrdenada).front() ){
      ++itOrdenada;
    }
    paginasSimplesOrdenadas.insert(itOrdenada, *itNoOrdenada); // no hacemos ninguna comprobación ya que no hay repeticiones
    ++itNoOrdenada;
  }
} 

// IMPRIMIMOS LA SALIDA DEL CASO
void imprimirSalidaCaso(int numCaso){
  list<list<int>>::iterator itOrdenada = paginasSimplesOrdenadas.begin();
  cout << "Caso " << numCaso << ": total " << paginasSimplesOrdenadas.size() << endl;  // imprimimos el número de páginas simples
  int i = 1;
  // Para cada página simple, imprimimos las páginas que la forman
  while( itOrdenada != paginasSimplesOrdenadas.end() ){
    cout << "Simple " << i << ":";  // imprimimos la página simple
    list<int>::iterator itNodos = (*itOrdenada).begin();
    while( itNodos != (*itOrdenada).end() ){
      cout << " " << *itNodos;  // imprimimos las páginas que la forman
      ++itNodos;
    }
    ++itOrdenada;
    ++i;
    cout << endl;
  }
}


//////////////////////////////////////////////////////////////
////////////        PROGRAMA PRINCIPAL        ////////////////
//////////////////////////////////////////////////////////////

int main (void){
  int numCasos;
  cin >> numCasos;
  for (int i = 1; i <= numCasos; ++i){
    leeGrafo();
    calculaCFC();
    imprimirSalidaCaso(i);
    vaciarListasAdyacencia();
  }
}