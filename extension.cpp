/**
 * Name: Haiyu Huang, Kyle Chang
 * Date: November 13, 2016
 * CSE 100 Fall 2016 
 * PA4: Graph Theory 
 * File: pathfinder.cpp
 * Description: Finds shortest path between two actors
 *
 */ 

#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Graph.h"

#define NUM_ARGS 3
using namespace std;


int main(int argc, const char ** argv){
    // input parameters
    const char * dataSource = argv[1];
    const char * out_filename = argv[2];
    
    // Checks for valid number of inputs
    if (argc != NUM_ARGS) {
        cerr << "Wrong number of inputs into extension" << endl;
    return -1;

    }

    Graph graph;                      // the graph data structure
    vector<pair<int,int>> dataSet;    // data of retweets

    
    // Initialize the file stream
    ifstream infile(dataSource);
    ofstream outfile(out_filename);


    // keep reading lines until the end of file is reached
    while (infile) {
        int first;

        infile >> first;
         
        int second;
        
        infile >> second;
		if (infile.eof()) break;

        // stores inputs 
        dataSet.push_back(make_pair(first, second));
    }

    // create the node set
    for (auto & pair : dataSet){
        if ( !(graph.nodeSet[pair.first]) )
           graph.nodeSet[pair.first] = new Node (pair.first); 

        if ( !(graph.nodeSet[pair.second]) )
           graph.nodeSet[pair.second] = new Node (pair.second); 

    }
    

	
	

    // connect the edges
	graph.createGraph(dataSet);


	// initialize a stack of vertices
	stack<int> vertices;
	for (auto & pair: graph.nodeSet)
		vertices.push(pair.first);

	// DFS on the original graph
	stack<int> finished = graph.DFSTraversal(vertices);


	
	graph.clearGraph();
	Graph transpose(graph);		// the transposed graph
	transpose.createTransposeGraph(dataSet);

	auto tree = transpose.DFSTraversalSCC(finished); //find SCC in transpose
    
    // prints to outfile
	for (const vector<int>& vector: tree){
		for (int num : vector)
			outfile << num << " ";
		outfile << "\n";
	}

	// run algorithm to generate the strongly connected component
    


    if (!infile.eof()) {
        cerr << "Failed to read " << dataSource << "!\n";
        return false;
    }
    
    // closes streams 
    infile.close();
    outfile.close();

	// setting pointer to null to prevent double deletion
	for (auto& pair : transpose.nodeSet)
		pair.second = NULL;


    return 0;

}
