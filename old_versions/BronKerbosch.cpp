#include <graph.h>


class BronKerbosch{
    Graph& g = *(new Graph);
    int count = 0;

    BronKerbosch(Graph& graph){
        if (g.edges_list.size() == 0){
            delete(&g);
        }

        g = graph;
    }

    void report(unordered_set<int>& included){
        count++;
        cout<<"found clique of size " << included.size() <<endl;
    }

    void solve(unordered_set<int> included, unordered_set<int> potential,  unordered_set<int> excluded){
        if (potential.size() == 0 && excluded.size() == 0){
            report(included);
            return;
        }


        for (int v: potential){
            //calculating params for recursion
            unordered_set<int> new_potential;
            unordered_set<int> new_excluded;
            intersection_set(potential, g.edges_list[v], new_potential);
            intersection_set(excluded, g.edges_list[v], new_excluded);
            included.insert(v);

            //solve recursively
            solve(included, new_potential, new_excluded);

            included.erase(v); 
            excluded.insert(v); //consantly appending cur element so we don't double count
        }

    }
};