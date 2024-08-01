/*
 * UnionFind.hpp
 * Author: Haiyu Huang, Kyle Chang
 * Date:   November 13, 2016
 * 
 * This file is meant to declare and define member fields and member methods 
 * used to perform ufind in actor connections
 */
#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP

#include <vector>
#include <algorithm>
#include "ActorGraph.h"


using namespace std;

/**
 * Class to define member variables and methods related to the UnionFind data
 * structure used in actor connections.
 *
 */   
class UnionFind{
	friend class ActorGraph; // Has access to private members of ActorGraph 

	pair<int,int> * dataArray; // a pair of index of node/size of set it's in
	int arrayLength;		   // length of the dataArray

public:
    /** 
     * Constructor for UnionFind object
     * Parameters: 
     *      graph: Graph to make UnionFind data structure from
     */ 
	UnionFind(ActorGraph& graph);

	/**
     * Destructor for UnionFind objects
     */   
    ~UnionFind();

    /**
     * Finds set based on node inputed
     * Parameters:
     *      node: Index of actor node to find
     *
     * Return: Sentinel Node index of set node is in. 
     */ 
	int find(int node);

    /**
     * Finds which set object is inside based on node. 
     * Parameters:
     *      node: Index of actor to find
     *
     * Return: Sentinel of set which actor is in   
     */ 
	void merge(int left, int right);

};

/**
 * Destructor for UnionFind object
 */ 
UnionFind::~UnionFind(){
	delete dataArray;
}

/**
 * Constructor for UnionFind object
 * Parameters:
 *      graph: Graph to make UnionFind data structure from 
 */ 
UnionFind::UnionFind(ActorGraph& graph){
    
    // Creates set, and fills arrayLength items after dataArray with default values.
	arrayLength = graph.actorSet.size();
	dataArray = new pair<int,int>[arrayLength];
	fill_n (dataArray, arrayLength, make_pair(-1,1));
}

/**
 * Finds which set object is inside based on node. 
 * Parameters:
 *      node: Index of actor to find
 *      
 * Return: Sentinel of set which actor is in   
 */ 
int UnionFind::find(int node){
	// for path compression to attach all nodes passed to the sentinel nodes
	vector<int> toAttach;	

	// go up to search for the sentinal node
	int curr = node;

	// go up the up-tree to find the sentinel node
	while ( dataArray[curr].first != -1 ){
		toAttach.push_back(curr);
		curr = dataArray[curr].first;
	}

	// path compression
	for (int num: toAttach){
		dataArray[num].first = curr;
	}	

	return curr;


}

/**
 * Merges two sets. Left and right are nodes from sets.
 * Parameters: 
 *      left: Index of Actor
 *      right: Index of Actor 
 */ 
void UnionFind::merge(int left, int right){
	// find the left and right sentinel node respectively
	int leftSentinel = find(left);
	int rightSentinel = find(right);

	// already merged
	if (leftSentinel == rightSentinel)
		return;

	// update size
	int sum = dataArray[leftSentinel].second + dataArray[rightSentinel].second;

	// compare size of sets
	if (dataArray[leftSentinel].second < dataArray[rightSentinel].second){
		dataArray[leftSentinel].first = rightSentinel;
		dataArray[rightSentinel].second = sum;
	}
	else{
		dataArray[rightSentinel].first = leftSentinel;
		dataArray[leftSentinel].second = sum;
	}
}


#endif // UNIONFIND_HPP
