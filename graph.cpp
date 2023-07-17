#include <graph.h>

#pragma once

class Graph{
public:
    vector<vector<int>> edges_list;
    int size;

    void init_from_stdin(){
        int size;
        cin>>size;
        this->size = size;
        this->edges_list = vector<vector<int>>(size,vector<int>());


        for (int i = 0; i< size; i++){
            int vid, int edges;
            cin>>vid;
            cin>>edges;

            for (int j = 0; j<edges; j++){
                int edge;
                cin>>edge;
                edges_list[vid].push_back(edge);
            }
        }
    }

};

void intersection_set (unordered_set<int>& set, vector<int>& neigbours, unordered_set<int>& result){
    for (int i: neigbours){
        if (set.find(i) != set.end()){
            result.insert(i);
        }
    }
}

int intersection_set_size (unordered_set<int>& set, vector<int>& neigbours){
    int ans = 0;
    for (int i: neigbours){
        if (set.find(i) != set.end()){
            ans++;
        }
    }
    return ans;
}


void exclusion_set (unordered_set<int>& set, vector<int>& neigbours, unordered_set<int>& result){

}