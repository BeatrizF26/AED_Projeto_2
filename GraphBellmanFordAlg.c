//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphBellmanFord - Bellman-Ford Algorithm
//

// Student Name : Ana Beatriz Gonçalves Francisco 
// Student Number : 118638
// Student Name : Mariana Figueiredo Marques
// Student Number : 118971

/*** COMPLETE THE GraphBellmanFordAlgExecute FUNCTION ***/

#include "GraphBellmanFordAlg.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "IntegersStack.h"
#include "instrumentation.h"

struct _GraphBellmanFordAlg {
  unsigned int* marked;  // To mark vertices when reached for the first time
  int* distance;  // The number of edges on the path from the start vertex
                  // distance[i]=-1, if no path found from the start vertex to i
  int* predecessor;  // The predecessor vertex in the shortest path
                     // predecessor[i]=-1, if no predecessor exists
  Graph* graph;
  unsigned int startVertex;  // The root of the shortest-paths tree
};

GraphBellmanFordAlg* GraphBellmanFordAlgExecute(Graph* g, unsigned int startVertex) {
  //Requisitos para garantir que se pode realizar a função
  assert(g != NULL);
  assert(startVertex < GraphGetNumVertices(g));
  assert(GraphIsWeighted(g) == 0);

  //Nome dos contadores que considerámos importante usar
  InstrName[0] = "memops";                //Conta o número de vezes que se teve de aceder à memória para criar o novo grafo
  InstrName[1] = "relaxations";          //Conta o número de vezes que se tiveram de atualizar as arestas até chegar às de menor custo
  InstrName[2] = "iterations";          //Conta o número de iterações feitas para tentar relaxar as arestas

  unsigned int numVertices = GraphGetNumVertices(g);

  //Cria um grafo com as informações do algoritmo para o vértice passado como argumento
  GraphBellmanFordAlg* result = (GraphBellmanFordAlg*)malloc(sizeof(GraphBellmanFordAlg));
  InstrCount[0]++;
  assert(result != NULL);

  //Copia o grafo dado para o grafo novo que se criou e indica qual é o vértice a se considerar
  result->graph = g;
  result->startVertex = startVertex;

  //Cria arrays para alocar na memória com as informações sobre o grafo original
  //Cada um dos arrays tem em conta o número de vértices do grafo para se poder atribuir o espaço na memória
  result->marked = (unsigned int*)calloc(numVertices, sizeof(unsigned int));    //Vai conter os valores que indicam se o vértice já foi verificado ou não
  result->distance = (int*)malloc(numVertices * sizeof(int));                  //Vai conter as distâncias mínimas do startVertex até cada um dos outros vértices
  result->predecessor = (int*)malloc(numVertices * sizeof(int));              //Vai conter os valores do vértice anterior em cada caminho mais curto do startVertex até cada um dos outros vértices

  //Garante que os arrays foram alocados na memória
  assert(result->marked != NULL && result->distance != NULL && result->predecessor != NULL);

  //Antes de se começara a fazer o algoritmo, atribui-se valores iniciais a cada vértice nos arrays
  for (unsigned int i = 0; i < numVertices; i++) {
    result->marked[i] = 0;         //No início ainda não foi verificado nenhum vértice
    result->distance[i] = -1;     //Ainda não foi calculada nenhuma distância
    result->predecessor[i] = -1;  //Ainda não se sabe qual vai ser o último vértice do caminho
  }

  //No grafo criado, a distância mínima para ele próprio vai ser 0
  //Por isso, pode ser automaticamente verificado e atribuir os valores
  result->marked[startVertex] = 1;
  result->distance[startVertex] = 0;

  //Cada vértice só pode ter, no máximo, numVertices-1 arestas, uma vez que não pode ter uma aresta para ele próprio
  for (unsigned int pass = 0; pass < numVertices - 1; pass++) {
    InstrCount[0]++;            
    InstrCount[2]++;

    //Faz um loop por todos os vértices para ver se dá para relaxar as arestas conectadas ao vértice em questão
    for (unsigned int v = 0; v < numVertices; v++) {
      InstrCount[0]++;    
      InstrCount[1]++;

      //Se esse vértice ainda não tiver sido verificado, então ignora-se
      //Uma vez que não se sabe ainda quais as arestas ligadas a esse vértice nem os outros dados
      if (result->marked[v] == 0) continue;

      //Obtem-se os vértice adjacentes ao vértice em questão
      unsigned int* adjacents = GraphGetAdjacentsTo(g, v);
      unsigned int numAdjacents = adjacents[0];

      //Por cada vértice adjacente, vê se dá para relaxar a aresta entre esses dois vértices
      for (unsigned int i = 1; i <= numAdjacents; i++) {
        unsigned int neighbour = adjacents[i];

        //Para a aresta ser relaxada, não pode existir nenhum caminho entre esses vértices
        //Ou o caminho através de v é menor do que o caminho guardado até ao momento
        if (result->distance[neighbour] == -1 || result->distance[v] + 1 < result->distance[neighbour]) {
          //Se der para relaxar a aresta, então guarda-se o novo caminho por ser mais curto
          //E guarda-se também o vértice v para indicar que foi o último do caminho
          result->distance[neighbour] = result->distance[v] + 1;
          result->predecessor[neighbour] = v;
          result->marked[neighbour] = 1;                    //Já se pode marcar o vértice como visitado
        }

        //Verifica se existem ciclos negativos
        //Se existir um caminho entre o próprio vértice e ele mesmo e ele for o caminho mais curto
        //Então vai existir um ciclo negativo, o que não pode acontecer no algoritmo
        if (result->distance[v] != -1 && result->distance[v] + 1 < result->distance[neighbour]) {
        printf("Ciclo negativo detectado no grafo!\n");
        
        //Liberta a memória que não foi utilizada nos arrays
        free(adjacents);                
        free(result->marked);          
        free(result->distance);
        free(result->predecessor);
        free(result);

        return NULL;            //Retorna NULL porque não foi possível continuar o algoritmo
        }
      }
      free(adjacents);          //Liberta a memória que não foi utilizada no array dos vértices adjacentes
    }
  }

  InstrPrint();               //Mostra no terminal a tabela com os contadores
  InstrReset();              //Dá reset aos contadores
  return result;            //Retorna os resultados obtidos
}

