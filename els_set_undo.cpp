
/*
A  Currently used XP set implementation that stores an undo queue 



*/




#include <cstdlib>
#include <vector>
#include <iostream>

#pragma once

using namespace std;

class X_P_Set{
public:
    //start with X->P
    //contains lookup table for all elements
    //vals contains all edges bounded by a range
    int n;
    vector<int>& lookup; //table index->loc of element (-1 if doesn't exist within table)
    vector<int>& vals;  // raw combined array of X,P sets
    vector<int>& undo_queue; //queue for undoing moves
    int X_size;
    int P_start;
    int P_size;
    int queue_size = 0;


    //adds all elements to P by default
    X_P_Set(int n):
    n(n), X_size(0), P_start(0), P_size(n), lookup(*new vector<int>(n, -1)), vals(*new vector<int>(n, -1)), undo_queue(*new vector<int>()){
        for (int i = 0; i<n; i++){
            lookup[i]=i;
            vals[i]=i;
        }
    };


    //constructor will just use same lookup and val table but change refs to pointers
    X_P_Set(X_P_Set& s, int X_size, int P_size, int org_queue_size = 0) : 
        n(s.n), 
        lookup(s.lookup), 
        vals(s.vals), 
        undo_queue(s.undo_queue),
        X_size(X_size), 
        P_start(s.P_start), 
        P_size(P_size),
        queue_size(org_queue_size)
    { 
        if (org_queue_size == 0) queue_size = undo_queue.size();
    };

    //copy constructor
    // X_P_Set(int n, int* P_table, int P_size, int* X_table, int X_size):
    // n(n), X_size(0), P_start(X_size), P_size(X_size+P_size), lookup(*new vector<int>(n, -1)), vals(*new vector<int>(n, -1)){

    //     int cur_loc = 0;

    //     for (int i=0; i<X_size;i++){
    //         int elm = X_table[i];
    //         lookup[elm] = cur_loc;
    //         vals[cur_loc] = elm;
    //         cur_loc++;
    //     }

    //     for (int i=0; i<P_size;i++){
    //         int elm = P_table[i];
    //         lookup[elm] = cur_loc;
    //         vals[cur_loc] = elm;
    //         cur_loc++;
    //     }
    
    // }

    void operator=(X_P_Set sets){
        n = sets.n;
        X_size = sets.X_size;
        P_start = sets.P_start;
        P_size = sets.P_size;
    }

    //iteratate over neigbours and count intersection size and swap 
    X_P_Set get_intersection(vector<int>& neighbours){ return get_intersection(neighbours.data(), neighbours.size()); }

    X_P_Set get_intersection(int* neighbours, int size){
        int new_p_size = 0;
        int new_x_size = 0;

        int elm;
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
        return X_P_Set(*this,new_x_size, new_p_size);
    };
    

    int get_intersection_P_size(vector<int>& neighbours){return get_intersection_P_size(neighbours.data(), neighbours.size());}

    int get_intersection_P_size(int* neighbours, int size){
        int new_p_size = 0;
        int elm;
        for (int i = 0; i < size; i++){
            elm = neighbours[i];
            if (in_P(elm)){
                new_p_size++;
            }
        }
        return new_p_size;
    };


    //will add all intersecting neigbours as exclusions
    X_P_Set get_exclusion(vector<int>& neighbours){
        return get_exclusion(neighbours.data(), neighbours.size());
    }

    X_P_Set get_exclusion(int* neighbours, int size){
        int new_p_size = P_size;
        int new_x_size = X_size;
        int org_undo_size = undo_queue.size();
        for (int i = 0; i < size; i++){
            int elm = neighbours[i];

            if (in_P(elm)){
                do_swap(get_Pi(new_p_size-1), elm);
                new_p_size--;
            }
            else if (in_X(elm)){
                do_swap(get_Xi(new_x_size-1), elm);
                new_x_size--;
            }
        }
        return X_P_Set(*this, new_x_size, new_p_size, org_undo_size);
    };

    //precondition is that element is in P
    bool add_exclusion(int elm){
        if (!in_P(elm)){
            return false;
        }
        undo_queue.push_back(elm);
        do_swap(elm, get_Pi(0)); //only here should elements be added to the queue
        
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




    ~X_P_Set(){
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





