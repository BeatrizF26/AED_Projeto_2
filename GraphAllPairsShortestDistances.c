//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphAllPairsShortestDistances
//

// Student Name : Ana Beatriz Gonçalves Francisco
// Student Number : 118638
// Student Name : Mariana Figueiredo Marques
// Student Number : 118971

/*** COMPLETE THE GraphAllPairsShortestDistancesExecute FUNCTION ***/

#include "GraphAllPairsShortestDistances.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphBellmanFordAlg.h"

struct _GraphAllPairsShortestDistances {
  int** distance;  // The 2D matrix storing the all-pairs shortest distances
                   // It is stored as an array of pointers to 1D rows
                   // Idea: an INDEFINITE distance value is stored as -1
  Graph* graph;
};

// Allocate memory and initialize the distance matrix
// Compute the distances between vertices by running the Bellman-Ford algorithm
GraphAllPairsShortestDistances* GraphAllPairsShortestDistancesExecute(
    Graph* g) {
    //Requisito para garantir que se pode realizar a função
    assert(g != NULL);

    //Obtém o número de vértices que o grafo original tem
    unsigned int numVertices = GraphGetNumVertices(g);

    //Aloca na memória a estrutura onde se vai registar as distâncias mais pequenas entre vértices e garante que essa estrutura foi criada
    GraphAllPairsShortestDistances* shortest = (GraphAllPairsShortestDistances*)malloc(sizeof(GraphAllPairsShortestDistances));
    assert(shortest != NULL);

    //Guarda o grafo original na estrutura criada
    shortest->graph = g;

    // Aloca e inicializa a matriz de distâncias
    //Aloca na memória a matriz que que vai ter as distâncias entre os vértices e garante que essa matriz foi criada
    shortest->distance = (int**)calloc(numVertices, sizeof(int*));
    assert(shortest->distance != NULL);

    //Na matriz, cada vértice vai ter de ter o valor do seu caminho para todos os outros vértices, inclusive ele próprio
    //Ou seja, cada um vai ter um array onde se vai alocar as distâncias para cada um desses vértices
    for (unsigned int i = 0; i < numVertices; i++) {
        shortest->distance[i] = (int*)calloc(numVertices, sizeof(int));
        assert(shortest->distance[i] != NULL);

        //Antes de se começar a calcular os valores para os caminhos,
        //Assumimos que não existe nenhumcaminho possivel entre os vértices, por isso, o valor da distância vai ser -1
        for (unsigned int j = 0; j < numVertices; j++) {
            shortest->distance[i][j] = -1;
        }
    }

    //Para se saber qual vai ser o valor a colocar na matriz, é preciso fazer o algoritmo Bellmand-Ford para cada vértice
    for (unsigned int v = 0; v < numVertices; v++) {
        GraphBellmanFordAlg* bf = GraphBellmanFordAlgExecute(g, v);

        //Se por algum motivo não se conseguir obter os dados do algoritmo,
        //Então liberta-se da memória o espaço que estava guardado para a estrutura e retorna-se NULL porque não se conseguiu um resultado
        if (bf == NULL) {
            GraphAllPairsShortestDistancesDestroy(&shortest);
            return NULL;
        }

        //Por cada vértice, vai buscar ao algoritmo a distãncia entre esse par de vértice e armazena o seu valor na matriz
        for (unsigned int w = 0; w < numVertices; w++) {
            if (GraphBellmanFordAlgReached(bf, w)) {
                shortest->distance[v][w] = GraphBellmanFordAlgDistance(bf, w);
            }
        }
        //Destrói a estrutura que foi criada com o algoritmo Bellman-Ford para não ocupar espaço desnecessário na memória
        GraphBellmanFordAlgDestroy(&bf);
    }

    return shortest;                //Retorna a estrutura que contém a matriz com as distâncias entre os vértices
}


void GraphAllPairsShortestDistancesDestroy(GraphAllPairsShortestDistances** p) {
  assert(*p != NULL);

  GraphAllPairsShortestDistances* aux = *p;
  unsigned int numVertices = GraphGetNumVertices(aux->graph);

  for (unsigned int i = 0; i < numVertices; i++) {
    free(aux->distance[i]);
  }

  free(aux->distance);

  free(*p);
  *p = NULL;
}

// Getting the result

int GraphGetDistanceVW(const GraphAllPairsShortestDistances* p, unsigned int v,
                       unsigned int w) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));
  assert(w < GraphGetNumVertices(p->graph));

  return p->distance[v][w];
}

// DISPLAYING on the console

void GraphAllPairsShortestDistancesPrint(
    const GraphAllPairsShortestDistances* p) {
  assert(p != NULL);

  unsigned int numVertices = GraphGetNumVertices(p->graph);
  printf("Graph distance matrix - %u vertices\n", numVertices);

  for (unsigned int i = 0; i < numVertices; i++) {
    for (unsigned int j = 0; j < numVertices; j++) {
      int distanceIJ = p->distance[i][j];
      if (distanceIJ == -1) {
        // INFINITY - j was not reached from i
        printf(" INF");
      } else {
        printf(" %3d", distanceIJ);
      }
    }
    printf("\n");
  }
}
