#include <cassert>
#include <iostream>
#include "../els_set.cpp"
#include "../graph.h"

using namespace std;

Graph init_basic_graph(){
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
    return g;
}

/*

get_intersection(neigbours, size) //or vertex
get_exclusion(neigbours, size) //or vertex
add_exclusion(elm);
in_P(int elm);
in_X(int elm);
get_Pi(int i); //get ith elm in p
get_Xi(int i); //get ith elm in p



*/



void test_basic(){
    Graph g = init_basic_graph();
    X_P_Set set(5);

    //test init
    assert(set.get_Xi(0)==-1);
    for (int i = 0; i<5; i++){
        assert(set.in_P(i));
        assert(!set.in_X(i));
        assert(set.get_Pi(i)==i);
    }

    //test add exclusion
    assert(set.add_exclusion(2));
    assert(set.X_size==1);
    assert(set.get_Xi(0)==2);
    assert(set.in_X(2));
    assert(!set.in_P(2));

    for (int i = 0; i<5; i++){
        if (i==2) continue;
        assert(set.in_P(i));
        assert(!set.in_X(i));
        assert(set.get_Pi(i)!=2);
    }
}

void test_intersection(){
    Graph g = init_basic_graph();
    X_P_Set set(5);
    set.add_exclusion(2);

    set = set.get_intersection(g.edges_list[2]);
    //should be P: {0,1} 
    set.print_everything();
    assert(set.in_P(0));
    assert(set.in_P(1));
    assert(!set.in_X(2));
    assert((!set.in_P(3) && !set.in_X(3)));
    assert((!set.in_P(4) && !set.in_X(4)));
    assert(set.get_Xi(1)==-1);
    assert(set.get_Xi(0)==2);
    assert(set.get_Pi(2)==-1);
    cout<<set.get_Pi(0)<<" "<<set.get_Pi(1)<<endl;
    assert(set.get_Pi(0)==0 || set.get_Pi(0)==1);
    assert(set.get_Pi(1)==0 || set.get_Pi(1)==1);
    assert(set.get_Pi(0) != set.get_Pi(1));

}

int main(){
    test_basic();
    test_intersection();
}


