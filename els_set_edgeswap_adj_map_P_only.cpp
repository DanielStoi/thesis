#include <cstdlib>
#include <vector>
#include <iostream>
#include "graph.h"
#include <cassert>

using namespace std;

/*
THIS IS A VERSION OF X_P_Set_lookup_map which:
- reorderes ONLY vertices in P (instead of P+X) so edges lists store P at front
- has edge lookup table O(n^2) space

optimisations can be made if only elements of P are reordered
*/


#include <cstdlib>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "graph.h"
#include "edge_lookup.h"


#pragma once

using namespace std;








class X_P_Set_Custom{
public:
    //start with X->P
    //contains lookup table for all elements
    //vals contains all edges bounded by a range
    int n;
    vector<int>& lookup; //table index->loc of element (-1 if doesn't exist within table)
    vector<int>& vals;  // raw combined array of X,P sets
    vector<int>& undo_queue; //queue for undoing moves
    edge_lookup& elook;
    Graph& g;
    int X_size;
    int P_start;
    int P_size;
    int queue_size = 0;


    //adds all elements to P by default
    X_P_Set_Custom(int n, Graph& g):
    n(n), X_size(0), P_start(0), P_size(n), lookup(*new vector<int>(n, -1)), vals(*new vector<int>(n, -1)), undo_queue(*new vector<int>()), g(g), elook(*new edge_lookup(g)){
        for (int i = 0; i<n; i++){
            lookup[i]=i;
            vals[i]=i;
        }
    };


    //constructor will just use same lookup and val table but change refs to pointers
    X_P_Set_Custom(X_P_Set_Custom& s, int X_size, int P_size, int org_queue_size) : 
        n(s.n), 
        lookup(s.lookup), 
        vals(s.vals), 
        undo_queue(s.undo_queue),
        X_size(X_size), 
        P_start(s.P_start), 
        P_size(P_size),
        queue_size(org_queue_size),
        g(s.g),
        elook(s.elook)
    {};

    void operator=(X_P_Set_Custom sets){
        n = sets.n;
        X_size = sets.X_size;
        P_start = sets.P_start;
        P_size = sets.P_size;
    }

    //iteratate over neigbours and count intersection size and swap 
    X_P_Set_Custom get_intersection(vector<int>& neighbours){ return get_intersection(neighbours.data(), neighbours.size()); }

    X_P_Set_Custom get_intersection(int* neighbours, int size){
        //change from default implementation: need to reorder adjacency list for each n
        int new_p_size = 0;
        int new_x_size = 0;
        int org_undo_size = undo_queue.size();

        int elm;


        //constructing intersection
        for (int i = 0; i < size; i++){
            
            
            elm = neighbours[i];

            if (in_P(elm)){
                do_swap(get_Pi(new_p_size), elm);
                new_p_size++;
            }
            else if (in_X(elm)){
                do_swap(get_Xi(new_x_size), elm);
                new_x_size++;
            }
        }

        //CIR_OPT

        //reordering adjacency list
        for(int i = 0; i < new_p_size; i++){
            elm = get_Pi(i);
            update_adj_list(elm, new_p_size);

        }
        for(int i = 0; i< new_x_size; i++){
            elm = get_Xi(i);
            update_adj_list(elm, new_p_size);
        }

        return X_P_Set_Custom(*this,new_x_size, new_p_size, org_undo_size);
    };
    
    void update_adj_list(int elm, int new_p_size){
        //printf("      update adj list for %d\n", elm);
        //printf("      before:"); print_vector(g.edges_list[elm]);
        int till = 0;
        auto& edge_list = g.edges_list[elm];
        for (int j = 0; j < edge_list.size(); j++){
            int e = edge_list[j];
            int loc = lookup[e];
            if (loc >= P_start && loc < P_start+new_p_size){//apply swap for smaller val
                elook.do_edge_swap(elm, edge_list[till], e);
                till++;
            }
            else if (!in_P(e)){
                break;
            }
        }
        //printf("      after:"); print_vector(g.edges_list[elm]);
    }

    void update_adj_list_single_elm(int source, int elm, int side = 0){ //want to move elm to outer edges of source edgelist
        vector<int>& edge_list = g.edges_list[source];
        int p_size = get_intersection_P_size(edge_list);
        if (p_size> edge_list.size()|| p_size> P_size || p_size == 0){
            printf("ERRORRRR relating to size %d %d %d\n", p_size, elook.check_edge_exists(source, elm), in_P(elm));
            print_vector(edge_list);
            for (int i = 0; i<P_size; i++){
                printf("%d ",get_Pi(i));
            } printf("\n");
            print_everything();
            assert(false);
        }
        bool res = elook.do_edge_swap(source, elm, edge_list[p_size-1]);
        if (!res){
            printf("side: %d - elms: %d, %d- source of failure inside update adj list, res is: %d %d\n",side, source, elm, elook.check_edge_exists(source, elm),elook.check_edge_exists(elm,source));
        }
    }

