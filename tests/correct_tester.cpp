#include <iostream>
#include "../correct_imp.cpp"
#include <vector>

void get_correct(Graph& g){
    printf("\ncorrect implementation:\n");
    correctImp c = correctImp();
    c.loadgraph(g);
    std::vector< std::set<int >  > cliques = c.find_all_cliques();
    printf("\nfound %ld clicks\n", cliques.size());
    for (auto clq : cliques){
        printf("clique: ");
        for (auto v: clq){
            printf(" %d",v);
        }
        printf("\n");
    }
}