#include "graph.h"
#include "els_set_undo.cpp"
#include "degeneracy_ordering.cpp"
#include "should_print_clique.h"

class BronKerboschDegen{
public:
    Graph& g;
    int count = 0;
    bool use_max_pivot;

    BronKerboschDegen(Graph& graph, bool use_max_pivot): g(graph){
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
            int potential_size_cand = sets.get_intersection(g.edges_list[v]).P_size;
            if (potential_size_cand>pivot_size){
                pivot = v;
                pivot_size = potential_size_cand;
            }
        }
        for (int i = 0; i<sets.X_size; i++){
            int v = sets.get_Xi(i);
            int potential_size_cand = sets.get_intersection(g.edges_list[v]).P_size;
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
        vector<int> ordering;


        //preprocessing: getting degeneracy ordering list
        int degen_ordering = Lin_Heap(&g).get_ordering(ordering);
        printf("ordered %ld elements, degeneracy: %d\n", ordering.size(), degen_ordering);
        //all elements will be in P by default
        X_P_Set XP(g.edges_list.size());
        
        for (auto v: ordering){
            
            //add v and solve using BronKerboschPivot
            included.push_back(v);
            
            {
                X_P_Set new_XP = XP.get_intersection(g.edges_list[v]);
                solve_pivot(included, new_XP);
            }

            //add each elm to exclusion
            included.pop_back(); 
            XP.add_exclusion(v);
        }
        return count;
    }


    //exactly
    void solve_pivot(vector<int>& included, X_P_Set XP){
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
            solve_pivot(included, new_XP);
            }
            
            included.pop_back(); 
            
        }

    }
};