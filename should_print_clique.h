#pragma once
bool should_print_clique = false;

void print_clique (vector<int>& c){
    printf("clique: ");
        for (auto v: c){
            printf(" %d",v);
        }
        printf("\n");
}