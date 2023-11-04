#include "maximal_clique_checker.cpp"

/*
Implementation that treats X as if it was empty all the time then checks if a given clique is maximal



*/




#include <cstdlib>
#include <vector>
#include <iostream>
#include "graph.h"

#pragma once

using namespace std;


#define NO_X_MAINTAINANCE
#define USE_PIVOT_P_ONLY


class X_P_Set{
public:
    //start with X->P
    //contains lookup table for all elements
    //vals contains all edges bounded by a range
    int n;
    vector<int>& lookup; //table index->loc of element (-1 if doesn't exist within table)
    vector<int>& vals;  // raw combined array of X,P sets
    Graph& g;
    int X_size = 0;
    int P_start;
    int P_size = 0;
    maximal_clique_checker& checker;


    //adds all elements to P by default
    X_P_Set(int n, Graph& g):
    n(n), P_start(0), P_size(n), lookup(*new vector<int>(n, -1)), vals(*new vector<int>(n, -1)), g(g), checker(*new maximal_clique_checker(g)){
        for (int i = 0; i<n; i++){
            lookup[i]=i;
            vals[i]=i;
        }
    };


    //constructor will just use same lookup and val table but change refs to pointers
    X_P_Set(X_P_Set& s,int P_size) : 
        n(s.n), 
        lookup(s.lookup), 
        vals(s.vals), 
        P_start(s.P_start), 
        P_size(P_size),
        g(s.g),
        checker(s.checker)
    { 
    };



    void operator=(X_P_Set sets){
        n = sets.n;
        P_start = sets.P_start;
        P_size = sets.P_size;
    }




    //iteratate over neigbours and count intersection size and swap 
    X_P_Set get_intersection(int v){ 
        vector<int>& ref = g.edges_list[v];
        return get_intersection(ref.data(), ref.size()); 
    }

    X_P_Set get_intersection(int* neighbours, int size){
        int new_p_size = 0;

        int elm;
        for (int i = 0; i < size; i++){
            
            
            elm = neighbours[i];

            if (in_P(elm)){
                do_swap(get_Pi(new_p_size), elm);
                new_p_size++;
            }
        }
        return X_P_Set(*this, new_p_size);
    };
    

    int get_intersection_P_size(vector<int>& neighbours){return get_intersection_P_size(neighbours.data(), neighbours.size());}

    int get_intersection_P_size(int* neighbours, int size){
        int new_p_size = 0;
        int elm;
        for (int i = 0; i < size; i++){
            elm = neighbours[i];
            if (in_P(elm)){
                new_p_size++;
            }
        }
        return new_p_size;
    };


    //will add all intersecting neigbours as exclusions
    X_P_Set get_exclusion(vector<int>& neighbours){
        return get_exclusion(neighbours.data(), neighbours.size());
    }

    X_P_Set get_exclusion(int* neighbours, int size){
        int new_p_size = P_size;
        for (int i = 0; i < size; i++){
            int elm = neighbours[i];

            if (in_P(elm)){
                do_swap(get_Pi(new_p_size-1), elm);
                new_p_size--;
            }
        }
        return X_P_Set(*this, new_p_size);
    };

    //precondition is that element is in P
    bool add_exclusion(int elm){
        if (!in_P(elm)) return false;
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
        
    }






};





