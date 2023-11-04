#include <cstdlib>
#include <vector>
#include <iostream>
#include "graph.h"
#include "maximal_clique_checker.cpp"
#include <cassert>

using namespace std;


#define INPUT_V_SET
#define NO_X_MAINTAINANCE

#define SMART_INTERSECTION_CHOICE


#define ADJ_LOOKUP_MAP
/*
based off els_set_lookup_map except doesn't maintain X and instead does a check at the end

*/


#include <cstdlib>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "graph.h"
#include "edge_lookup.h"

#pragma once

using namespace std;


const string impl = "els_set_lookup_map_no_X_no_undo.cpp";



class X_P_Set{
public:
    //start with X->P
    //contains lookup table for all elements
    //vals contains all edges bounded by a range
    int n;
    vector<int>& lookup; //table index->loc of element (-1 if doesn't exist within table)
    vector<int>& vals;  // raw combined array of X,P sets
    maximal_clique_checker& checker;
    edge_lookup& elook;
    Graph& g;
    int X_size=0;
    int P_start;
    int P_size;


    //adds all elements to P by default
    X_P_Set(int n, Graph& g):
    n(n), P_start(0), P_size(n), lookup(*new vector<int>(n, -1)), vals(*new vector<int>(n, -1)), g(g), 
    elook(*new edge_lookup(g)), checker(*new maximal_clique_checker(g)){
        for (int i = 0; i<n; i++){
            lookup[i]=i;
            vals[i]=i;
        }
    };


    //constructor will just use same lookup and val table but change refs to pointers
    X_P_Set(X_P_Set& s, int P_size) : 
        n(s.n), 
        lookup(s.lookup), 
        vals(s.vals), 
        P_start(s.P_start), 
        P_size(P_size),
        g(s.g),
        elook(s.elook),
        checker(s.checker)
    {};

    void operator=(X_P_Set sets){
        n = sets.n;
        X_size = sets.X_size;
        P_start = sets.P_start;
        P_size = sets.P_size;
    }

    //iteratate over neigbours and count intersection size and swap 
    X_P_Set get_intersection(int v){ 
        vector<int>& ref = g.edges_list[v];

#ifndef SMART_INTERSECTION_CHOICE
        return get_intersection_iter_list(ref.data(), ref.size()); 
#else
        if (ref.size()< P_size){
            return get_intersection_iter_list(ref.data(), ref.size()); 
        }
        else{
            return get_intersection_iter_PX(v);
        }
#endif
    }

    X_P_Set get_intersection_iter_list(int* neighbours, int size){
        //TODO: optimisation - pass in edge name use lookup table for optimisations
        //change from default implementation: need to reorder adjacency list for each n
        int new_p_size = 0;

        int elm;


        //constructing intersection
        for (int i = 0; i < size; i++){
            
            
            elm = neighbours[i];

            if (in_P(elm)){
                do_swap(get_Pi(new_p_size), elm);
                new_p_size++;
            }
        }



        return X_P_Set(*this, new_p_size);
    };

    X_P_Set get_intersection_iter_PX(int v){
        //TODO: optimisation - pass in edge name use lookup table for optimisations
        //change from default implementation: need to reorder adjacency list for each n
        int new_p_size = 0;
        int new_x_size = 0;

        int elm;

        //constructing intersection
        for (int i = 0; i< P_size; i++){
            elm = get_Pi(i);
            if (elook.check_edge_exists(v,elm)){
                do_swap(get_Pi(new_p_size++), elm);
            }
        }


        return X_P_Set(*this,new_p_size);
    };
    



    int get_intersection_P_size(int v){
        vector<int>& neigbours = g.edges_list[v];
        int ans = 0;
        if (neigbours.size() < P_size){
            for (int elm: neigbours){
                if (in_P(elm)) ans++;
            }
            return ans;
        }
        
        for (int i = 0; i< P_size; i++){
            int elm = get_Pi(i);
            if (elook.check_edge_exists(elm,v)) ans++;
        }
        return ans;
    };


    //will add all intersecting neigbours as exclusions

    X_P_Set get_exclusion(int v){
        int new_p_size = P_size;
        vector<int>& neighbours = g.edges_list[v];

        if (P_size > neighbours.size()){
            for (int elm: neighbours){
                if (in_P(elm)){
                    do_swap(get_Pi(new_p_size-1), elm);
                    new_p_size--;
                }
            }
        }
        else{
            for (int i = P_size-1; i>=0; i--){
                int elm = get_Pi(i);
                if (elook.check_edge_exists(elm,v)){
                    do_swap(get_Pi(new_p_size-1), elm);
                    new_p_size--;
                }
            }

        }
        return X_P_Set(*this, new_p_size);
    };

    //precondition is that element is in P
    bool add_exclusion(int elm){
        //printf("ADD excl: %d\n", elm);
        if (!in_P(elm)){
            return false;
        }
        do_swap(elm, get_Pi(P_size-1)); //only here should elements be added to the queue
        P_size--;
        
        return true;
    };



    void do_swap(int elm_a, int elm_b){
        int loc_a = lookup[elm_a];
        int loc_b = lookup[elm_b];
        //swapping val entries
        vals[loc_a] = elm_b;
        vals[loc_b] = elm_a;
        //swapping lookup table entries
        int temp = loc_a;
        lookup[elm_a] = loc_b;
        lookup[elm_b] = temp;

    }

    //get i'th element in P
    int get_Pi(int i){
        if (i>=P_size) return -1;

        return vals[P_start+i];
    }

    //get i'th element in P
    int get_Xi(int i){
        if (i>=X_size) return -1;
        return vals[P_start-1-i];
    }


    bool in_P(int elm){
        int loc = lookup[elm];
        if (loc == -1)
            return false;
        return loc>=P_start && loc< P_size+P_start;
    };

    bool in_X(int elm){
        int loc = lookup[elm];
        if (loc == -1) 
            return false;
        return loc>=P_start-X_size && loc< P_start;
    };


    void print_everything(int depth = 0){
        for (int d = 0; d<depth; d++) printf("  ");
        cout<<"P_start:"<<P_start;
        cout<<"  P_size:"<<P_size;
        cout<<"  X_size:"<<X_size;
        cout<<endl;
        for (int d = 0; d<depth; d++) printf("  ");
        cout<<"vals: ";
        for (int i=0; i<n; i++){
            if (i == P_start) cout<<"|";
            if (i==P_start-X_size) cout<<"|";
            if (i==P_start+P_size) cout<<"|";
            cout<<vals[i]<<" ";
        }
        cout<<endl;
    }




    ~X_P_Set(){
        //printf("undoing %ld to %d\n", undo_queue.size()/2, queue_size/2);
        
    }






};





