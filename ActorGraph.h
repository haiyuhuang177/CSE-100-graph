/*
 * ActorGraph.h
 * Author: Haiyu Huang, Kyle Chang
 * Date:    November 13, 2016
 * 
 * Defines classes for ActorNodeComp, MovieComp, and ActorGraph. The first two
 * classes are used as functional classes to compare ActorNode and Movie 
 * objects respectively. The ActorGraph class defines the member methods to 
 * update and traverse the graph. 
 *
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "ActorNode.h"

using namespace std;

/** A 'function class' for use as the Compare class in a
 *  priority_queue<pair <int, int>>.
 *  First integer: index of the ActorNode
 *  Second integer: path cost
 */
class ActorNodeComp {
    
	public:
       /** 
        * Overloaded compare operator to compare actor nodes
        * Parameters:
        *       lhs: left hand actor pair
        *       rhs: right hand actor pair
        * 
        * Return: True if lhs is greater, false if otherwise
        */ 
		bool operator()(pair<int,int> const& lhs,
                                             pair<int,int> const& rhs) const {
			return lhs.second > rhs.second;
		}
};

/**
 * A function class to compare movies. 
 * Parameters:
 *      lhs: left hand movie reference
 *      rhs: right hand movie reference
 *
 * return: True if lhs is lesser, false if otherwise
 */ 
class MovieComp {
	public:
        /**
         * Overloaded compare operator to compare movies
         * Parameters:
         *      lhs: left hand movie reference
         *      rhs: right hand movie reference
         * 
         * Return: True is lhs movie_year is smaller, false if otherwise
         */      
		bool operator()(const Movie* const& lhs, 
                                            const Movie* const& rhs) const {
			return lhs->movie_year < rhs->movie_year;
		}
};

/**
 * A class that defines all the member methods related to the graph of 
 * ActorNodes.
 *
 */ 
class ActorGraph {
	friend class UnionFind;  // allows access to UnionFind private variables
	private:
		/**
		 * Initializes the member fields of nodes to correct default value
		 */ 
		void initialize();


	public:
		vector<ActorNode *> actorSet;//underlying data structure for ActorGraph
		
        /**
		 * default constructor for ActorGraph object
		 */ 
		ActorGraph(void);
        
        /**
         * ActorGraph destructor
         */ 
		~ActorGraph(void);

		/** 
		 * Locates ActorNode inside the graph given the name of the actor
		 * Parameters:
		 *      name: Name of actor
		 *
		 * Return: Index of actor     
		 */ 
		int locateActor ( string name );

		/** 
		 * Load the graph from a tab-delimited file of actor->movie 
		 * relationships.
		 * Parameters:
		 *      in_filename - input filename
		 *
		 * Return: true if file was loaded sucessfully, false otherwise
		 */
		bool loadFromFile(const char* in_filename, unordered_map<Movie,
                                                            vector<int>>& map);
        
        /**
         * Checks if two actors are connected
         * Parameters: 
         *      fromActor: index ofActor to start search from
         *      toActor: index of Actor connected to
         *
         * Return: True or false if two actors are connected or not
         */  
		bool testConnectivity(int fromActor, int toActor);

        /**
         * Creates entire graph at once for pathfinder
         * Parameters:
         *      map: Map with pairs of Movie and vector of Actor indices
         *
         */
		void createGraph(unordered_map <Movie, vector<int>>& map);

        /**
         * Creates graph year by year for actorconnections
         * Parameters:
         *      map: Map with pairs of Movie and vector of Actor indices
         *      movie: Movie reference to connect actors who acted in it
         *
         */       
		void createGraph(unordered_map <Movie, vector<int>>& map,
                                             const Movie * const&  movie);
		
		/** 
		 * Creates the edges and fills the member fields of each node inside 
		 * the map such that nodes are connected with the shortest unweighted
		 *  distances
		 *  Parameters:
		 *      fromActor: index of Actor to start BFS traversal from
		 *      toActor: index of Actor connected to fromActor
		 *
		 */ 
		void BFSTraversal(int fromActor, int toActor);

		/** 
		 * Creates the edges and fills the member fields of each node inside 
		 * the map such that nodes are connected with the shortest weighted
		 * distances
		 * Parameters:
		 *      fromActor: index of Actor to start Dijkstra traversal from
		 *      toActor: index of Actor connected to fromActor
		 *
		 */
		void Dijkstra(int fromActor, int toActor);

		/**
		 * Prints the shortest distance and the movies connecting two actors.
		 * Parameters:
		 *      fromActor: index of Actor to print connection from
		 *      toActor: index of Actor connected to fromActor
		 *      outfile: Stream to output results
		 *      use_weighted_edges: Use dijkstra or BFS 
		 *
		 */
		void printConnections(int fromActor, int toActor, ofstream & outfile,
                                                     bool use_weighted_edges);
};

#endif // ACTORGRAPH_H
