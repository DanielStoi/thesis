#include "graph.h"
#include <vector>
#include <unordered_map>
#include <cassert>
#include <iostream>

#pragma once

using namespace std;


#ifdef LIST_ELS_SET_LOOKUP
struct edge_lookup{
//do unordered_map implementation here
}
#else

struct edge_lookup{
    vector<vector<int>> edges;
    vector<unordered_set<int>> edge_seen;
    Graph& g;
    edge_lookup(Graph& g): g(g){
        printf("created\n");
        int n = g.edges_list.size();
        edges = vector<vector<int>>(n, vector<int>(n,-1));
        edge_seen = vector<unordered_set<int>>(n);
        for (int v = 0; v<n; v++){
            int cur = 0;
            for (int e: g.edges_list[v]){
                edges[v][e] = cur++;
                edge_seen[v].insert(e);
            }
        }

        check_valid();
    }

    void check_valid(){
        //assert(false);
        for (int v = 0; v<g.edges_list.size(); v++){
            for (int e: g.edges_list[v]){
                assert(g.edges_list[v][edges[v][e]] == e);
            }
            for (int e = 0; e< g.edges_list.size(); e++){
                if (edge_seen[v].find(e) == edge_seen[v].end()){
                    assert(edges[v][e] == -1);
                    assert(edges[e][v] == -1);
                    assert(check_edge_exists(e,v) == false);
                }
                else{
                    assert(edges[v][e] != -1);
                    assert(edges[e][v] != -1);
                    assert(check_edge_exists(e,v) == true);
                }
            }
        }
    }

    bool check_edge_exists(int a1, int a2){
        if (g.edges_list.size() != edges.size() || edges.size() == 0 ){
            printf("BRUHWTF\n");
        }
        if (max(a1,a2)>=g.edges_list.size()) {
            printf("invaliiid - greater than allowed\n");
        }
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
        assert(g.edges_list[source][pos1] == e2);
        assert(g.edges_list[source][pos2] == e1);
        return true;
    }

    ~edge_lookup(){
        printf("deconstructed\n");
    }
};

#endif
