#pragma once

#include "graph.h"
#include "els_set_edgeswap_adj_map_P_only.cpp"
#include "should_print_clique.h"


class BronKerboschPivot_P_only{
public:
    Graph& g;
    int count = 0;
    bool use_max_pivot;

    BronKerboschPivot_P_only(Graph& graph, bool use_max_pivot): g(graph){
        this->use_max_pivot = use_max_pivot;
    }

    void report(vector<int>& included){
        count++;
        if (should_print_clique){
            cout<<"found clique of size " << included.size() <<endl;
            print_vector(included);
        }
    }


    int find_max_pivot(X_P_Set_Custom& sets){
        int pivot = -1;
        int pivot_size = -1;
        for (int i = 0; i<sets.P_size; i++){
            int v = sets.get_Pi(0);
            int potential_size_cand = sets.get_intersection_P_size(g.edges_list[v]);
            if (potential_size_cand>pivot_size){
                pivot = v;
                pivot_size = potential_size_cand;
            }
        }
        //printf("found pivot as %d\n", pivot);
        return pivot;
    }

    int solve(){
        vector<int> included;

        //all elements will be in P by default
        X_P_Set_Custom XP(g.edges_list.size(), g);


        solve(included,XP);
        return count;
    }

    void get_exclusion(vector<int>& ans, vector<int>& neigbours, X_P_Set_Custom& XP){
        auto tempXP = XP.get_exclusion(neigbours);
        for (int i = 0; i < tempXP.P_size; i++){
            ans.push_back(tempXP.get_Pi(i));
        }
    }

    void solve(vector<int>& included, X_P_Set_Custom& XP){

        if (XP.P_size == 0 && XP.X_size == 0){
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
        get_exclusion(search, g.edges_list[pivot], XP);
        
        //cout<<"--------------"<<pivot<<"\n";
        //cout<<"included:";
        //print_vector(included);
        //cout<<"--\nXP:\n";
        //XP.print_everything(included.size());
        //cout<<"XP_search:\n";
        //XP.get_exclusion(g.edges_list[pivot]).print_everything();

        for (auto v: search){

            //calculating params for recursion
            included.push_back(v);

            {
                X_P_Set_Custom new_XP = XP.get_intersection(g.edges_list[v]);
                //solve recursively
                solve(included, new_XP);
            }

            included.pop_back(); 
            XP.add_exclusion(v);
        }


    }
};