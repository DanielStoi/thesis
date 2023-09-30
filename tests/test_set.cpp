#include <cassert>
#include <iostream>
#include "../els_set.h"
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
    X_P_Set set(g.edges_list.size(), g);

    //test init
    assert(set.get_Xi(0)==-1);
    set.print_everything();
    for (int i = 0; i<5; i++){
        assert(set.in_P(i));
        assert(!set.in_X(i));
        //assert(set.get_Pi(i)==i);
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
    X_P_Set set(g.edges_list.size(), g);
    set.add_exclusion(2);

    set = set.get_intersection(2);
    //should be P: {0,1} 
    set.print_everything();
    assert(set.in_P(0));
    assert(set.in_P(1));
    assert(!set.in_X(2));
    assert((!set.in_P(3) && !set.in_X(3)));
    assert((!set.in_P(4) && !set.in_X(4)));
    assert(set.get_Xi(1)==-1);
    assert(set.get_Xi(0)==-1);
    assert(set.get_Pi(2)==-1);
    cout<<set.get_Pi(0)<<" "<<set.get_Pi(1)<<endl;
    assert(set.get_Pi(0)==0 || set.get_Pi(0)==1);
    assert(set.get_Pi(1)==0 || set.get_Pi(1)==1);
    assert(set.get_Pi(0) != set.get_Pi(1));

}


void test_iter(){
    Graph g = init_basic_graph();
    X_P_Set set(g.edges_list.size(), g);
    unordered_set<int> seen;
    for (int i = 0; i<5; i++){
        int val = set.get_Pi(i);
        assert (0<=val);
        assert(5>val);
        assert(seen.count(val)==0);
        seen.insert(val);
    }
    assert(set.get_Pi(5) == -1);
}

void test_iter_exc(){
    Graph g = init_basic_graph();
    X_P_Set set_org(g.edges_list.size(), g);
    set_org.add_exclusion(4);
    set_org.add_exclusion(2);
    vector<int> exclist{1,4};
    printf("int size: %d\n", set_org.get_intersection_P_size(exclist));
    X_P_Set set = set_org.get_exclusion(exclist);
    unordered_set<int> seen;
    for (int i = 0; i<2; i++){
        int val = set.get_Pi(i);
        assert (0<=val);
        assert(5>val);
        assert(val !=5 && val != 1 && val !=2);
        assert(seen.count(val)==0);
        seen.insert(val);
    }
    
    set.print_everything();
    printf("%d\n", set.get_Pi(3));
    assert(set.get_Pi(3) == -1);
}

int main(){
    test_basic();
    test_intersection();
    test_iter();
    test_iter_exc();
}


