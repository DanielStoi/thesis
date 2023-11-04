#include <cstdlib>
#include <vector>
#include <iostream>
#include "graph.h"
#include <cassert>

using namespace std;


#define INPUT_V_SET

#define SMART_INTERSECTION_CHOICE
#define EDGESWAP


#define ADJ_LOOKUP_MAP
/*
basic els_set except maintains a lookuptable so chooses to iterate over adjacency list or existing elements in P and X depending on size.
relevent for:
-pivot calculation
-intersection
-get exclusion
*/


#include <cstdlib>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "graph.h"
#include "edge_lookup.h"

#pragma once

using namespace std;


const string impl = "els_set_lookup_map.cpp";



class X_P_Set{
public:
    //start with X->P
    //contains lookup table for all elements
    //vals contains all edges bounded by a range
    int n;
    vector<int>& lookup; //table index->loc of element (-1 if doesn't exist within table)
    vector<int>& vals;  // raw combined array of X,P sets
    vector<int>& undo_queue; //queue for undoing moves
    edge_lookup& elook;
    Graph& g;
    int X_size;
    int P_start;
    int P_size;
    int queue_size = 0;


    //adds all elements to P by default
    X_P_Set(int n, Graph& g):
    n(n), X_size(0), P_start(0), P_size(n), lookup(*new vector<int>(n, -1)), vals(*new vector<int>(n, -1)), undo_queue(*new vector<int>()), g(g), elook(*new edge_lookup(g)){
        for (int i = 0; i<n; i++){
            lookup[i]=i;
            vals[i]=i;
        }
    };


    //constructor will just use same lookup and val table but change refs to pointers
    X_P_Set(X_P_Set& s, int X_size, int P_size, int org_queue_size) : 
        n(s.n), 
        lookup(s.lookup), 
        vals(s.vals), 
        undo_queue(s.undo_queue),
        X_size(X_size), 
        P_start(s.P_start), 
        P_size(P_size),
        queue_size(org_queue_size),
        g(s.g),
        elook(s.elook)
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
        if (ref.size()< X_size + P_size){
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
        int new_x_size = 0;
        int org_undo_size = undo_queue.size();

        int elm;


        //constructing intersection
        for (int i = 0; i < size; i++){
            
            
            elm = neighbours[i];

            if (in_P(elm)){
                do_swap(get_Pi(new_p_size), elm);
                new_p_size++;
            }
            else if (in_X(elm)){
                do_swap(get_Xi(new_x_size), elm);
                new_x_size++;
            }
        }



        return X_P_Set(*this,new_x_size, new_p_size, org_undo_size);
    };

    X_P_Set get_intersection_iter_PX(int v){
        //TODO: optimisation - pass in edge name use lookup table for optimisations
        //change from default implementation: need to reorder adjacency list for each n
        int new_p_size = 0;
        int new_x_size = 0;
        int org_undo_size = undo_queue.size();

        int elm;

        //constructing intersection
        for (int i = 0; i< P_size; i++){
            elm = get_Pi(i);
            if (elook.check_edge_exists(v,elm)){
                do_swap(get_Pi(new_p_size++), elm);
            }
        }
        for (int i = 0; i< X_size; i++){
            elm = get_Xi(i);
            if (elook.check_edge_exists(v,elm)){
                do_swap(get_Xi(new_x_size++), elm);
            }
        }


        return X_P_Set(*this,new_x_size, new_p_size, org_undo_size);
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
        int new_x_size = X_size;
        vector<int>& neighbours = g.edges_list[v];
        int org_undo_size = undo_queue.size();

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
        return X_P_Set(*this, new_x_size, new_p_size, org_undo_size);
    };

    //precondition is that element is in P
    bool add_exclusion(int elm){
        //printf("ADD excl: %d\n", elm);
        if (!in_P(elm)){
            return false;
        }
        undo_queue.push_back(elm);
        do_swap(elm, get_Pi(0)); //only here should elements be added to the queue
        

        P_start++;
        X_size++;
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
        cout<<"  undo_size:"<<undo_queue.size()/2;
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
        undo_changes();
        
    }


    void undo_changes(){
        while (undo_queue.size() > queue_size){
            //every elemen in undo queue was element added to X from P
            //can add to the front of X iteratively
            int elm = undo_queue[undo_queue.size()-1];
            undo_queue.pop_back();
            do_swap(elm, get_Xi(0));
            P_start-=1;
        }
    }



};





