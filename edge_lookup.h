#include "graph.h"
#include <vector>
#include <unordered_map>
#include <cassert>
#include <iostream>

#pragma once

using namespace std;


//remove or add define to switch between hashmap and list implementations
//#define USE_HASHMAP_ELS_SET_LOOKUP




#ifdef USE_HASHMAP_ELS_SET_LOOKUP
//HASHMAP IMPLEMENTATION O(n+m) space

struct edge_lookup{
    vector<unordered_map<int,int>> edges;
    Graph& g;
    edge_lookup(Graph& g): g(g){
        printf("created\n");
        int n = g.edges_list.size();
        edges = vector<unordered_map<int,int>>(n, unordered_map<int,int>());
        for (int v = 0; v<n; v++){
            int cur = 0;
            for (int e: g.edges_list[v]){
                edges[v][e] = cur++;
            }
        }

        check_valid();
    }

    void check_valid(){
        // TODO 
    }

    bool check_edge_exists(int a1, int a2){
        if (g.edges_list.size() != edges.size() || edges.size() == 0 ){
            printf("BRUHWTF\n");
        }
        if (max(a1,a2)>=g.edges_list.size()) {
            printf("invaliiid - greater than allowed\n");
        }
        return edges[a1].count(a2) > 0;
    }

    bool do_edge_swap(int source, int e1, int e2){

        if (edges[source].count(e1) == 0 || edges[source].count(e2) == 0){
            printf("invaliiid - one pos doesn't exist\n");
            return false;
        }

        int pos1 = edges[source][e1];
        int pos2 = edges[source][e2];
        
        
        swap(g.edges_list[source][pos1], g.edges_list[source][pos2]);
        swap(edges[source][e1], edges[source][e2]);
        //assert(g.edges_list[source][pos1] == e2);
        //assert(g.edges_list[source][pos2] == e1);
        return true;
    }

    ~edge_lookup(){
        printf("deconstructed\n");
    }
};




#else
//LIST IMPLEMENTATION O(n^2) space
struct edge_lookup{
    vector<vector<int>> edges;
    Graph& g;
    edge_lookup(Graph& g): g(g){
        printf("created\n");
        int n = g.edges_list.size();
        edges = vector<vector<int>>(n, vector<int>(n,-1));
        for (int v = 0; v<n; v++){
            int cur = 0;
            for (int e: g.edges_list[v]){
                edges[v][e] = cur++;
            }
        }

        check_valid();
    }

    void check_valid(){
        // TODO 
    }

    bool check_edge_exists(int a1, int a2){
        return edges[a1][a2] != -1;
    }

    bool do_edge_swap(int source, int e1, int e2){
        int pos1 = edges[source][e1];
        int pos2 = edges[source][e2];
        
        if (pos1 == -1 || pos2 == -1) {
            printf("invaliiid - one pos is negative %d %d %d \n",pos1,pos2, check_edge_exists(source, e1));
            check_valid();
            return false;
        }
        swap(g.edges_list[source][pos1], g.edges_list[source][pos2]);
        swap(edges[source][e1], edges[source][e2]);
        //assert(g.edges_list[source][pos1] == e2);
        //assert(g.edges_list[source][pos2] == e1);
        return true;
    }

    ~edge_lookup(){
        printf("deconstructed\n");
    }
};

#endif