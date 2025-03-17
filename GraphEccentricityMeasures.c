//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphEccentricityMeasures
//

// Student Name : Ana Beatriz Gonçalves Francisco
// Student Number : 118638
// Student Name : Mariana Figueiredo Marques 
// Student Number : 118971

/*** COMPLETE THE GraphEccentricityMeasuresCompute FUNCTION ***/
/*** COMPLETE THE GraphGetCentralVertices FUNCTION ***/
/*** COMPLETE THE GraphEccentricityMeasuresPrint FUNCTION ***/

#include "GraphEccentricityMeasures.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "Graph.h"
#include "GraphAllPairsShortestDistances.h"

struct _GraphEccentricityMeasures {
  unsigned int*
      centralVertices;  // centralVertices[0] = number of central vertices
                        // array size is (number of central vertices + 1)
  int* eccentricity;    // the eccentricity value of each vertex
  Graph* graph;         // the graph
  int graphRadius;      // the graph radius
  int graphDiameter;    // the graph diameter
};

// Allocate memory
// Compute the vertex eccentricity values
// Compute graph radius and graph diameter
// Compute the set of central vertices
GraphEccentricityMeasures* GraphEccentricityMeasuresCompute(Graph* g) {
    assert(g != NULL);

    // COMPLETE THE CODE
    // CREATE AUXILIARY (static) FUNCTIONS, IF USEFUL
    // Graph radius --- the smallest vertex eccentricity value
    // Graph diameter --- the largest vertex eccentricity value
    // Do not forget that -1 represents an IDEFINITE value

    // Computing the set of central vertices
    // Allocate the central vertices array : number of central vertices + 1
    // Fill in the central vertices array

    unsigned int numVertices = GraphGetNumVertices(g);

    //Cria a estrutura onde se vai ter as informações sobre o grafo
    GraphEccentricityMeasures* measures = (GraphEccentricityMeasures*)malloc(sizeof(struct _GraphEccentricityMeasures));
    
    //Garante que a estrutura foi criada
    assert(measures != NULL);

    //Coloca o grafo na estrutura e cria um array para os valores da excentricidade de cada vértice
    measures->graph = g;
    measures->eccentricity = (int*)calloc(numVertices, sizeof(int));
    assert(measures->eccentricity != NULL);

    //Define os valores iniciais do raio e do diâmetro
    //O raio começa por ter o maior inteiro possível, uma vez que vai representar o menor valor da excentricidade
    //O diâmetro começa por ter o menor inteiro possível, uma vez que vai representar o maior valor da excentricidade
    measures->graphRadius = INT_MAX;
    measures->graphDiameter = INT_MIN;

    //Vai calcular a matriz com as distâncias entre os vértices para ser mais fácil de saber quais os valores dos caminhos de cada vértice
    GraphAllPairsShortestDistances* shortest = GraphAllPairsShortestDistancesExecute(g);

    //Por cada vértice, verifica-se os valores da excentricidade, raio, diâmetro
    for (unsigned int v = 0; v < numVertices; v++) {
        //Assim como na matriz, definimos inicialmente que o valor máximo da distância é -1
        int maxDistance = -1;
        for (unsigned int w = 0; w < numVertices; w++) {
            //Vai se buscar os valores da distância entre cada par de vértices, se for diferente de -1 e for menor do que a maior distância
            //Então o valor da distância entre os vértices vai passar a ser o maior valor
            int distance = GraphGetDistanceVW(shortest, v, w);
            if (distance != -1) {
                if (distance > maxDistance) {
                    maxDistance = distance;
                }
            }
        }
        //Depois de se percorrer a distância de um vértice com todos os outros,
        //O valor máximo da distância vai ser a excentricidade para esse vértice
        measures->eccentricity[v] = maxDistance;

        //O valor do raio e do diâmetro vai sendo atualizados se as condições para cada um forem verificadas
        if (maxDistance != -1) {
            if (maxDistance < measures->graphRadius) {
                measures->graphRadius = maxDistance;
            }
            if (maxDistance > measures->graphDiameter) {
                measures->graphDiameter = maxDistance;
            }
        }
    }

    //Cada vértice vai ter de ser conferido para ver se pode fazer parte do conjunto de vértices centrais
    unsigned int centralCount = 0;                                      //Inicialmente ainda não vão existir vértices que tenham valor igual ao raio, porque ainda não se sabe qual vai ser o valor final
    for (unsigned int v = 0; v < numVertices; v++) {
        //Se o valor da excentricidade desse vértice for igual ao raio do grafo, então o número de vértices do conjunto vai incrementar
        if (measures->eccentricity[v] == measures->graphRadius) {
            centralCount++;
        }
    }

    //Guarda num array os vértices centrais do grafo
    //O tamanho vai ter em conta centralCount+1 porque o primeiro valor do array vai conter o número de vértices do conjunto
    measures->centralVertices = (unsigned int*)malloc((centralCount + 1) * sizeof(unsigned int));
    measures->centralVertices[0] = centralCount;
    
    //Como o primeiro valor indica só o tamanho do conjunto, o index começa no 1
    unsigned int index = 1;
    for (unsigned int v = 0; v < numVertices; v++) {
        //Se o valor da excentricidade desse vértice for igual ao raio do grafo,
        //Então adiciona-se ao array do conjunto de vértices o vértice nessa posição
        //E incrementa-se o valor do índice para atualizar para se poder armazenar na próxima iteração
        if (measures->eccentricity[v] == measures->graphRadius) {
            measures->centralVertices[index++] = v;
        }
    }
    
    //Destrói a estrutura que foi criada para não ocupar espaço desnecessário na memória
    GraphAllPairsShortestDistancesDestroy(&shortest);

  return measures;          //Retorna a estrutura que tem as informações sobre o grafo
}

