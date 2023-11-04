
#include <cstdlib>
#include <vector>
#include <iostream>
#include "graph.h"
#include <cassert>



struct maximal_clique_checker{
public:
    vector<int> lookup;
    vector<int> vals;
    Graph& g;
    int size = 0;
    int n;
    maximal_clique_checker(Graph& g): g(g){
        n = g.edges_list.size();
        lookup = vector<int>(n,0);
        vals = vector<int>(n,0);
        for (int i = 0; i<n; i++){
            vals[i]=i;
            lookup[i]=i;
        }
    }

    bool exists(int elm){
        return lookup[elm]<size;
    }
    void do_swap(int elm_a, int elm_b){
        int loc_a = lookup[elm_a];
        int loc_b = lookup[elm_b];
        //swapping val entries
        vals[loc_a] = elm_b;
        vals[loc_b] = elm_a;
        //swapping lookup table entries
        lookup[elm_a] = loc_b;
        lookup[elm_b] = loc_a;
    }

    bool check_maximal(vector<int>& clique){
        size = g.edges_list.size();

        //removing clique elements for candidate extensions
        for (int v: clique){
            do_swap(v, vals[size-1]);
            size--;
        }
        for (int v: clique){
            int new_size = 0;
            for (int elm: g.edges_list[v]){
                if (lookup[elm] < size){
                    do_swap(elm, vals[new_size]);
                    new_size++;
                }
            }
            size = new_size;

            if (size==0) return true;
        }
        return false;
    }

    bool check_correctness(){
        unordered_set<int> seen;
        bool is_valid = true;
        for (int i = 0; i<n; i++){
            if (seen.count(vals[i])){
                printf("ERROR IN CLIQUE CHECKER: DOUBLE COUNTED ELEMENT %d\n", vals[i]);
                is_valid = false;
            }

            if (lookup[vals[i]] != i){
                printf("ERROR IN CLIQUE CHECKER: WRONG LOOKUP FOR ELM %d\n", vals[i]);
                is_valid = false;
            }
            seen.insert(vals[i]);

        }

        if(!is_valid){
            printf("vals: ");
            print_vector(vals);
            printf("lookups: ");
            print_vector(lookup);
        }
        assert(is_valid);
        return is_valid;
    }
};

