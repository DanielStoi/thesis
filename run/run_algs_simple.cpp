#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>


#include "../BronKerboschPivotList.cpp"
#include "../graph.h"
#include "../correct_imp.cpp"


using namespace std;


/*

g++ run/run_algs.cpp -o run_algs
./run_algs datasets/brock200_2 normal

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

    if (strcmp(argv[2], "pivot") == 0){
        ans = BronKerboschPivot(g, false).solve();
    }
    else if (strcmp(argv[2], "pivotopt") == 0){
        ans = BronKerboschPivot(g, true).solve();
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