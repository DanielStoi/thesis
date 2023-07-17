#include <graph.h>


class BronKerboschPivot{
    Graph& g = *(new Graph);
    int count = 0;
    bool use_max_pivot;

    BronKerboschPivot(Graph& graph, bool use_max_pivot){
        if (g.edges_list.size() == 0){
            delete(&g);
        }

        g = graph;
        this->use_max_pivot = use_max_pivot;
    }

    void report(unordered_set<int>& included){
        count++;
        cout<<"found clique of size " << included.size() <<endl;
    }


    int find_max_pivot(unordered_set<int>& potential,  unordered_set<int>& excluded){
        int pivot;
        int new_potential_size = -1;
        for (int v: potential){
            int potential_size_cand = intersection_set_size(potential, g.edges_list[v]);
            if (potential_size_cand>0){
                pivot = v;
                new_potential_size = potential_size_cand;
            }
        }
        for (int v: excluded){
            int potential_size_cand = intersection_set_size(potential, g.edges_list[v]);
            if (potential_size_cand>0){
                pivot = v;
                new_potential_size = potential_size_cand;
            }
        }
        return pivot;
    }

    void solve(unordered_set<int> included, unordered_set<int>& potential,  unordered_set<int>& excluded){
        if (potential.size() == 0 && excluded.size() == 0){
            report(included);
            return;
        }

        if (potential.size()==0) return;


        //CHOOSING PIVOT
        int pivot;
        if (use_max_pivot){
            pivot = find_max_pivot(potential, excluded);
        }
        else{
            pivot = *potential.begin();
        }

        //ITERATING OVER EDGES
        unordered_set<int> pivot_potential;
        exclusion_set(potential, g.edges_list[pivot], pivot_potential);

        for (int v: pivot_potential){
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