    int get_intersection_P_size(vector<int>& neighbours){return get_intersection_P_size(neighbours.data(), neighbours.size());}

    int get_intersection_P_size(int* neighbours, int size){
        int lower_bound = 0;
        int upper_bound = min(P_size, size)-1;
        int ans = 0;
        int mid;
        int elm;
        
        if (size < 10 ){
            for (int i = 0; i < size; i++){
                elm = neighbours[i];
                if (in_P(elm)){
                    ans++;
                }
                else break;
            }
            return ans;
        }

        while (upper_bound>=lower_bound){
            mid = (lower_bound+upper_bound)>>1;
            elm = neighbours[mid];
            if (in_P(elm)){
                lower_bound = mid+1;
                ans = lower_bound;
            }
            else{
                upper_bound = mid-1;
            }
        }

        return ans;
    };


    //will add all intersecting neigbours as exclusions
    X_P_Set_Custom get_exclusion(vector<int>& neighbours){
        return get_exclusion(neighbours.data(), neighbours.size());
    }

    X_P_Set_Custom get_exclusion(int* neighbours, int size){
        int new_p_size = P_size;
        int new_x_size = X_size;
        int org_undo_size = undo_queue.size();
        for (int i = 0; i < size; i++){
            int elm = neighbours[i];

            if (in_P(elm)){
                do_swap(get_Pi(new_p_size-1), elm);
                new_p_size--;
            }
            else break;
        }
        return X_P_Set_Custom(*this, 0, new_p_size, org_undo_size);
    };

    //precondition is that element is in P
    bool add_exclusion(int elm){
        //printf("ADD excl: %d\n", elm);
        if (!in_P(elm)){
            return false;
        }
        undo_queue.push_back(elm);
        do_swap(elm, get_Pi(0)); //only here should elements be added to the queue
        

        int neig;
        //idea: could optimise this process by also maintain a set of adj edges
        //for (int v: g.edges_list[elm]){
        //    if (!in_P(v)) break;
        //    update_adj_list_single_elm(v, elm);
        //}

        for(int i = 1; i < P_size; i++){
            neig = get_Pi(i);
            if (!elook.check_edge_exists(neig, elm)) continue;
            update_adj_list_single_elm(neig, elm);
            //}
        }


        P_start++;
        X_size++;
        P_size--;
        
        return true;
    };



    void do_swap(int elm_a, int elm_b){
        int loc_a = lookup[elm_a];
        int loc_b = lookup[elm_b];
        //swapping val entries
        vals[loc_a] = elm_b;
        vals[loc_b] = elm_a;
        //swapping lookup table entries
        int temp = loc_a;
        lookup[elm_a] = loc_b;
        lookup[elm_b] = temp;

    }

    //get i'th element in P
    int get_Pi(int i){
        if (i>=P_size) return -1;

        return vals[P_start+i];
    }

    //get i'th element in P
    int get_Xi(int i){
        if (i>=X_size) return -1;
        return vals[P_start-1-i];
    }


    bool in_P(int elm){
        int loc = lookup[elm];
        if (loc == -1)
            return false;
        return loc>=P_start && loc< P_size+P_start;
    };

    bool in_X(int elm){
        int loc = lookup[elm];
        if (loc == -1) 
            return false;
        return loc>=P_start-X_size && loc< P_start;
    };


    void print_everything(int depth = 0){
        for (int d = 0; d<depth; d++) printf("  ");
        cout<<"P_start:"<<P_start;
        cout<<"  P_size:"<<P_size;
        cout<<"  X_size:"<<X_size;
        cout<<"  undo_size:"<<undo_queue.size()/2;
        cout<<endl;
        for (int d = 0; d<depth; d++) printf("  ");
        cout<<"vals: ";
        for (int i=0; i<n; i++){
            if (i == P_start) cout<<"|";
            if (i==P_start-X_size) cout<<"|";
            if (i==P_start+P_size) cout<<"|";
            cout<<vals[i]<<" ";
        }
        cout<<endl;
    }




    ~X_P_Set_Custom(){
        //printf("undoing %ld to %d\n", undo_queue.size()/2, queue_size/2);
        undo_changes();
        
    }


    void undo_changes(){
        while (undo_queue.size() > queue_size){
            //every elemen in undo queue was element added to X from P
            //can add to the front of X iteratively
            int elm = undo_queue[undo_queue.size()-1];
            undo_queue.pop_back();
            do_swap(elm, get_Xi(0));
            P_start-=1;
        }
    }



};