void GraphEccentricityMeasuresDestroy(GraphEccentricityMeasures** p) {
  assert(*p != NULL);

  GraphEccentricityMeasures* aux = *p;

  free(aux->centralVertices);
  free(aux->eccentricity);

  free(*p);
  *p = NULL;
}

// Getting the computed measures

int GraphGetRadius(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  return p->graphRadius;
}

int GraphGetDiameter(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  return p->graphDiameter;
}

int GraphGetVertexEccentricity(const GraphEccentricityMeasures* p,
                               unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));
  assert(p->eccentricity != NULL);

  return p->eccentricity[v];
}

// Getting a copy of the set of central vertices
// centralVertices[0] = number of central vertices in the set
unsigned int* GraphGetCentralVertices(const GraphEccentricityMeasures* p) {
    //Requisitos para garantir que se pode realizar a função
    assert(p != NULL);
    assert(p->centralVertices != NULL);

    unsigned int size = p->centralVertices[0];

    //Aloca-se na memória um array que vai conter a cópia do conjunto de vértices do grafo
    unsigned int* centralVerticesCopy = (unsigned int*)malloc(size * sizeof(unsigned int));
    
    //Por cada item do array, copia-se o que está no array original para a cópia
    for (unsigned int i = 0; i <= size; i++) {
        centralVerticesCopy[i] = p->centralVertices[i];
    }

    return centralVerticesCopy;                 //Retorna o array do conjunto de vértices
}


// Print the graph radius and diameter
// Print the vertex eccentricity values
// Print the set of central vertices
void GraphEccentricityMeasuresPrint(const GraphEccentricityMeasures* p) {
    //Requisito para garantir que se pode realizar a função
    assert(p != NULL);

    //Imprime no terminal o valor do raio e do diâmetro
    printf("Graph Radius: %d\n", p->graphRadius);
    printf("Graph Diameter: %d\n", p->graphDiameter);

    //Imprime no terminal o valor da excentricidade para cada vértice do grafo
    printf("Vertex Eccentricities:\n");
    unsigned int numVertices = GraphGetNumVertices(p->graph);
    for (unsigned int v = 0; v < numVertices; v++) {
        printf("Vertex %u: %d\n", v, p->eccentricity[v]);
    }

    //Imprime no terminal os vértices que fazem parte do conjunto de vértices
    //E ainda indica qual o valor do raio
    //Para ajudar a perceber qual o valor que a excentricidade tinha de ter para o vértice fazer parte do conjunto
    printf("Central Vertices (Radius = %d):\n", p->graphRadius);
    for (unsigned int i = 1; i <= p->centralVertices[0]; i++) {
        printf("%u ", p->centralVertices[i]);
    }
    printf("\n");
}

