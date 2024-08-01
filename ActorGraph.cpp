/*
 * ActorGraph.cpp
 * Author: Haiyu Huang, Kyle Chang
 * Date:   November 13, 2016
 *
 * This file is meant to exist as a container for starter code that 
 * you can use to read the input file format defined in movie_casts.tsv.
 * Feel free to modify any/all aspects as you wish.
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack> 
#include <climits>
#include <utility>
#include <unordered_map>
#include "ActorGraph.h"
#include "Movie.h"

using namespace std;

/**
 * Defaut ActorGraph constructor
 */ 
ActorGraph::ActorGraph(void) {}

/**
 * ActorGraph destructor
 */ 
ActorGraph::~ActorGraph(void){
	for (auto & node : actorSet)
			delete node;
}

/**
 * Initializes node fields, distance and prev to correct starting values
 */ 
void ActorGraph:: initialize() {
	for (auto & actor : actorSet){
		actor->dist = INT_MAX;
		actor->prev = -1;
		actor->done = false;
	}
}

/** 
 * Locates ActorNode inside the graph given the name of the actor
 * Parameters:
 *      name: Name of actor
 *
 * Return: Index of actor     
 */ 
int ActorGraph:: locateActor ( string name ){
    for (ActorNode *& actor : actorSet){
        if (actor->actor_name == name)
            return actor->index;
    }
}


/**
 * Prints the shortest distance and the movies connecting two actors.
 * Parameters:
 *      fromActor: index of Actor to print connection from
 *      toActor: index of Actor connected to fromActor
 *      outfile: Stream to output results
 *      use_weighted_edges: Use dijkstra or BFS 
 *
 */
void ActorGraph::printConnections (int fromActor, int toActor, 
                                ofstream& outfile, bool use_weighted_edges){

    ActorNode * from = actorSet[fromActor]; // locate the starting vertex
    int fromIndex = from->index;               // index of the starting vertex
    ActorNode * to = actorSet[toActor];     // locate the ending vertex

    stack<ActorNode *> explore; // use stack to reverse path traversal to print

    explore.push(to);
	
	// validity check for connectedness
	if ( to->prev == -1)
		outfile << "Actors/actresses not connected!\n";

    // going back from the fromActor to toActor
    ActorNode * current = to;
    while ( current->prev != fromIndex ){
        explore.push(actorSet[current->prev]);
        current = actorSet[current->prev];
    }
    explore.push(from);

    Movie movie; // Movie object

   
   // Printing the order of movies linking actors in reverse with stack
   while ( explore.size() > 1){
        ActorNode * curr = explore.top();
        explore.pop();
        ActorNode * next = explore.top();

        outfile << "(" << curr->actor_name << ")" << "--";
		if (use_weighted_edges){
			Movie max={"", 0}; 
			for (pair<int,Movie>& pairs : curr->edgeSet){
				if (pairs.first == next->index){
					if (pairs.second.movie_year > max.movie_year)
						movie = pairs.second;
						max = movie;
				}
			}

		}
		else{
			for (pair<int,Movie>& pairs : curr->edgeSet){
				if (pairs.first == next->index){
					movie = pairs.second;
					break;
				}
			}
        }

        outfile << "[" << movie.movie_title << "#@" 
                                                << movie.movie_year << "]-->";

    }
    outfile << "(" << actorSet[toActor]->actor_name << ")\n";

}


/** 
 * Creates the edges and fills the member fields of each node inside 
 * the map such that nodes are connected with the shortest unweighted
 *  distances
 *  Parameters:
 *      fromActor: index of Actor to start BFS traversal from
 *      toActor: index of Actor connected to fromActor
 *
 */ 
void ActorGraph::BFSTraversal(int fromActor, int toActor){

	initialize();			// initialize the dist to infinity and prev to -1
    ActorNode * from = actorSet[fromActor]; // starting ActorNode 
    queue<ActorNode *> explore;                // queue to do BFS
    from->dist = 0;              // initialize starting ActorNode's dist field
    explore.push(from);            // push the starting ActorNode to the queue

    //perform the breath first search
    while (!explore.empty() ) {
       
       ActorNode * curr = explore.front();
       explore.pop(); 
       auto it = curr->edgeSet.begin(); 
       for ( ; it != curr->edgeSet.end(); ++it) {
           ActorNode* neighbor = actorSet[it->first];
           // make sure no cycles
           if (curr->dist + 1 < neighbor->dist){
               neighbor->dist = curr->dist + 1;
               neighbor->prev = curr->index;
               explore.push(neighbor);
           }
		   
		   // Returns from method once neighboring actor matches 
		   if (neighbor->actor_name == actorSet[toActor]->actor_name){
			   return;
		   }
       }    
    }   
}

