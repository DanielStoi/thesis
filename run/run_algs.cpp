#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>

#include "../BronKerboschOrdering.cpp"
#include "../BronKerbosch.cpp"
#include "../BronKerboschPivotlist.cpp"
#include "../P_only_opt/BronKerboschPivotlist_P_only.cpp"
//#include "../BronKerboschEquivPlain.cpp"
//#include "../BronKerboschEquivDegen.cpp"
//#include "../BronKerboschEquivRecDegen.cpp"
#include "../graph.h"
#include "../correct_imp.cpp"


using namespace std;


/*

g++ run/run_algs.cpp -o run_algs
./run_algs datasets/brock200_2 normal

RUNNING IN DEBUG MODE:
g++ run/run_algs.cpp -o run_algs -g
gdb ./run_algs datasets/brock200_2 normal

bt    -- to see stacktrace
*/


string help = 
"usage: ./run_algs graphdir type\n"
"types: normal, pivot, pivotopt, degen, degenopt, correct\n";





int main(int argc, char** argv){
    if (argc<3){
        cout<<help;
        return 0;
    }
    Graph g(argv[1]);
    bool is_valid = g.verify_self();
    if (!is_valid) return 0;

    int ans = 0;
    if (strcmp(argv[2], "normal") == 0){
        ans = BronKerbosch(g).solve();
    }
    else if (strcmp(argv[2], "pivot") == 0){
        ans = BronKerboschPivot(g, false).solve();
    }
    else if (strcmp(argv[2], "pivotopt") == 0){
        ans = BronKerboschPivot(g, true).solve();
    }
    else if (strcmp(argv[2], "p_pivot_adj_ord") == 0){
        ans = BronKerboschPivot_P_only(g, true).solve();
    }
    else if (strcmp(argv[2], "p_pivotopt_adj_ord") == 0){
        ans = BronKerboschPivot_P_only(g, true).solve();
    }
    else if (strcmp(argv[2], "degen") == 0){
        ans = BronKerboschDegen(g, false).solve();
    }
    else if (strcmp(argv[2], "degenopt") == 0){
        ans = BronKerboschDegen(g, true).solve();
    }
    else if (strcmp(argv[2], "equiv") == 0){
        //ans = BronKerboschEquivReduction(g).solve();
    }
    else if (strcmp(argv[2], "equivdegen") == 0){
       // ans = BronKerboschEquivReductionDegen(g).solve();
    }
    else if (strcmp(argv[2], "equivrecdegen") == 0){
        //ans = BronKerboschEquivReductionRecursiveDegen(g).solve();
    }
    else if (strcmp(argv[2], "correct") == 0){
        auto c = correctImp();
        c.loadgraph(g);
        ans = c.find_all_cliques().size();
    }
    else{
        printf("invalid type: %s\n",argv[2]);
        return 0;
    }

    printf("found %d cliques\n", ans);
    return 0;
}