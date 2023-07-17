#include "graph.h"
#include "els_set.h"
#include "degeneracy_ordering.cpp"
#include "should_print_clique.h"
#include "BronKerboschPivotList.cpp"

class BronKerboschDegen: public BronKerboschPivot{
    using BronKerboschPivot::BronKerboschPivot;
public:


    int solve(){
        vector<int> included;
        vector<int> ordering;


        //preprocessing: getting degeneracy ordering list
        int degen_ordering = Lin_Heap(&g).get_ordering(ordering);
        printf("ordered %ld elements, degeneracy: %d\n", ordering.size(), degen_ordering);
        //all elements will be in P by default
        X_P_Set XP(g.edges_list.size(), g);
        
        for (auto v: ordering){
            
            //add v and solve using BronKerboschPivot
            included.push_back(v);
            
            {
                X_P_Set new_XP = XP.get_intersection(g.edges_list[v]);
                BronKerboschPivot::solve(included, new_XP);
            }

            //add each elm to exclusion
            included.pop_back(); 
            XP.add_exclusion(v);
        }
        return count;
    }

};