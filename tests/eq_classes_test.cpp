#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>

#include "../graph.h"
#include "eg_graphs.cpp"
#include "../docs/equiv_classes.h"





void test_basic(){
    Graph g;
    vector<vector<int>> edges = {
        {1,2,3},
        {0,3},
        {0,3},
        {0},
        {},
        {}
    };
    g.edges_list = edges;
    g.size = edges.size();
    Graph ans;
    unordered_map<unsigned int, unsigned int> counts;
    compute_equivalence_classes(g,ans, counts);
    
    ans.print_graph();
    cout<<ans.edges_list.size()<<endl;
    for (auto p: counts){
        printf("%d:%d\n", p.first, p.second);
    }
}



void test_basic2(){
    Graph g = create_graph_eg1();
    printf("---\n");


    
}





int main(){
    test_basic();
    //test_basic2();
    return 0;
}