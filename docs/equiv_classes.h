/*
 * common_functions.h
 *
 *  Created on: 29May,2018
 *      Author: ljchang@outlook.com
 */

#ifndef COMMON_FUNCTIONS_H_
#define COMMON_FUNCTIONS_H_

#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unordered_set>
#include <unordered_map>
#include "../graph.h"

#ifndef NDEBUG
#define NDEBUG
#endif

#include <cassert>

// compute independent set-equivalence classes (self_loop = 0): u and v are in the same equivalence class iff (u,v) \notin E, and N^-(u) = N^-(v)
// compute clique-equivalence classes (self_loop = 1): u and v are in the same equivalence class iff (u,v) \in E, and N^-(u) = N^-(v)
// the resulting equivalence classes are represented by eq_class_starts and eq_class_ids
void compute_equivalence_classes(const Graph& g, Graph& eq_graph, vector<int>& eq_sizes, char self_loop = 0) {
    unsigned int eq_class_n = 0;
    unsigned int n = g.edges_list.size();

    unsigned int *vertex2set = new unsigned int[n];
    for(unsigned int i = 0;i < n;i ++) vertex2set[i] = 0; //all vertices are assigned to set 0

    unsigned int *eq_class_starts = new unsigned int[n+1];
    unsigned int *eq_class_ids =  new unsigned int[n+1];
    
    unsigned int *queue = eq_class_ids;
    unsigned int *candidate_setIDs = eq_class_starts; //available setIDs
    unsigned int *new_setID = new unsigned int[n+1]; //mapping from old setID to new setID
    unsigned int *set_size = new unsigned int[n+1]; //number of vertices in each set


    for(unsigned int i = 0;i <= n;i ++) {
        new_setID[i] = n+1;
        set_size[i] = 0;
        candidate_setIDs[i] = i;
    }
    set_size[0] = n;

    unsigned int candidate_setIDs_head = 1;

    for(unsigned int i = 0;i < n;i ++) {
        unsigned int queue_n = 0;
        /*if(self_loop) {
            unsigned int v = i;
            unsigned int old_setID = vertex2set[v];
            if(new_setID[old_setID] > n) {
                new_setID[old_setID] = candidate_setIDs[candidate_setIDs_head ++];
                queue[queue_n ++] = old_setID;
            }
            vertex2set[v] = new_setID[old_setID];
            ++ set_size[vertex2set[v]];
            if(-- set_size[old_setID] == 0) candidate_setIDs[-- candidate_setIDs_head] = old_setID;
        }*/
        for(int v: g.edges_list[i]) {
            unsigned int old_setID = vertex2set[v];

            if(new_setID[old_setID] > n) {    //assigning a new_setID for edge
                if(set_size[old_setID] == 1) continue;

                new_setID[old_setID] = candidate_setIDs[candidate_setIDs_head ++]; 
                queue[queue_n ++] = old_setID;
            }
            vertex2set[v] = new_setID[old_setID];
            ++ set_size[vertex2set[v]];
            if(-- set_size[old_setID] == 0) candidate_setIDs[-- candidate_setIDs_head] = old_setID;
        }
        while(queue_n > 0) new_setID[queue[-- queue_n]] = n+1;   //equivalent to removing elements from new_setId?
    }


    //setting up sizes and mapping for eventual writing	
    eq_class_n = 0;
    eq_class_starts[0] = 0;
    for(unsigned int i = 0;i < n;i ++) {
        unsigned int setID = vertex2set[i];
        if(new_setID[setID] > n) { //if set hasn't been added already: add it
            new_setID[setID] = eq_class_n ++;
            eq_class_starts[eq_class_n] = eq_class_starts[eq_class_n-1] + set_size[setID];
        }
    }



    //writing elements into output format
    for(unsigned int i = 0;i < n;i ++) {
        unsigned int setID = new_setID[vertex2set[i]];
        eq_class_ids[eq_class_starts[setID] ++] = i;
    }

    for(unsigned int i = eq_class_n;i > 0;i --) eq_class_starts[i] = eq_class_starts[i-1];
    eq_class_starts[0] = 0;



    //constructing a new graph:
    for (unsigned int i = 0; i < eq_class_n; i++){
        eq_graph.add_node();
        eq_sizes[i] = eq_class_starts[i+1]-eq_class_starts[i];
    }


    for(unsigned int i = 0; i < eq_class_n; i++){
        unordered_set<unsigned int> seen;
        int v = eq_class_ids[eq_class_starts[i]];
        for (int av: g.edges_list[v]){
            int setID = new_setID[vertex2set[av]];
            if (seen.find(setID) == seen.end()){
                eq_graph.edges_list[v].push_back(setID);
                seen.insert(setID);
            }
        }
    }



#ifndef NDEBUG
    char *dvis = new char[n];
    memset(dvis, 0, sizeof(char)*n);

    assert(eq_class_starts[eq_class_n] == n);

    for(unsigned int i = 0;i < eq_class_n;i ++) if(eq_class_starts[i+1] > eq_class_starts[i]+1) {
        unsigned int u = eq_class_ids[eq_class_starts[i]];
        for(unsigned int j = pstart[u];j < pend[u];j ++) dvis[edges[j]] = 1;
        if(self_loop) dvis[u] = 1;

        for(unsigned int j = eq_class_starts[i]+1;j < eq_class_starts[i+1];j ++) {
            unsigned int v = eq_class_ids[j];
            if(self_loop) assert(dvis[v]);
            else assert(!dvis[v]);
            assert(pend[u] - pstart[u] == pend[v] - pstart[v]);
            for(unsigned int k = pstart[v];k < pend[v];k ++) assert(dvis[edges[k]]);
        }

        for(unsigned int j = pstart[u];j < pstart[u+1];j ++) dvis[edges[j]] = 0;
        dvis[u] = 0;
    }

    delete[] dvis;
#endif

    delete[] set_size;
    delete[] new_setID;
    delete[] vertex2set;
    delete[] eq_class_starts;
    delete[] eq_class_ids;
}