void GraphBellmanFordAlgDestroy(GraphBellmanFordAlg** p) {
  assert(*p != NULL);

  GraphBellmanFordAlg* aux = *p;

  free(aux->marked);
  free(aux->predecessor);
  free(aux->distance);

  free(*p);
  *p = NULL;
}

// Getting the paths information

int GraphBellmanFordAlgReached(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  return p->marked[v];
}

int GraphBellmanFordAlgDistance(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  return p->distance[v];
}
Stack* GraphBellmanFordAlgPathTo(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  Stack* s = StackCreate(GraphGetNumVertices(p->graph));

  if (p->marked[v] == 0) {
    return s;
  }

  // Store the path
  for (unsigned int current = v; current != p->startVertex;
       current = p->predecessor[current]) {
    StackPush(s, current);
  }

  StackPush(s, p->startVertex);

  return s;
}

// DISPLAYING on the console

void GraphBellmanFordAlgShowPath(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  Stack* s = GraphBellmanFordAlgPathTo(p, v);

  while (StackIsEmpty(s) == 0) {
    printf("%d ", StackPop(s));
  }

  StackDestroy(&s);
}

// Display the Shortest-Paths Tree in DOT format
void GraphBellmanFordAlgDisplayDOT(const GraphBellmanFordAlg* p) {
  assert(p != NULL);

  Graph* original_graph = p->graph;
  unsigned int num_vertices = GraphGetNumVertices(original_graph);

  // The paths tree is a digraph, with no edge weights
  Graph* paths_tree = GraphCreate(num_vertices, 1, 0);

  // Use the predecessors array to add the tree edges
  for (unsigned int w = 0; w < num_vertices; w++) {
    // Vertex w has a predecessor vertex v?
    int v = p->predecessor[w];
    if (v != -1) {
      GraphAddEdge(paths_tree, (unsigned int)v, w);
    }
  }

  // Display the tree in the DOT format
  GraphDisplayDOT(paths_tree);

  // Housekeeping
  GraphDestroy(&paths_tree);
}

