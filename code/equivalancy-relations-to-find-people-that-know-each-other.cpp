#include <stdlib.h>  // Funcion exit
#include <iostream>  // Funciones cin y cout
using namespace std;


//////////////////////////////////////////////////////////////
////////////     FUNCIONES DEL PROGRAMA       ////////////////
//////////////////////////////////////////////////////////////

// ENCONTRAMOS EL ELEMENTO REALIZANDO COMPRESIÓN DE CAMINOS
int encontrar(int * relaciones, int i){
  if(relaciones[i] <= 0)
    return i;
  else{
    relaciones[i] = encontrar(relaciones, relaciones[i]);
    return relaciones[i];
  }
}

// IMPRIMIMOS EL NÚMERO DE ISLAS, Y A QUE ISLA PERTENECE CADA PERSONA, LA IMPLEMENTACIÓN ES MEDIANTE RELACIONES DE EQUIVALENCIA
void determinarIslas(){
  int numPersonas;
  int numConocidosDirectos;
  cin >> numPersonas;
  cin >> numConocidosDirectos;
  if(numPersonas < 0 || numConocidosDirectos < 0){
    cerr << "Numero de personas o de conocidos directos inválido" << endl; // imprimimos mensaje de error si el número de personas o de emparejamientos no es correcto
    exit(0);
  }
  else{
    // CREAMOS UNA RELEACIÓN DE EQUIVALENCIA
    int * relaciones = new int[numPersonas+1];  // reservamos para numPersonas+1 para que sea más fácil la gestión de los índices
    for(int i = 1; i <= numPersonas; ++i) relaciones[i] = 0;  // inicializamos las personas como raices
    // ESTABLECEMOS LAS RELACIONES DE EQUIVALENCIA ENTRE LAS DISTINTAS PERSONAS 
    int persona1; // primera persona de la pareja leída
    int persona2; // segunda persona de la pareja leída
    for(int i = 1; i <= numConocidosDirectos; ++i){
      cin >> persona1 >> persona2;
      persona1 = encontrar(relaciones, persona1); // encontramos su raiz
      persona2 = encontrar(relaciones, persona2); // encontramos su raiz
      // Nótese que asignamos el índice mayor siempre al menor, para que se cumpla la condición del orden de la islas para las personas
      if(persona1 < persona2) 
        relaciones[persona2] = persona1;  // apuntamos persona2 a persona1
      else if(persona1 > persona2)
        relaciones[persona1] = persona2;  // apuntamos persona1 a persona2
    }
    // A CADA RAIZ LE ASIGNAMOS UN NÚMERO DE ISLA
    int indiceIsla = 1;
    for(int i = 1; i <= numPersonas; ++i){
      if(relaciones[i] == 0){
        relaciones[i] = -indiceIsla;
        ++indiceIsla;
      }
    }
    // IMPRIMIMOS EL NÚMERO DE ISLAS, LA ISLA A LA QUE PERTENECE CADA PERSONA, Y LIBERAMOS LA MEMORIA CREADA DINÁMICAMENTE
    cout << indiceIsla-1 << endl;
    for(int i = 1; i <= numPersonas; ++i){
      cout << -relaciones[encontrar(relaciones, i)] << endl;
    }
    delete relaciones;
  }
}


//////////////////////////////////////////////////////////////
////////////        PROGRAMA PRINCIPAL        ////////////////
//////////////////////////////////////////////////////////////

int main (void){
  int numCasos;
  cin >> numCasos;
  for (int i = 1; i <= numCasos; ++i){
    cout << "Caso " << i << endl;
    determinarIslas();
  }
}