vector<pair<int,int>> compute_equivalence_classes_PX(const Graph& g, X_P_Set& XP, vector<int>& eq_sizes, char self_loop = 0) {
    unsigned int eq_class_n = 0;
    unsigned int n = XP.P_size + XP.X_size;


    unsigned int *vertex2set = new unsigned int[n];
    for(unsigned int i = 0;i < n;i ++) vertex2set[i] = 0; //all vertices are assigned to set 0

    unsigned int *eq_class_starts = new unsigned int[n+1];
    unsigned int *eq_class_ids =  new unsigned int[n+1];
    
    unsigned int *queue = eq_class_ids;
    unsigned int *candidate_setIDs = eq_class_starts; //available setIDs
    unsigned int *new_setID = new unsigned int[n+1]; //mapping from old setID to new setID
    unsigned int *set_size = new unsigned int[n+1]; //number of vertices in each set


    for(unsigned int i = 0;i <= n;i ++) {
        new_setID[i] = n+1;
        set_size[i] = 0;
        candidate_setIDs[i] = i;
    }
    set_size[0] = n;

    unsigned int candidate_setIDs_head = 1;

    int XP_start = XP.P_start-XP.X_size;
    #define setelm_to_vertex(x) (XP.vals[x+XP_start]);
    #define vertex_to_setelm(x) (XP.lookup[x]-XP_start);

    for(unsigned int i = 0;i < n;i ++) {
        unsigned int queue_n = 0;
        int vi = setelm_to_vertex(i);


        //we need to iterate through entire of X,P 
        //an optimisation for edge lookup table is iterating through X,P instead of edges
        for(int v: g.edges_list[i]) {
            if (!XP.in_P(v) && !XP.in_X(v)) 
                continue;
            v = vertex_to_setelm(v);
            unsigned int old_setID = vertex2set[v];

            if(new_setID[old_setID] > n) {    //assigning a new_setID for edge
                if(set_size[old_setID] == 1) continue;

                new_setID[old_setID] = candidate_setIDs[candidate_setIDs_head ++]; 
                queue[queue_n ++] = old_setID;
            }
            vertex2set[v] = new_setID[old_setID];
            ++ set_size[vertex2set[v]];
            if(-- set_size[old_setID] == 0) candidate_setIDs[-- candidate_setIDs_head] = old_setID;
        }
        while(queue_n > 0) new_setID[queue[-- queue_n]] = n+1;   //equivalent to removing elements from new_setId?
    }


    //setting up sizes and mapping for eventual writing	
    eq_class_n = 0;
    eq_class_starts[0] = 0;
    for(unsigned int i = 0;i < n;i ++) {
        unsigned int setID = vertex2set[i];
        if(new_setID[setID] > n) { //if set hasn't been added already: add it
            new_setID[setID] = eq_class_n ++;
            eq_class_starts[eq_class_n] = eq_class_starts[eq_class_n-1] + set_size[setID];
        }
    }

    


    //writing elements into output format
    for(unsigned int i = 0;i < n;i ++) {
        unsigned int setID = new_setID[vertex2set[i]];
        eq_class_ids[eq_class_starts[setID] ++] = i;
    }

    for(unsigned int i = eq_class_n;i > 0;i --) eq_class_starts[i] = eq_class_starts[i-1];
    eq_class_starts[0] = 0;

    //storing equiv class changes in this for future reversability
    vector<pair<int,int>> changes;

    //updating XP to reflect changes
    //printf("---\n");
    int new_p_size = XP.P_size;
    int new_x_size = XP.X_size;
    for (unsigned int i = 0; i < eq_class_n; i++){
        int cstart = eq_class_starts[i];
        int equiv_size = eq_class_starts[i+1]-cstart;
        if (equiv_size <= 1) continue;
        int elm = setelm_to_vertex(eq_class_ids[cstart]);
        changes.push_back(make_pair(elm ,equiv_size));
        eq_sizes[elm] *= equiv_size;
        //printf("head_equiv: %d, size: %d\n", elm, equiv_size);

        
        for (int i = 1; i< equiv_size; i++){
            
            //remove other equiv edges from X,P
            int elm = setelm_to_vertex(eq_class_ids[cstart+i]);
            //printf("removing element: %d\n", elm);
            if (XP.in_P(elm)){
                XP.do_swap(XP.get_Pi(--new_p_size), elm);
            }
            else if (XP.in_X(elm)){
                XP.do_swap(XP.get_Xi(XP.X_size-1), elm);
                XP.X_size--;
            }

        }
    }

#ifdef EDGESWAP
    for(int i = 0; i < XP.P_size; i++){
        int elm = XP.get_Pi(i);
        XP.update_adj_list(elm, new_p_size);
    }
    for(int i= 0; i< XP.X_size; i++){
        int elm = XP.get_Xi(i);
        XP.update_adj_list(elm, new_p_size);
    }

#endif



    //deleting used data
    delete[] set_size;
    delete[] new_setID;
    delete[] vertex2set;
    delete[] eq_class_starts;
    delete[] eq_class_ids;

    return changes;
}



#endif /* COMMON_FUNCTIONS_H_ */
