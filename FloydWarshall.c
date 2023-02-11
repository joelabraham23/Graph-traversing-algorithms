// Floyd Warshall ADT interface
// COMP2521 Assignment 2

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "FloydWarshall.h"
#include "Graph.h"

static Vertex **create2DarrayAndInitialize(int size, int initializer);


/**
 * Finds all shortest paths between all pairs of nodes.
 * 
 * The  function  returns  a 'ShortestPaths' structure with the required
 * information:
 * - the number of vertices in the graph
 * - distance matrix
 * - matrix of intermediates (see description above)
 */
ShortestPaths FloydWarshall(Graph g) {
// TODO: Implement this function
    // ShortestPaths sps = {0};
    ShortestPaths sps;
    int numNodes = GraphNumVertices(g);
    sps.numNodes = numNodes;

    Vertex **dist = create2DarrayAndInitialize(numNodes, INFINITY);
    Vertex **next = create2DarrayAndInitialize(numNodes, -1);

    // Make dist between the same vertexs 0
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            if (i == j) {
                dist[i][j] = 0;
            }
        }
    }

    // Assign weights to dist array and direct links to next array
    for (Vertex i = 0; i < numNodes; i++) {
        AdjList curr = GraphOutIncident(g, i);
        while (curr != NULL) {
            dist[i][curr->v] = curr->weight;
            next[i][curr->v] = curr->v;
            curr = curr->next;
        }
    }
    // FloydWarshall Algorithm
    for (Vertex k = 0; k < numNodes; k++) {
        for (Vertex j = 0; j < numNodes; j++) {
            for (Vertex i = 0; i < numNodes; i++) {
                // If the dist is less than the distance already assigned in distance array then assign the new 
                // distance to be the shortest distance. An extra if statement is used to prevent integer overflow.
                if (dist[i][k] + dist[k][j] < dist[i][j] && (dist[i][k] != INFINITY && dist[k][j] != INFINITY)) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }

    sps.dist = dist;
    sps.next = next;


    return sps;
}
/**
 * This  function  is  for  you to print out the ShortestPaths structure
 * while you are debugging/testing your implementation. 
 * 
 * We will not call this function during testing, so you may  print  out
 * the  given  ShortestPaths  structure in whatever format you want. You
 * may choose not to implement this function.
 */
void showShortestPaths(ShortestPaths sps) {
    
}

/**
 * Frees  all  memory associated with the given ShortestPaths structure.
 * We will call this function during testing, so you must implement it.
 */
void freeShortestPaths(ShortestPaths sps) {
    // TODO: Implement this function
    for (int i = 0; i < sps.numNodes; i++) { 
        Vertex *currentDistPtr = sps.dist[i];
        Vertex *currentNextPtr = sps.next[i]; 
        free(currentDistPtr);
        free(currentNextPtr);
    }
    free(sps.dist);
    free(sps.next);
}

static Vertex **create2DarrayAndInitialize(int size, int initializer) {
    // Mallocing space for 2d array with given size
    int **arr = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        arr[i] = (int *)malloc(size * sizeof(int));
    }
    // Initializing the array to given initializer value
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            arr[i][j] = initializer;
        }
    }
    return arr;

}
