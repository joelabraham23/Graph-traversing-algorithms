// Centrality Measures ADT interface
// COMP2521 Assignment 2

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "FloydWarshall.h"
#include "Graph.h"

static double **twoDArrayCreateAndInitialise(int size, double initializer);

/**
 * Finds  the  edge  betweenness  centrality  for each edge in the given
 * graph and returns the results in a  EdgeValues  structure.  The  edge
 * betweenness centrality of a non-existant edge should be set to -1.0.
 */
EdgeValues edgeBetweennessCentrality(Graph g) {
    EdgeValues e;

    e.numNodes = GraphNumVertices(g);
    e.values = twoDArrayCreateAndInitialise(e.numNodes, -1.0);

    ShortestPaths sps = FloydWarshall(g);
    Vertex **distOfGraph = sps.dist;
    Vertex **nextOfGraph = sps.next;

    for (int src_edge = 0; src_edge < e.numNodes; src_edge++) {
        for (int dest_edge = 0; dest_edge < e.numNodes; dest_edge++) {
            // If there is an adjacent edge
            if (GraphIsAdjacent(g, src_edge, dest_edge)) {
                // Centrality measure becomes zero if adjacent edge is found
                e.values[src_edge][dest_edge] = 0.0;
                // Nested for loop to count amount of time that edge connecting src_edge and dest_edge is passed through
                for (Vertex i = 0; i < e.numNodes; i++) {
                    for (Vertex j = 0; j < e.numNodes; j++) {
                        Vertex path_pre = i;
                        // While loop to check all edges in graph and count how many paths go through the original edge
                        while (distOfGraph[path_pre][j] != 0 && distOfGraph[path_pre][j] != INFINITY) {
                            // If an edge passes through src and dest then add to the centrality for that edge
                            if (path_pre == src_edge && nextOfGraph[path_pre][j] == dest_edge) {
                                if (e.values[src_edge][dest_edge] == -1.0) {
                                    e.values[src_edge][dest_edge] = 0.0;
                                }
                                e.values[src_edge][dest_edge]++;
                            }
                            path_pre = nextOfGraph[path_pre][j];
                            
                        }
                    }

                }
            }
        }
    }
    freeShortestPaths(sps);
    return e;
}

/**
 * Prints  the  values in the given EdgeValues structure to stdout. This
 * function is purely for debugging purposes and will NOT be marked.
 */
void showEdgeValues(EdgeValues evs) {

}

/**
 * Frees all memory associated with the given EdgeValues  structure.  We
 * will call this function during testing, so you must implement it.
 */
void freeEdgeValues(EdgeValues evs) {
    // TODO: Implement this function
    for (int i = 0; i < evs.numNodes; i++) { 
        double *currentValuesPtr = evs.values[i];
        free(currentValuesPtr);
    }
    free(evs.values);
}

static double **twoDArrayCreateAndInitialise(int size, double initializer) {
    // Mallocing space for 2d array with given size
    double **arr = (double **)malloc(size * sizeof(double *));
    for (int i = 0; i < size; i++) {
        arr[i] = (double *)malloc(size * sizeof(double));
    }
    // Initializing the array to given initializer value
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            arr[i][j] = initializer;
        }
    }
    return arr;

}