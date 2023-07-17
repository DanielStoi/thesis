/*
A X P set only used for reference


*/

#include <cstdlib>
#include <vector>
#include <iostream>

#pragma once

using namespace std;

class X_P_Set_base{
public:
    //start with X->P
    //contains lookup table for all elements
    //vals contains all edges bounded by a range
    int n;
    vector<int>& lookup; //table index->loc of element (-1 if doesn't exist within table)
    vector<int>& vals;  // raw combined array of X,P sets
    int X_size;
    int P_start;
    int P_size;


    //adds all elements to P by default
    X_P_Set_base(int n):
    n(n), X_size(0), P_start(0), P_size(n), lookup(*new vector<int>(n, -1)), vals(*new vector<int>(n, -1)){
        for (int i = 0; i<n; i++){
            lookup[i]=i;
            vals[i]=i;
        }
    };


    //constructor will just use same lookup and val table but change refs to pointers
    X_P_Set_base(int n, vector<int>& lookup, vector<int>& vals, int X_size, int P_start, int P_size):
    n(n), lookup(lookup), vals(vals), X_size(X_size), P_start(P_start), P_size(P_size) {};

    //copy constructor
    // X_P_Set_base(int n, int* P_table, int P_size, int* X_table, int X_size):
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

    void operator=(X_P_Set_base sets){
        n = sets.n;
        X_size = sets.X_size;
        P_start = sets.P_start;
        P_size = sets.P_size;
    }

    //iteratate over neigbours and count intersection size and swap 
    X_P_Set_base get_intersection(vector<int>& neighbours){ return get_intersection(neighbours.data(), neighbours.size()); }

    X_P_Set_base get_intersection(int* neighbours, int size){
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
        return X_P_Set_base(n,lookup,vals,new_x_size, P_start, new_p_size);
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
    X_P_Set_base get_exclusion(vector<int>& neighbours){
        return get_exclusion(neighbours.data(), neighbours.size());
    }

    X_P_Set_base get_exclusion(int* neighbours, int size){
        int new_p_size = P_size;
        int new_x_size = X_size;
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
        return X_P_Set_base(n,lookup,vals,new_x_size, P_start, new_p_size);
    };

    //precondition is that element is in P
    bool add_exclusion(int elm){
        if (!in_P(elm)){
            return false;
        }

        do_swap(elm, get_Pi(0));
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
        for (int d = 0; d<depth; d++) printf("   ");
        cout<<"P_start:"<<P_start;
        cout<<"  P_size:"<<P_size;
        cout<<"  X_size:"<<X_size<<endl;
        for (int d = 0; d<depth; d++) printf("   ");
        cout<<"vals: ";
        for (int i=0; i<n; i++){
            if (i == P_start) cout<<"|";
            if (i==P_start-X_size) cout<<"|";
            if (i==P_start+P_size) cout<<"|";
            cout<<vals[i]<<" ";
        }
        cout<<endl;
    }

};





