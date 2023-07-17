/*
Working version
*/


#include "graph.h"
#include "els_set_undo.cpp"
#include "should_print_clique.h"

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
        }
    }


    int find_max_pivot(X_P_Set& sets){
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
        for (int i = 0; i<sets.X_size; i++){
            int v = sets.get_Xi(i);
            int potential_size_cand = sets.get_intersection_P_size(g.edges_list[v]);
            if (potential_size_cand>pivot_size){
                pivot = v;
                pivot_size = potential_size_cand;
            }
        }
        return pivot;
    }

    int solve(){
        vector<int> included;

        //all elements will be in P by default
        X_P_Set XP(g.edges_list.size());


        solve(included,XP);
        return count;
    }

    void solve(vector<int>& included, X_P_Set& XP){
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
        auto XP_search = XP.get_exclusion(g.edges_list[pivot]);

        //TODO: verify heap struct is correct for this pivot step
        while (XP_search.P_size>0){
            int v = XP_search.get_Pi(0);


            XP_search.add_exclusion(v);
            XP.add_exclusion(v);
            


            //calculating params for recursion
            {
            X_P_Set new_XP = XP.get_intersection(g.edges_list[v]);
            included.push_back(v);

            //solve recursively
            solve(included, new_XP);
            }
            
            included.pop_back(); 
            
        }

    }
};