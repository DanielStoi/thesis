#include "graph.h"
#include "els_set.h"
//#include "els_set_undo.cpp"
#include "should_print_clique.h"
#include "docs/equiv_classes.h"
#include "degeneracy_ordering.cpp"



//NOTE: STILL HAS SOME ERRORS

#define USE_MAX_PIVOT_EQUIVREDDEGEN true;

class BronKerboschEquivReductionRecursiveDegen{
public:
    Graph& g = *(new Graph);
    int count = 0;
    unsigned int multiplier = 1;
    vector<int> eq_size;
    bool use_max_pivot = USE_MAX_PIVOT_EQUIVREDDEGEN;

    BronKerboschEquivReductionRecursiveDegen(Graph& org): eq_size(vector<int>(org.size,0)){
        compute_equivalence_classes(org, g, eq_size);
    }


    

    void report(vector<int>& included){
        count += multiplier;
        if (should_print_clique){
            cout<<"found clique of size " << included.size() << ", mult:" << multiplier<< "cur count:"<< count <<endl;
            print_clique(included);
        }

        if (multiplier == 0){
            printf("UNEXPECTED INCORRECT BEHAVIOUR: MULTIPLER IS ZERO\n");
        }
    }
    

    void get_exclusion(vector<int>& ans, vector<int>& neigbours, X_P_Set& XP){
        auto tempXP = XP.get_exclusion(neigbours);
        for (int i = 0; i < tempXP.P_size; i++){
            ans.push_back(tempXP.get_Pi(i));
        }
    }

    int find_max_pivot(X_P_Set& sets){
        int pivot = -1;
        int pivot_size = -1;
        for (int i = 0; i<sets.P_size; i++){
            int v = sets.get_Pi(0);
            #ifndef CACHE_ADJ_SIZE_X_P_Set
            int potential_size_cand = sets.get_intersection_P_size(g.edges_list[v]);
            #else
            //int potential_size_cand = sets.adj_sizes[v].back();
            int potential_size_cand = sets.get_intersection_P_size(v);
            #endif
            if (potential_size_cand>pivot_size){
                pivot = v;
                pivot_size = potential_size_cand;
            }
        }
        for (int i = 0; i<sets.X_size; i++){
            int v = sets.get_Xi(i);
            #ifndef CACHE_ADJ_SIZE_X_P_Set
            int potential_size_cand = sets.get_intersection_P_size(g.edges_list[v]);
            #else
            //int potential_size_cand = sets.adj_sizes[v].back();
            int potential_size_cand = sets.get_intersection_P_size(v);
            #endif
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
        //printf("ordered %ld elements, degeneracy: %d\n", ordering.size(), degen_ordering);
        //all elements will be in P by default
        X_P_Set XP(g.edges_list.size(), g);
        
        for (auto v: ordering){
            
            //add v and solve using BronKerboschPivot
            included.push_back(v);
            multiplier *= eq_size[v];
            {
                X_P_Set new_XP = XP.get_intersection(v);
                solve(included, new_XP);
            }
            multiplier /= eq_size[v];

            //add each elm to exclusion
            included.pop_back(); 
            XP.add_exclusion(v);
        }
        return count;
    }

    
    
    void solve(vector<int>& included, X_P_Set& XP){

        if (XP.P_size == 0 && XP.X_size == 0){
            report(included);
            return;
        }

        if (XP.P_size==0) return;

        //applying equiv reduction to just edges in X,P
        vector<pair<int,int>> changes = compute_equivalence_classes_PX(g, XP, eq_size);


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
        // printf("search: ");
        // print_vector(search);
        
        // cout<<"--------------"<<pivot<<"\n";
        // cout<<"included:";
        // print_vector(included);
        // cout<<"--\nXP:\n";
        // XP.print_everything(included.size());
        // cout<<"XP_search:\n";
        // XP.get_exclusion(g.edges_list[pivot]).print_everything();

        for (auto v: search){

            //calculating params for recursion
            included.push_back(v);
            multiplier *= eq_size[v];

            {
                X_P_Set new_XP = XP.get_intersection(v);
                //solve recursively
                solve(included, new_XP);
            }
            multiplier /= eq_size[v];

            included.pop_back(); 
            XP.add_exclusion(v);
        }


        //reversing changes to equiv class
        for (pair<int,int> change: changes){
            
            eq_size[change.first] /= change.second;
            //printf("resetting from %d, mult:%d, new: %d\n", change.first, change.second, eq_size[change.first]);
        }

    }
};