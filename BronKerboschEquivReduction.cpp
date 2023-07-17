#include "graph.h"
#include "els_set.h"
//#include "els_set_undo.cpp"
#include "should_print_clique.h"
#include "docs/equiv_classes.h"

class BronKerboschEquivReduction{
public:
    Graph& g = *(new Graph);
    int count = 0;
    unsigned int multiplier = 1;
    unordered_map<unsigned int, unsigned int> eq_size;

    BronKerboschEquivReduction(Graph& org){
        compute_equivalence_classes(org, g, eq_size);
    }

    void report(vector<int>& included){
        count += multiplier;
        if (should_print_clique){
            cout<<"found clique of size " << included.size() <<endl;
            print_clique(included);
        }
    }

    int solve(){
        vector<int> included;

        //all elements will be in P by default
        X_P_Set XP(g.edges_list.size(), g);
        //X_P_Set XP(g.edges_list.size());


        solve(included,XP);
        return count;
    }
    
    void solve(vector<int>& included, X_P_Set& XP){
        //XP.print_everything(included.size());
        if (XP.P_size == 0 && XP.X_size == 0){
            report(included);
            return;
        }

        if (XP.P_size==0) return;

        //iterating through all the elements and updating
        while (XP.P_size>0){
            int v = XP.get_Pi(0);
            //calculating params for recursion
            
            {
                X_P_Set new_XP = XP.get_intersection(g.edges_list[v]);

                included.push_back(v);
                multiplier *= eq_size[v];


                //solve recursively
                solve(included, new_XP);

                included.pop_back(); 
                multiplier /= eq_size[v];
            }


            XP.add_exclusion(v);
        }

    }
};