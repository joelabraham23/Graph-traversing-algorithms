// Girvan-Newman Algorithm for community discovery
// COMP2521 Assignment 2

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "FloydWarshall.h"
#include "CentralityMeasures.h"
#include "GirvanNewman.h"
#include "Graph.h"

static void removeHighestCentralityEdges(Graph g, EdgeValues e);
static Dendrogram DoGirvanNewman(Graph g, Vertex Value);
static bool singleVertex(ShortestPaths sps, Vertex i);
static Graph CreateNewGraph(Graph g, ShortestPaths sps, int value);
static bool seperation(ShortestPaths sps, ShortestPaths removedsps, int i, int j);
static void FreeingEverything(Graph left, Graph right, EdgeValues e, ShortestPaths removedsps, ShortestPaths sps);
/**
 * Generates  a Dendrogram for the given graph g using the Girvan-Newman
 * algorithm.
 * 
 * The function returns a 'Dendrogram' structure.
 */

Dendrogram GirvanNewman(Graph g) {
	// TODO: Implement this function
	return DoGirvanNewman(g, -1);
}
// Function to perform GirvanNewman recursively
static Dendrogram DoGirvanNewman(Graph g, Vertex value) {
	int numNodes = GraphNumVertices(g);
	Dendrogram d = malloc(sizeof(DNode));
	ShortestPaths sps = FloydWarshall(g);
	// Base case (only 1 vertex in community)
	if (value != -1 && singleVertex(sps, value))
		d->vertex = value;

	// While there are still edges in the graph
	// i.e when the highest centrality is not -1
	bool edgeNonExistence = false;
	while (!edgeNonExistence) {
		EdgeValues e = edgeBetweennessCentrality(g);
		// Removes highest centrality edges from graph g
		removeHighestCentralityEdges(g, e);
		// New sps struct created with removed edge to calculate 
		// whether seperation occured
		ShortestPaths removedsps = FloydWarshall(g);
		// Checks there are still edges in graph
		edgeNonExistence = true;
		for (int i = 0; i < numNodes; i++) {
			for (int j = 0; j < numNodes; j++) {
				if (removedsps.next[i][j] != -1) {
					edgeNonExistence = false;
				}
			}
		}
		for (int i = 0; i < sps.numNodes; i++) {
			for (int j = 0; j < sps.numNodes; j++) {
				// If there was a seperation and communities were split into two
				if (seperation(sps, removedsps, i , j)) {
					// splits community into right subtree of dendogram
					Graph right = CreateNewGraph(g, removedsps, j);
					d->right = DoGirvanNewman(right, j);
					// splits community into left subtree of dendogram
					Graph left = CreateNewGraph(g, removedsps, i);
					d->left = DoGirvanNewman(left, i);
					//Freeing
					FreeingEverything(left, right, e, removedsps, sps);
					return d;
				}
			}
		}
	}
	freeShortestPaths(sps);
	return d;
}

/**
 * Frees all memory associated with the given Dendrogram  structure.  We
 * will call this function during testing, so you must implement it.
 */
void freeDendrogram(Dendrogram d) {
	// // TODO: Implement this function
	if (d == NULL) 
		return;
	
	freeDendrogram(d->left);
	freeDendrogram(d->right);

	free(d);
}



// Removes the highest centrality edge given a graph and its edge values
static void removeHighestCentralityEdges(Graph g, EdgeValues e) {
	int numNodes = GraphNumVertices(g);

	// Finding highest centrality edge
	double highestCentrality = -1.0;
	for (int i = 0; i < numNodes; i++) {
		for (int j = 0; j < numNodes; j++) {
			if (e.values[i][j] > highestCentrality) {
				highestCentrality = e.values[i][j];
			}
		}
	}
	// Removing highest centrality edge
	for (int i = 0; i < numNodes; i++) {
		AdjList curr = GraphOutIncident(g, i);
		while (curr != NULL) {
			if (e.values[i][curr->v] == highestCentrality) {
				AdjList temp = curr;
				curr = curr->next;
				GraphRemoveEdge(g, i, temp->v);
			}
			else{
				curr = curr->next;
			}	

		}
	}
}

// If a seperation has occured and 2 communities have been formed returns true otherwise false
static bool seperation(ShortestPaths sps, ShortestPaths removedsps, int i, int j) {
	if ((sps.next[i][j] != -1 || sps.next[j][i] != -1) 
	&& (removedsps.next[i][j] == -1 && removedsps.next[i][j] == -1)) {
		return true;
	}
	return false;
}


// Checks if given vertex has no edges connected to it in given graph
static bool singleVertex(ShortestPaths sps, Vertex i) {
	bool res = true;;
	for (int j = 0; j < sps.numNodes; j++) {
		if (sps.next[i][j] != -1 || sps.next[j][i] != -1) {
			res = false;
		}		
	}
	return res;
}


// Creates new Graph after removal of edge
static Graph CreateNewGraph(Graph g, ShortestPaths sps, int value) {
	int numNodes = GraphNumVertices(g);
	Graph new = GraphNew(numNodes);
	// Adding inlinks
	for (int i = 0; i < numNodes; i++) {
		// If there is a connection with given value then add to graph
		if (sps.next[i][value] != -1 && sps.next[value][i] != -1) {
			AdjList currIn = GraphOutIncident(g, i);
			while (currIn != NULL) {
				GraphInsertEdge(new, i, currIn->v, currIn->weight);
				currIn = currIn->next;
			}
		}
	}
	// Adding outlinks
	AdjList currOut = GraphOutIncident(g, value);
	while (currOut != NULL) {
		GraphInsertEdge(new, value, currOut->v, currOut->weight);
		currOut = currOut->next;
	}
	return new;
}

static void FreeingEverything(Graph left, Graph right, EdgeValues e, ShortestPaths removedsps, ShortestPaths sps) {
	GraphFree(left);
	GraphFree(right);
	freeEdgeValues(e);	
	freeShortestPaths(removedsps);
	freeShortestPaths(sps);
}