#include "AllPair.h"
#include <vector>
#include <stdlib.h>
#include <iostream>
#define INT_MAX 999999
using namespace std;

Graph::Graph(int size){
    size_ = size;
    for(int i = 0; i<size; i++){
        AdjList* new_adjlist = new AdjList;
        vertices_.push_back(new_adjlist);
    }
}

void Graph::addEdge(int src, int dest, int weight ){
    ListNode* new_node = new ListNode(src, dest, weight);
    vertices_[src]->push_back(new_node);
}

void MinHeap::swapNodes(int a, int b){


    HeapNode* temp = nodes[a];
    nodes[a] = nodes[b];
    nodes[b] = temp;

}

void MinHeap::minHeapify(int idx){
	int smallest = idx;
	int left = 2*idx+1;
	int right = 2*idx+2;

	if(left<filled_ && nodes[left]->dist_ < nodes[smallest]->dist_){
		smallest = left;
	}
	if(right<filled_ && nodes[right]->dist_ < nodes[smallest]->dist_){
		smallest = right;
	}

	if(smallest!=idx){
		HeapNode * small_nd = nodes[smallest];
		HeapNode * idx_nd = nodes[idx];

		vertex_to_idx[small_nd->vid_] = idx;
		vertex_to_idx[idx_nd->vid_] = smallest;

		swapNodes(smallest, idx);

		minHeapify(smallest);
	}
}

HeapNode* MinHeap::getMin(){
	if(isEmpty()){
    	return NULL;
	}
	
	HeapNode* root = nodes[0];
	HeapNode* last_nd = nodes[filled_-1];

	nodes[0] = last_nd;

	vertex_to_idx[root->vid_] = filled_-1;
	vertex_to_idx[last_nd->vid_] = 0;

	filled_--;

	minHeapify(0);

	return root;

}

void MinHeap::decreaseKey(int vid, int dist){

	int i = vertex_to_idx[vid];
	nodes[i]->dist_ = dist;

	while(i && nodes[i]->dist_ < nodes[(i-1)/2]->dist_){
		vertex_to_idx[nodes[i]->vid_] = (i-1)/2;
		vertex_to_idx[nodes[(i-1)/2]->vid_] = i;

		swapNodes(i, (i-1)/2);

		i = (i-1)/2;
	}
}

bool MinHeap::isInMinHeap(int vid){

	if(vertex_to_idx[vid] < filled_) return true;

	return false;
}

void printDistances(int * dist, int V){

	cout << "vertex   distance from source \n";

	for(int i =0; i<V; i++){
		std::cout  << i << "      " << dist[i] << "\n";
	
	}



}

void runDijkstra(Graph* g, int src){
	int V = g->size_;
	int dist[V];

	MinHeap heap(V);

	for(int i=0 ; i<V; i++){
		dist[i] = INT_MAX;
		HeapNode* nd = new HeapNode(i, dist[i]);
		heap.nodes.push_back(nd);
		heap.vertex_to_idx[i] = i;
	}
	
	heap.nodes[src] = new HeapNode(src, dist[src]);
	heap.vertex_to_idx[src] = src;
	dist[src] = 0;
	heap.decreaseKey(src, dist[src]);

	heap.filled_ = V;

	while(!heap.isEmpty()){
		HeapNode* min_nd = heap.getMin();
		int u = min_nd->vid_;

		std::vector<ListNode*>::iterator it = g->vertices_[u]->begin();
		while(it!=g->vertices_[u]->end()){
			int v = (*it)->dest_;

			if(heap.isInMinHeap(v) && dist[u]!=INT_MAX && (*it)->weight_+dist[u] < dist[v]){
				dist[v] = dist[u] + (*it)->weight_;
				heap.decreaseKey(v, dist[v]);
			}

		}
		++it;
	
	}


	printDistances(dist, V);

}

