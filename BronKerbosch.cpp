#include "graph.h"
#include "els_set.h"
//#include "els_set_undo.cpp"
#include "should_print_clique.h"

class BronKerbosch{
public:
    Graph& g = *(new Graph);
    int count = 0;

    BronKerbosch(Graph& graph): g(graph), count(0){}

    void report(vector<int>& included){
        count++;
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
                X_P_Set new_XP = XP.get_intersection(v);
                included.push_back(v);

                //solve recursively
                solve(included, new_XP);

                included.pop_back(); 
            }


            XP.add_exclusion(v);
        }

    }
};