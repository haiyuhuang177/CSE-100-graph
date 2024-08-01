/**
 * Name: Haiyu Huang, Kyle Chang
 * Date: November 22, 2016
 * CSE 100 Fall 2016 
 * PA4: Graph Theory 
 * File: Node.h
 * Description: Class and member definitions for Node of user
 *
 */ 


#ifndef NODE_H
#define NODE_H

#include <vector>
#include <cstring>
#include <climits>

using namespace std;

/**
 * Class definition for Node which represents each user.
 */ 
class Node{
	friend class Graph;
private:
    bool visited;            // status of the node in Dijkstra's Algorithm
public:
    vector<int>  edgeSet; // the adjacency list
    int userID;           // Id of the user
    /** 
     * constructor for an Node object
     */ 
    Node(int ID){
        userID = ID;
		visited = false;
    }
    
    /**
     * Overloaded equality operator for ActorNode objects
     */ 
    long operator == (const Node& other){
        return userID == other.userID;

    }
};




#endif // NODE_H
