/**
 * Name: Haiyu Huang, Kyle Chang
 * Date: November 13, 2016
 * CSE 100 Fall 2016 
 * PA4: Graph Theory 
 * File: ActorNode.h
 * Description: Class and member definitions for Actor Object
 *
 */ 


#ifndef ACTORNODE_H
#define ACTORNODE_H

#include <vector>
#include <cstring>
#include <climits>
#include "Movie.h"

using namespace std;

/**
 * A class that defines the member variables, constructors and methods for
 * the ActorNode objects stored in the graph. 
 *
 */  
class ActorNode{
	friend class ActorGraph;
private:
    vector< pair<int, Movie> > edgeSet; // the adjacency list
    int dist;          // the distance from the source
    int prev;          // the index of the vertex previous in the path 
    bool done;         // status of the node in Dijkstra's Algorithm

public:
    string actor_name; // name of the actor
    int index;         // the index of the actorNode
    /** 
     * constructor for an ActorNode object
     */ 
    ActorNode(string name){
        actor_name = name;
        dist = INT_MAX;
        prev = -1;
        index = -1;
		done = false;
    }
    
    /**
     * Overloaded equality operator for ActorNode objects
     */ 
    long operator == (const ActorNode& other){
        return actor_name == other.actor_name;

    }
};




#endif // ACTORNODE_H
