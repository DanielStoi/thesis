#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <memory>
#include "linked_list.cpp"
#include <algorithm>    // std::sort
#include <graph.h>

using namespace std;

typedef linked_list<pair<int, linked_node<FreqBucket>*>> Vertex;
typedef pair<int, unordered_set<Vertex*>> FreqBucket;



struct vertex_linked;
struct edge_list_buckets;

struct edge_bucket{
    unordered_map<int, vertex_linked*> vertexes;
    int count = 0;

    vertex_linked* pop(int v){
        auto i = vertexes.find(v);
        if (i == vertexes.end()){
            return NULL;
        }
        vertex_linked* ans = i->second;
        vertexes.erase(i);
        return ans;
    }

    vertex_linked* pop(){
        if (vertexes.size()==0) return NULL;
        return pop(vertexes.begin()->first);
    }

    vertex_linked* find(int vid ){
        if (vertexes.find(vid) == vertexes.end()){
            return NULL;
        }
        return vertexes[vid];
    }
};

struct vertex_linked{
    int id;
    unordered_map<int, linked_node<edge_bucket*>*> edges;
    vertex_linked(int nid){
        id = nid;
    }
};

struct edge_list_buckets{
    linked_list<edge_bucket*> buckets;

    void downgrade(linked_node<edge_bucket*>* bucket, int v){
        int new_count = bucket->val->count - 1;


        vertex_linked* elm = bucket->val->pop(v);
        if (elm == NULL){
            cout<<"element "<<v<<" for downgrade not found"<<endl;
            return;
        }

        edge_bucket* prev_edge_bucket;
        if (bucket->prev == NULL || bucket->prev->val->count != new_count){
            prev_edge_bucket = new edge_bucket;
            prev_edge_bucket->count = new_count;
            buckets.insert_before(prev_edge_bucket, bucket);
        }
        else{
            prev_edge_bucket = bucket->prev->val;
        }

        prev_edge_bucket->vertexes[v] = elm;
    }


    void push_back(vertex_linked* v, int size){

        int back_size = -1;
        if  (buckets.size>0)
            back_size = buckets.tail->val->count;

        edge_bucket* bucket;
        if (back_size != size){
            bucket = new edge_bucket();
            bucket->count = size;
            buckets.push_back(bucket);
        }
        bucket->vertexes[v->id] = v;
    }

    int pop_and_update(){
        if (buckets.size == 0) return -1;

        if (buckets.head->val->vertexes.size() == 0){
            remove_node(buckets.head);
            return pop_and_update();
        }

        vertex_linked* v = buckets.head->val->pop();

        for (auto keyval: v->edges){
            int nid = keyval.first;
            linked_node<edge_bucket*>* nbucket = keyval.second;
            downgrade(nbucket, nid);
        }
    }
    
};

class DegeneracyOrdering{
public:
    edge_list_buckets edges_list;
    int size;


    //want to initialise the bucket structure from graph
    DegeneracyOrdering(Graph* g){
        vector<pair<int,int>> vertex_count;
        for (int v = 0; v< g->edges_list.size(); v++){
            vertex_count.push_back({g->edges_list[v].size(), v});

        }
        sort(vertex_count.begin(), vertex_count.end());

        //doing initial scan to set up edges_list
        unordered_map<int, linked_node<edge_bucket*>*> edge_locs;
        for(pair<int, int> p: vertex_count){
            int vid = p.second;
            int cur_freq = p.first;

            vertex_linked* v = new vertex_linked(vid);

            edges_list.push_back(v, cur_freq);
            edge_locs[vid] = edges_list.buckets.tail;
        }

        //real scan
        linked_node<FreqBucket>* cur_bucket = edges_list.buckets.head;
        while (cur_bucket != NULL){

            for(Vertex* p: cur_bucket->val.second){

                //adding itself to all the neigbours
                for (int neighbour: g->edges_list[p->id]){
                    vertex_linked* n = edge_locs[neighbour]->val->find(neighbour);
                    n->edges[p->id] = cur_bucket;
                }
            }
            cur_bucket = cur_bucket->next;
        }
    }


    void solve(vector<int>& result){
        int v = edges_list.pop_and_update();

        while (v!= -1){
            result.push_back(v);
            v = edges_list.pop_and_update();
        }
    }

};