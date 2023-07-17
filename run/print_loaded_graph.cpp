#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>

#include "../BronKerbosch.cpp"
#include "../graph.h"



int main(int argc, char** argv){
    if (argc<2){
        printf("expected to input graph file in first arg\n");
        return 0;
    }
    Graph g(argv[1]);
    //g.print_graph();
}