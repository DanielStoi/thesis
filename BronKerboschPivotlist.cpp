#pragma once

#include "graph.h"
#include "els_set.h"
#include "should_print_clique.h"


#ifdef DEBUG_EDGESWAP_CORRECTNESS
#include "tests/correct_edgeswap_tester.cpp"
#endif


class BronKerboschPivot{
public:
    Graph& g;
    int count = 0;
    bool use_max_pivot;

    BronKerboschPivot(Graph& graph, bool use_max_pivot): g(graph){
        this->use_max_pivot = use_max_pivot;
    }

    void report(vector<int>& included){
        count++;

        if (should_print_clique){
            cout<<"found clique of size " << included.size() <<endl;
            print_vector(included);
        }
    }


    int find_max_pivot(X_P_Set& sets){
        #ifdef DONEPIVOTCALC
        if (sets.max_pivot != -1) return sets.max_pivot;
        #endif
        int pivot = -1;
        int pivot_size = -1;
        for (int i = 0; i<sets.P_size; i++){
            int v = sets.get_Pi(i);
            #ifndef INPUT_V_SET
            int potential_size_cand = sets.get_intersection_P_size(g.edges_list[v]);
            #else
            int potential_size_cand = sets.get_intersection_P_size(v);
            //int potential_size_cand = sets.adj_sizes[v].back();
            #endif
            if (potential_size_cand>pivot_size){
                pivot = v;
                pivot_size = potential_size_cand;
            }
        }
        #ifndef USE_PIVOT_P_ONLY
        for (int i = 0; i<sets.X_size; i++){
            int v = sets.get_Xi(i);
            #ifndef INPUT_V_SET
            int potential_size_cand = sets.get_intersection_P_size(g.edges_list[v]);
            #else
            int potential_size_cand = sets.get_intersection_P_size(v);
            //int potential_size_cand = sets.adj_sizes[v].back();
            #endif
            if (potential_size_cand>pivot_size){
                pivot = v;
                pivot_size = potential_size_cand;
            }
        }
        #endif
        //printf("found pivot as %d\n", pivot);
        return pivot;
    }

    int solve(){
        vector<int> included;

        //all elements will be in P by default
        X_P_Set XP(g.edges_list.size(), g);

        solve(included,XP);

        return count;
    }

    void get_set_exclusion(vector<int>& ans, int pivot, X_P_Set& XP){
        vector<int>& neigbours = g.edges_list[pivot];
        #ifndef INPUT_V_SET
        auto tempXP = XP.get_exclusion(neigbours);
        #else
        auto tempXP = XP.get_exclusion(pivot);
        #endif

        for (int i = 0; i < tempXP.P_size; i++){
            ans.push_back(tempXP.get_Pi(i));
        }
    }

    void solve(vector<int>& included, X_P_Set& XP){

        if (XP.P_size == 0 && XP.X_size == 0){
            #ifdef NO_X_MAINTAINANCE
            if (!XP.checker.check_maximal(included)) return;
            #endif
            report(included);
            return;
        }

        if (XP.P_size==0) return;


        //CHOOSING PIVOT
        int pivot;
        if (use_max_pivot){
            pivot = find_max_pivot(XP);
        }
        else{
            pivot = XP.get_Pi(0);
        }

        //iterating over XP_search set
        vector<int> search;
        get_set_exclusion(search, pivot, XP);
        
        // cout<<"--------------"<<pivot<<"\n";
        // cout<<"included:";
        // print_vector(included);
        // cout<<"--\nXP:\n";
        // XP.print_everything(included.size());
        // cout<<"XP_search:\n";
        // XP.get_exclusion(g.edges_list[pivot]).print_everything();
        // cout<<"the edgelist:\n";
        // print_vector(g.edges_list[pivot]);

        for (auto v: search){

            //calculating params for recursion
            included.push_back(v);

            {
                X_P_Set new_XP = XP.get_intersection(v);
                //solve recursively
                solve(included, new_XP);
            }

            included.pop_back(); 
            XP.add_exclusion(v);
        }


    }

};