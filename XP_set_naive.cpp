
/*
naive, inefficient XP set implementation using:
-copying everything on every new set creation O(|X|+|P|) space per recursive call
-implementing everything using std::unordered_set (hashmaps)
-espected to be really slow
*/




#include <cstdlib>
#include <vector>
#include <iostream>
#include "graph.h"

#pragma once

using namespace std;

class X_P_Set{
public:
    //start with X->P
    //contains lookup table for all elements
    //vals contains all edges bounded by a range
    int n;
    unordered_set<int> P; //table index->loc of element (-1 if doesn't exist within table)
    int P_size;
    int X_size;
    unordered_set<int> X;  // raw combined array of X,P sets
    Graph& g;

    unordered_set<int>::iterator P_iter;
    int P_pos=-1;
    unordered_set<int>::iterator X_iter;
    int X_pos=-1;


    //adds all elements to P by default
    X_P_Set(int n, Graph& g):
    n(n), g(g){
        for (int i = 0; i<n; i++){
            P.insert(i);
        }
        P_size = n;
        X_size = 0;
    };

    void update_sizes(){
        P_size = P.size();
        X_size = X.size();
    }


    //constructor will just use same lookup and val table but change refs to pointers
    X_P_Set(X_P_Set& s, Graph& g) : 
        n(s.n), 
        g(g)
    {};

    X_P_Set( Graph& g) : 
        n(g.edges_list.size()), 
        g(g)
    {};


    void operator=(X_P_Set sets){
        n = sets.n;
        P = sets.P;
        X = sets.X;
    }

    //iteratate over neigbours and count intersection size and swap 
    X_P_Set get_intersection(int v){
        //printf("---inter---\n");
        //print_vector(g.edges_list[v]);
        X_P_Set new_XP(g);
        for (auto elm: g.edges_list[v]){
            
            if (in_P(elm)){
                new_XP.P.insert(elm);
            }
            else if (in_X(elm)){
                new_XP.X.insert(elm);
            }
        }
        new_XP.update_sizes();
        //print_everything();
        //new_XP.print_everything();
        return new_XP;
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



    X_P_Set get_exclusion(vector<int>& neighbours){
        X_P_Set new_XP =*this;
        for (auto elm: neighbours){

            if (in_P(elm)){
                new_XP.P.erase(elm);
            }
            else if (in_X(elm)){
                new_XP.X.erase(elm);
            }
        }
        new_XP.update_sizes();
        return new_XP;
    };

    //precondition is that element is in P
    bool add_exclusion(int elm){
        //printf("---add_excl for %d ---",elm);
        //print_everything();
        if (!in_P(elm)){
            return false;
        }
        P.erase(elm);
        X.insert(elm);
        P_pos = -1;
        X_pos = -1;
        update_sizes();
        //print_everything();
        return true;
    };





    //get i'th element in P
    int get_Pi(int i){
        update_sizes();
        if (i>=P_size) return -1;
        if (P_pos == -1 || P_pos>i){
            P_iter = P.begin();
            P_pos = 0;
        }
        while (P_pos<i){
            P_iter++;
            P_pos++;
        }
        return *P_iter;
    }

    //get i'th element in P
    int get_Xi(int i){
        update_sizes();
        if (i>=X_size) return -1;
        if (X_pos == -1 || X_pos>i){
            X_iter = X.begin();
            X_pos = 0;
        }
        while (X_pos<i){
            X_iter++;
            X_pos++;
        }
        return *X_iter;
    }


    bool in_P(int elm){
        return P.count(elm)>0;
    };

    bool in_X(int elm){
        return X.count(elm)>0;
    };


    void print_everything(int depth = 0){
        printf("P_size: %d, X_size: %d\nP:", P_size, X_size);
        for (auto i :P){
            printf(" %d",i);
        }
        printf("\nX:");
        for (auto i :X){
            printf(" %d",i);
        }
        printf("\n");
    }




    ~X_P_Set(){
        //printf("undoing %ld to %d\n", undo_queue.size()/2, queue_size/2);
        
    }





};





