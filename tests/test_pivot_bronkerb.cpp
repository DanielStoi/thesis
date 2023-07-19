#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>

#include "../BronKerboschPivotlist_P_only.cpp"
#include "../graph.h"
#include "eg_graphs.cpp"
#include "correct_tester.cpp"


void test_basic(){
    Graph g;
    vector<vector<int>> edges = {
        {1,2,3},
        {0,2},
        {0,1},
        {0},
        {}
    };
    g.edges_list = edges;
    g.size = edges.size();
    cout<<"start init\n";
    BronKerboschPivot sol(g, false);
    cout<<"init done\n\n\n";
    int ans = sol.solve();
    cout<<"amount of solutions: "<<ans<<endl;
    get_correct(g);
}

void test_basic2(){
    Graph g = create_graph_eg1();
    printf("---\n");
    g.print_graph();

    BronKerboschPivot sol(g, false);
    cout<<"init done\n\n\n";
    int ans = sol.solve();
    cout<<"amount of solutions: "<<ans<<endl;
    g.verify_self();

    get_correct(g);
    
}

int main(){
    test_basic();
    test_basic2();
    return 0;
}