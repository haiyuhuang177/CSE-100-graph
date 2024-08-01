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
#include <sstream>
#include "ActorGraph.h"
#define NUM_ARGS 5
using namespace std;

int main(int argc, const char ** argv){
    // input parameters
    const char * movie_casts = argv[1];
    const char * edgeOption = argv[2];
    const char * in_filename = argv[3];
    const char * out_filename = argv[4];
    
    // Checks for valid number of inputs
    if (argc != NUM_ARGS) {
        cerr << "Wrong number of inputs into pathfinder" << endl;
    return -1;

    }
    
    // checks for valid input option
    if ( strcmp(edgeOption, "w") && strcmp(edgeOption, "u")) {
        cerr << "Argument 2 ( edge weight option ) invalid " << endl;
		return -1;
    }
    
    bool use_weighted_edges = ( !strcmp(edgeOption, "w") ); // option 
    ActorGraph graph;                                       // graph
	unordered_map<Movie, vector <int>> map; // maps movie to all actors in movie
    graph.loadFromFile(movie_casts, map);   // loads graph to map
	graph.createGraph(map);     // creates graph from map 

    
    // Initialize the file stream
    ifstream infile(in_filename);
    ofstream outfile(out_filename);
    bool have_header = false;


	// output header
	outfile << "(actor)--[movie#@year]-->(actor)--...\n";

    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;
      
            // get the next string before hitting a tab character and
            // put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
    
        if (record.size() != 2) {
            // we should have exactly 2 columns
            continue;
        }
        
        // stores inputs 
        string fromActor(record[0]);
        string toActor(record[1]);

		// index of the actors in the actor set
		int fromIndex = graph.locateActor(fromActor);
		int toIndex = graph.locateActor(toActor);

		cerr << "Computing path for (" << fromActor << ") -> (" << toActor
                                                             << ")" << endl;
        // Runs corresponding algorithm
        if (use_weighted_edges){
            graph.Dijkstra(fromIndex, toIndex);
			graph.printConnections(fromIndex, toIndex, outfile, true);
        }
        else{ 
            graph.BFSTraversal(fromIndex, toIndex);
			graph.printConnections(fromIndex, toIndex, outfile, false);
        }
        
        // Prints all the movies and actors connecting two actors
    }


    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    
    // closes streams 
    infile.close();
    outfile.close();


    return 0;

}
