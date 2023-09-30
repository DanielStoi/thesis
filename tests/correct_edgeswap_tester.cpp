#include "../els_set.h"
#include <cassert>
#include <unordered_set>

using namespace std;
#pragma once

void check_edgeswap_conditon(Graph& g, X_P_Set& XP){
    int error_count = 0;
    for (int i = 0; i< XP.P_size; i++){
        int elm = XP.get_Pi(i);
        bool still_P = true;
        unordered_set<int> seen;
        for (auto e: g.edges_list[elm]){
            if (XP.in_P(e)){
                if (!still_P){
                    printf("voilated edgeswap condition: edgelist for %d has P element %d after non-P element\n", elm, e);
                    print_vector(g.edges_list[elm]);
                    XP.print_everything();
                    error_count++;
                }
            }
            else{
                still_P = false;
            }
            if (seen.count(e) > 0){
                printf("voilated edgeswap condition: edgelist for %d has duplicate element %d\n", elm, e);
                error_count++;
            }
            seen.insert(e);
        }
    }
    printf("detected %d edgeswap errors for P of size %d\n", error_count, XP.P_size);
    assert (!error_count);
}



void check_intersection_correctness(Graph& g, X_P_Set& XP_parent, X_P_Set& XP_child, int edge){
    int error_count = 0;
    //testing every edge is included correctly
    unordered_set<int> seen;
    for (auto elm: g.edges_list[edge]){
        bool valid = false;
        if (XP_parent.in_P(elm) && !XP_child.in_P(elm)){
            printf("violated intersection condition: missing element %d from child P\n", elm);
            error_count+=1;
        }
        else if (XP_parent.in_X(elm) && !XP_child.in_X(elm)){
            printf("violated intersection condition: missing element %d from child P\n", elm);
            error_count+=1;
        }
        else if (!XP_parent.in_P(elm) &&XP_child.in_P(elm)){
            printf("violated intersection condition: edge %d in child and edgelist but not in parent\n", elm);
            error_count+=1;
        }
        else if (!XP_parent.in_X(elm) && XP_child.in_X(elm)){
            printf("violated intersection condition: edge %d in child and edgelist but not in parent\n", elm);
            error_count+=1;
        }
        else{
            valid = true;
        }

        seen.insert(elm);
    }

    for (int i = 0; i<XP_child.P_size; i++){
        int elm = XP_child.get_Pi(i);
        if (seen.count(elm) == 0){
            printf("violated intersection condition: element %d in child but not in edgelist \n", elm);
            error_count+=1;
        }
    }
    printf("detected %d pure intersection errors for Ps of sizes %d->%d\n", error_count,XP_parent.P_size, XP_child.P_size);
    assert(!error_count);
}