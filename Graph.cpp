/*
 * Graph.cpp
 * Author: Haiyu Huang, Kyle Chang
 * Date:   November 13, 2016
 *
 * This file is meant to exist as a container for starter code that 
 * you can use to read the input file format defined in movie_casts.tsv.
 * Feel free to modify any/all aspects as you wish.
 */
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <stack> 
#include <climits>
#include <unordered_map>
#include "Graph.h"

using namespace std;

/**
 * Default Graph constructor
 */ 
Graph::Graph(void) {}

/**
 * Graph copy constructor
 * Parameters: A graph of which the nodeSet is copied 
 *
 */ 

Graph::Graph(const Graph& graph){
	nodeSet = graph.nodeSet;
}

/**
 * Graph destructor
 */ 
Graph::~Graph(void){
	for (auto& node : nodeSet){
		if (node.second){ 
			delete node.second;
		}		
	}
}

/**
 * DFS traversal to find every vertex in graph. 
 * Parameters:
 *      vertices: stack of vertices to visit
 * 
 * Return: Stack of finished visited vertices
 */ 
stack<int> Graph::DFSTraversal(stack<int>& vertices){
	stack<int> finished;			// a stack of visited node
	while (!vertices.empty()){
		int node = vertices.top();
		vertices.pop();

		if (!(nodeSet[node]->visited))
			DFSVisit(node, finished);

	}
	return finished;

}


/**
 * DFS traversal to find strongly connected components
 * Parameters: 
 *      vertices: Reference to stack of vertices to visit
 * 
 * Return: multiset of vertices with their strongly connected components
 */   
multiset<vector<int>, vectorComp> Graph::DFSTraversalSCC(stack<int>& vertices){
	multiset<vector<int>, vectorComp> scc;			// a stack of visited node
	stack<int> finished;
	while (!vertices.empty()){
		vector<int> currentSCC;
		int node = vertices.top();
		vertices.pop();

		if (!(nodeSet[node]->visited))
			DFSVisit(node, finished, currentSCC);

		if (currentSCC.size())
		
        
		if (!(nodeSet[node]->visited)) { 
			DFSVisit(node, finished, currentSCC);
        }
        
		if (currentSCC.size()) {
			scc.insert(currentSCC);
        }
        
	}
	return scc;

}

/**
 * Visits current vertex and runs DFS from vertex. Called from DFS traversal
 * Parameters:
 *      vertex: Current vertex being visited
 *      finished: Stack of verticies already visited
 *
 */ 
void Graph::DFSVisit(int vertex, stack<int>& finished){
	nodeSet[vertex]->visited = true;
	for (int neighbor : nodeSet[vertex]->edgeSet){
		if (!(nodeSet[neighbor]->visited))
			DFSVisit(neighbor,finished);
	}

	finished.push(vertex);
}

/**
 * Visits current vertex and runs DFS from vertex. Called form DFStraversalSCC
 * Parameters:
 *      vertex: current vertex being visited
 *      currentSCC: current strongly connected component being searched 
 *    
 */ 
void Graph::DFSVisit(int vertex, stack<int>& finished,vector<int>& currentSCC){

	currentSCC.push_back(vertex);

	nodeSet[vertex]->visited = true;
	for (int neighbor : nodeSet[vertex]->edgeSet){
		if (!(nodeSet[neighbor]->visited))
			DFSVisit(neighbor,finished, currentSCC);
	}

	finished.push(vertex);
}
/**
 * Creates entire graph at once for pathfinder
 * Parameters:
 *      map: Map with pairs of Movie and vector of Actor indices
 *
 */
void Graph::createGraph(vector<pair<int,int>>& dataSet){
	for (auto& pair : dataSet){
		nodeSet[pair.first]->edgeSet.push_back(pair.second);
	}
}

/**
 * Creates transpose graph to find SCC
 * Parameters:
 *      dataSet: Edges read read from file
 *
 */  
void Graph::createTransposeGraph(vector<pair<int,int>>& dataSet){
	for (auto& pair : dataSet){
		nodeSet[pair.second]->edgeSet.push_back(pair.first);
	}
}

/**
 * Clears edgeset of each pair before creating transpose
 */ 
void Graph::clearGraph(){
	for (auto& pair : nodeSet){
		pair.second->edgeSet.clear();
		pair.second->visited = false;
	}
}

