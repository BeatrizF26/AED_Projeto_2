//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphTransitiveClosure - Transitive Closure of a directed graph
//

// Student Name : Ana Beatriz Gonçalves Francisco
// Student Number : 118638
// Student Name : Mariana Figueiredo Marques
// Student Number : 118971

/*** COMPLETE THE GraphComputeTransitiveClosure FUNCTION ***/

#include "GraphTransitiveClosure.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphBellmanFordAlg.h"
#include "instrumentation.h"

// Compute the transitive closure of a directed graph
// Return the computed transitive closure as a directed graph
// Use the Bellman-Ford algorithm
Graph* GraphComputeTransitiveClosure(Graph* g) {
    //Requisitos para garantir que se pode realizar a função
    assert(g != NULL);
    assert(GraphIsDigraph(g));
    assert(GraphIsWeighted(g) == 0);

    //Nome dos contadores que considerámos importante usar
    //Os contadores usados no algoritmo Bellman-Ford continuam a poder ser incrementados nesta função através dos índices
    InstrName[3] = "comparisons";            //Conta o número de comparações feitas
    InstrName[4] = "edges added";           //Conta o número de arestas que se teve de adicionar
    InstrName[5] = "Bellman-Ford calls";   //Conta o número de vezes que se teve de usar o algoritmo Bellman-Ford

    unsigned int numVertices = GraphGetNumVertices(g);

    //Cria o fecho transitivo do grafo original com os mesmos dados
    //Para se poder criar o fecho transitivo, o grafo original tem de ser um digrafo mas as suas arestas não podem ter nenhum valor associado
    //Por isso é que ao criar o novo grafo, assumimos os valores 1 (é um digrafo) e 0 (as arestas não têm valores associados) 
    Graph* transitiveClosure = GraphCreate(numVertices, 1, 0);
    InstrCount[0]++;                                                    //Incrementa o contador memops

    //Tem de se verificar os caminhos de todos os vértices do grafo original
    for (unsigned int v = 0; v < numVertices; v++) {
       InstrCount[2]++;                                                 //Incrementa o contador iterations

        //Vai buscar os caminhos mais curtos do vértice em questão através do algoritmo Bellman-Ford
        //Assim, já se sabe quais as arestas que se têm de adicionar
        GraphBellmanFordAlg* bellmanFord = GraphBellmanFordAlgExecute(g, v);
        InstrCount[0]++;                                                //Incrementa o contador memops
        InstrCount[5]++;

        // adiciona caminhos para todos os vertices que tem indiretamente outro caminho
        //Verifica todos os pares de vértices possíveis
        for (unsigned int w = 0; w < numVertices; w++) {
            InstrCount[2]++;                                            //Incrementa o contador iterations
            InstrCount[3]++;
            
            //Caso exista um caminho entre os vértices e eles não sejam os mesmos, então adiciona-se essa aresta no fecho transitivo
            if (GraphBellmanFordAlgReached(bellmanFord, w) && v != w) {
                GraphAddEdge(transitiveClosure, v, w);
                InstrCount[4]++;
            }
        }

        //Destrói a estrutura que foi criada com o algoritmo Bellman-Ford para não ocupar espaço desnecessário na memória
        GraphBellmanFordAlgDestroy(&bellmanFord);
        InstrCount[0]++;
    }

    printf("\n----------Contadores usados pelo módulo Transitive-Closure----------\n");
    InstrPrint();
    InstrReset();

    return transitiveClosure;                       //Retorna o fecho transitivo do grafo original
}