#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>

#include "../degeneracy_ordering.cpp"
#include "../graph.h"

using namespace std;


/*
Lin_Heap:
    pop_vertex
    decrement_elm
    add
    remove
    get_degeneracy_ordering



*/

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


void test_init_function(){
    //also tests add function since init uses add
    Graph g = init_basic_graph();
    Lin_Heap hp = Lin_Heap(&g);
    
    assert(hp.head[4]==-1);
    assert(hp.head[3]==0);
    assert(hp.head[2]!=-1);
    assert(hp.head[1]==3);
    assert(hp.head[0] == 4);
    
    assert(hp.next[3]==-1);
    assert(hp.next[4] == -1);


    assert(hp.in_list[4]==0);
    assert(hp.in_list[3]==1);
    assert(hp.in_list[2]==2);
    assert(hp.in_list[1]==2);
    assert(hp.in_list[0]==3);


    vector<int> l2{hp.head[2], hp.next[hp.head[2]], hp.next[hp.next[hp.head[2]]]};
    sort(l2.begin(), l2.end());
    assert(l2[0]==-1);
    assert(l2[1]==1);
    assert(l2[2]==2);
    assert(hp.head[3]==0);
    assert(hp.next[0]==-1);
    assert(hp.smallest==0);
    assert(hp.head[4]==-1);
}

void test_basic_degen2(){
    Graph g;
    vector<vector<int>> edges = {
        {1,2,3},
        {0,2},
        {0,3},
        {0,1}
    };
    g.edges_list = edges;
    g.size = edges.size();
    Lin_Heap hp = Lin_Heap(&g);
    
    vector<int> ans;
    
    int res = hp.get_ordering(ans);
    cout<<res<<endl;

    assert(res == 2);
    assert(ans.size()==4);
    assert(ans[3]==0);
    sort(ans.begin(), ans.end());
    assert((ans == vector<int>{0,1,2,3}));
}



void test_basic_degen(){
    Graph g = init_basic_graph();
    Lin_Heap hp = Lin_Heap(&g);
    
    vector<int> ans;
    int res = hp.get_ordering(ans);

    cout<<"res:"<<res<<"size:"<<ans.size()<<endl;
    assert(res==2);
    assert(ans.size() == 5);
    assert(ans[0]== 4);
    assert(ans[1]== 3);
}

void test_basic_degen3(){
    vector<vector<int>> edges(10, vector<int>());
    Graph g;
    g.edges_list = edges;
    Lin_Heap hp = Lin_Heap(&g);
    
    vector<int> ans;
    int res = hp.get_ordering(ans);

    cout<<"res:"<<res<<"size:"<<ans.size()<<endl;
    //assert(res==1);
    assert(ans.size() == 5);
    assert(ans[0]== 4);
    assert(ans[1]== 3);
    assert(ans[2] == 2 || ans[2] == 1);
    assert(ans[3] == 2 || ans[3] == 1);
    assert(ans[2] != ans[3]);
    assert(ans[4] == 0);

}


int main(){
    test_init_function();
    test_basic_degen2();
    test_basic_degen();
    return 0;
}



