/*
 * Graph.h
 * Author: Haiyu Huang, Kyle Chang
 * Date:    November 23, 2016
 *
 * Contains all graph method declarations for extension project.
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <unordered_map>
#include "Node.h"

using namespace std;

/**
 * A function class to compare movies. 
 * Parameters:
 *      lhs: left hand movie reference
 *      rhs: right hand movie reference
 *
 * return: True if lhs is lesser, false if otherwise
 */ 
class vectorComp {
	public:
        /**
         * Overloaded compare operator to compare movies
         * Parameters:
         *      lhs: left hand movie reference
         *      rhs: right hand movie reference
         * 
         * Return: True is lhs movie_year is smaller, false if otherwise
         */      
		bool operator()(const vector<int>& lhs, 
                                            const vector<int>& rhs) const {
			return lhs.size() > rhs.size();
		}
};

/** 
 * Class for graph data structure used in extension
 */ 
class Graph {
    friend class Node;
	private:


	public:
		 unordered_map<int, Node *> nodeSet;     //underlying data structure for ActorGraph
		
        /**
		 * Default constructor for graph object
		 */ 
		Graph(void);

        /**
         * Graph constructor
         * Parameters: Hash map of all Node objects 
         *
         */ 
		Graph(const Graph& graph);
        
        /**
         * Destructor for graph
         */ 
		~Graph(void);


        /**
         * Clears edgeset of each pair before creating transpose
         */         
		void clearGraph();

        /**
         * Creates entire graph at once for pathfinder
         * Parameters:
         *      map: Map with pairs of Movie and vector of Actor indices
         *
         */
		void createGraph( vector<pair<int,int> >& dataSet);

        /**
         * Creates transpose graph to find SCC
         * Parameters:
         *      dataSet: Edges read read from file
         *
         */ 		
		void createTransposeGraph( vector <pair<int,int> >& dataSet);

        /**
         * Visits current vertex and runs DFS from vertex. Called from DFS 
         * traversal
         * Parameters:
         *      vertex: Current vertex being visited
         *      finished: Stack of verticies already visited
         *
         */ 
		void DFSVisit(int vertex, stack<int>& finished);

        /**
         * Visits current vertex and runs DFS from vertex. Called from DFS 
         * traversal
         * Parameters:
         *      vertex: Current vertex being visited
         *      finished: Stack of verticies already visited
         *
         */ 
		void DFSVisit(int vertex, stack<int>& finished, vector<int>& currentSCC);
		
        /**
         * DFS traversal to find every vertex in graph. 
         * Parameters:
         *      vertices: stack of vertices to visit
         * 
         * Return: Stack of finished visited vertices
         */   
		stack<int> DFSTraversal(stack<int>& vertices);

        /**
         * DFS traversal to find strongly connected components
         * Parameters: 
         *      vertices: Reference to stack of vertices to visit
         * 
         * Return: multiset of vertices with its strongly connected components
         */ 

		multiset<vector<int>, vectorComp> DFSTraversalSCC(stack<int>& vertices);

};

#endif // GRAPH_H
