#include <vector>
#include <list>
#include "graph.h"
#include "ordering.h"


using namespace std;



//linear heap data structure
class Lin_Heap: public ordering{
public:
    vector<int> head;
    vector<int> next;
    vector<int> prev;
    vector<int> in_list;

    int smallest = 0;
    Graph* g;
    int max_degen = 0;

    Lin_Heap(Graph* g){
        //want to a
        head = vector<int>(g->size, -1);
        next = vector<int>(g->size, -1);
        prev = vector<int>(g->size, -1);
        in_list = vector<int>(g->size, -1);
        for (int v = 0; v< g->size; v++){
            add(v, g->edges_list[v].size());
        }
        this->g = g;
    }

    //returns the degeneracy
    int get_ordering(vector<int>& arr) override{
        max_degen = smallest;
        int elm = pop_vertex();
        while (elm != -1){
            arr.push_back(elm);
            elm = pop_vertex();
        }

        return max_degen;
    }

    bool remove(int elm){
        if (in_list[elm] == -1) return false;
        int bucket = in_list[elm];
        in_list[elm] = -1;

        if (prev[elm] != -1){
            next[prev[elm]] = next[elm];
        }
        else{
            head[bucket] = next[elm];
        }

        if (next[elm] != -1){
            prev[next[elm]] = prev[elm];
        }
        return true;
    }

    bool add(int elm, int bucket){
        if (in_list[elm] != -1) return false;
        int old_head = head[bucket];
        if (old_head != -1){
            prev[old_head] = elm;
        }
        prev[elm] = -1;
        next[elm] = old_head;
        head[bucket] = elm;
        in_list[elm]= bucket;
        return true;
    }

    bool decrement_elm(int elm){
        int bucket = in_list[elm];
        if (bucket<=0) return false;
        remove(elm);
        add(elm, bucket-1);
        if (smallest==bucket){
            smallest--;
        }
        return true;
    }

    int pop_vertex(){
        while (smallest>=0 && smallest < head.size() && head[smallest] == -1){
            smallest++;
        } 
        
        if (smallest >= head.size()) return -1; 
        max_degen = max(max_degen, smallest); 

        int elm = head[smallest];
        remove(elm);

        for (int neigbour: g->edges_list[elm]){
            decrement_elm(neigbour);
        }

        return elm;
    }
};