/** 
 * Creates the edges and fills the member fields of each node inside 
 * the map such that nodes are connected with the shortest weighted
 * distances
 * Parameters:
 *      fromActor: index of Actor to start Dijkstra traversal from
 *      toActor: index of Actor connected to fromActor
 *
 */
void ActorGraph::Dijkstra(int fromActor, int toActor){

	initialize();           // Initializes node fields to default values
    ActorNode * from = actorSet[fromActor]; // start Node to search from
	priority_queue<pair <int,int>,vector<pair<int,int>>, ActorNodeComp> pqueue;
	pqueue.push(make_pair(from->index, 0));
    
    // perform DIjstra algorithm
	while (!pqueue.empty()){
		int curr = pqueue.top().first;
		pqueue.pop();
		
		if (! (actorSet[curr]->done)){
			actorSet[curr]->done = true;
			for (auto & pair : actorSet[curr]->edgeSet){
				int neighbor = pair.first;
				int weight = 2016 - pair.second.movie_year;
				int distance = actorSet[curr]->dist + weight;
				if (distance < actorSet[neighbor]->dist){
					actorSet[neighbor]->prev = curr;
					actorSet[neighbor]->dist = distance;
					pqueue.push(make_pair(neighbor, distance));
				}
			}
			if (actorSet[curr]->actor_name == actorSet[toActor]->actor_name)
				return;
		}
	}
	

}


/** 
 * Load the graph from a tab-delimited file of actor->movie 
 * relationships.
 * Parameters:
 *      in_filename - input filename
 *
 * Return: true if file was loaded sucessfully, false otherwise
 */
bool ActorGraph::loadFromFile(const char* in_filename,
                                     unordered_map<Movie,vector<int>>& map) {
    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;

    string currentActor; // current actor in the set
    int counter = 0; // index of actor in the set

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
      
            // gets next string before hitting tab character to put in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
    
        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);
        

        ActorNode * actor; // the actor in the current movie 
        // push actor to set of ActorNodes if actor has not appeared in set yet
        if (currentActor != actor_name){
            currentActor = actor_name;
            actor = new ActorNode(actor_name);
            actor->index = counter;
            actorSet.push_back(actor);   
            counter++;
        }

        // add the <movie, actor> pair into the hashmap
        Movie movie = {movie_title, movie_year};
        map[movie].push_back(counter - 1); 
    }
	
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();
	return true;
}

/**
 * Creates entire graph at once for pathfinder
 * Parameters:
 *      map: Map with pairs of Movie and vector of Actor indices
 *
 */
void ActorGraph::createGraph(unordered_map<Movie, vector<int>>& map){
    // iterate through the hash map
    for (auto & pair: map){
        // the set of actors played in this movie
        int number = pair.second.size();
        // connect them all
        for (int i = 0; i < number; i++){
            int actorIndex = pair.second[i];
            for (int j = 0; j < number; j++){
                // not connect to him/herself
                if (j != i ){
                    // add edge into the edgeSet of the current actor
                    actorSet[actorIndex]->edgeSet.push_back(make_pair(
                                                pair.second[j], pair.first)); 
                }

            }
        }

    }
}


/**
 * Creates graph year by year for actorconnections
 * Parameters:
 *      map: Map with pairs of Movie and vector of Actor indices
 *      movie: Movie reference to connect actors who acted in it
 *
 */    
void ActorGraph::createGraph(unordered_map<Movie, vector<int>>& map, 
                                                const Movie * const& movie){
        // the set of actors played in this movie
		auto actors = map[*movie];
        int number = actors.size();
        // connect them all
        for (int i = 0; i < number; i++){
            int actorIndex = actors[i];
            for (int j = 0; j < number; j++){
                // not connect to him/herself
                if (j != i ){
                    // add edge into the edgeSet of the current actor
                    actorSet[actorIndex]->edgeSet.push_back(make_pair(
                                                        actors[j], *movie)); 
                }

            }
        }

}

/**
 * Checks if two actors are connected
 * Parameters: 
 *      fromActor: index ofActor to start search from
 *      toActor: index of Actor connected to
 *
 * Return: True or false if two actors are connected or not
 */  
bool ActorGraph::testConnectivity(int fromActor, int toActor){
	BFSTraversal(fromActor, toActor);
	return (actorSet[toActor]->prev != -1);

}

