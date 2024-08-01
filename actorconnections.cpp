/**
 * Name: Haiyu Huang, Kyle Chang
 * Date: November 13, 2016
 * CSE 100 Fall 2016 
 * PA4: Graph Theory 
 * File: actorconnections.cpp
 * Description: Calculates after how many years two actors become connected.
 *
 */ 

#include <chrono>
#include <ctime>
#include <fstream>
#include <sstream>
#include <set>
#include <tuple>
#include "ActorGraph.h"
#include "UnionFind.hpp"
#define NUM_ARGS 5
using namespace std;

int main(int argc, const char ** argv){
	// input parameters from command line
	const char * movie_casts = argv[1];
	const char * in_filename = argv[2];
	const char * out_filename = argv[3];
	
	// timer 
	std::chrono::time_point<std::chrono::system_clock> start, end;
	bool use_union;           // algorithmic implementation

	// default to union if the forth argument is not given
	if (argc == 4) {
		use_union = true;
	}
	// choose algorithm depending on the forth input
	else{
		const char * algorithm = argv[4];

		if ( strcmp(algorithm, "ufind") && strcmp(algorithm, "bfs")) {
			cerr << "Argument 4 (algorithm) invalid " << endl;
			return -1;
		}
		use_union = ( !strcmp(algorithm, "ufind") );
	}



	
	int year_connected = 9999;		// default year
	ActorGraph graph;				// graph
	unordered_map<Movie, vector <int>> map;	// maps movie to actors in movie
	multiset<const Movie *, MovieComp> tree;    // sort movie based on year
	vector<tuple<int, int, int>> actorList;		// list of input actor pairs

	// load graph to the map
	graph.loadFromFile(movie_casts, map);

	// order movies by year using multiset 
	for (auto & pair: map){
		tree.insert( &(pair.first) );
	}

	// Initialize the file stream
	ifstream infile(in_filename);
	ofstream outfile(out_filename);
	bool have_header = false;

	// output header
	outfile << "Actor1	Actor2	Year\n";

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

			// gets next string before hitting tab character and puts in 'next'
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

		// store the actor pairs with default year in the set of tuples
		actorList.push_back( make_tuple(fromIndex, toIndex, year_connected) );

	}

    // Runs corresponding algorithm
	if (use_union){
		cerr << "Running Union Find" << endl; 

		// begin timer
        start = std::chrono::system_clock::now(); 

		// invokes the union find data structure
        UnionFind disjointSet(graph);

		int year = (*(tree.cbegin()))->movie_year;   // initial year
		for (auto it = tree.cbegin(); it != tree.cend();){
			// connect all actors played in same movie movies in current year
			while ( year == (*it)->movie_year ){
				vector<int> actors = map[**it];
				// merge pairs by pairs
				for (int i = 0; i < actors.size() - 1; i++){
					disjointSet.merge(actors[i], actors[i+1]);
				}
				++it;
				if (it == tree.cend())
					break;
			}
			// if connected, update tuple list and remove it from current vector
			for (auto & tuple : actorList){
				// already connected in previous year, move on
				if (year >= get<2>(tuple)){
					continue;
				}
				// test if the actor pair is connected in the year
				if ( disjointSet.find(get<0>(tuple)) == disjointSet.find(
                                                                get<1>(tuple)))
					get<2>(tuple) = year;
			}

			year++;
		}

		// end timer
	    end = std::chrono::system_clock::now();
        std::chrono::duration<double> ms = end - start;
        cerr << "The duration in seconds was " << ms.count() << endl;
    }
	else{	
 		cerr << "Running BFS" << endl;

		// begin timer
        start = std::chrono::system_clock::now(); // tracks duration
   	
        int year = (*(tree.cbegin()))->movie_year;   // initial year
		for (auto it= tree.cbegin(); it!= tree.cend();){
			// connect actors that played in same movie movies in current year
			while ( year == (*it)->movie_year ){
				// connect the edges with the map for the current movie
				graph.createGraph(map, *it);
				++it;
				if (it == tree.cend())
					break;
			}

			// if connected, updates tuple list/removes it from current vector
			for (auto & tuple : actorList){
				// already connected in previous year, move on
				if (year >= get<2>(tuple))
					continue;
				// test if the actor pair is connected in the year
				if ( graph.testConnectivity(get<0>(tuple), get<1>(tuple))){
					// update the tuple
					get<2>(tuple) = year;
				}
			}

			year++;
		}

		// end timer
 	    end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        cerr << "The duration in seconds was " << elapsed_seconds.count() << endl;
            
	}
    
    // prints output to outfile
	for (auto & tuple: actorList){
		outfile << graph.actorSet[get<0>(tuple)]->actor_name << "\t" <<
        graph.actorSet[get<1>(tuple)]->actor_name << "\t" << get<2>(tuple) <<
                                                                         "\n";
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
