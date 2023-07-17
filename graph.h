#include <unordered_set>
#include <iostream>
#include <vector>
#include <memory>

#pragma once

using namespace std;


void print_vector(vector<int>& ls){
    if (ls.size() == 0){
        cout<<"[]\n";
        return;
    }
    cout<<"["<<ls[0];
    for (int i = 1; i<ls.size();i++){
        cout<<", "<<ls[i];
    }
    cout<<"]\n";
}

class Graph{
public:
    vector<vector<int>> edges_list;
    int size=0;


    Graph(){}


    void add_node(){
        size++;
        edges_list.push_back(vector<int>());
    }
    void add_edge(int a, int b){
        edges_list[a].push_back(b);
        edges_list[b].push_back(a);
    }

    bool verify_self(){
        vector<unordered_set<int>>seen;

        auto is_valid = true;
        //adding edges to set
        for (int i = 0; i< edges_list.size(); i++){
            seen.push_back({});
            for (auto e: edges_list[i]){
                if (seen[i].find(e) != seen[i].end()){
                    printf("ERROR: duplication of edge present: %d is repeated within %d edge list\n",e,i);
                    is_valid = false;
                }
                seen[i].insert(e);
            }
        }

        //verifying correctness
        for (int i = 0; i < edges_list.size(); i++){
            for (auto e: edges_list[i]){
                if (seen[e].find(i) == seen[e].end()){
                    printf("ERROR: direct edge present from %d to %d but not other way around\n",i,e);
                    is_valid = false;
                }
            }
        }

        printf("correctness: %s on %ld vertexes\n", is_valid ? "true" : "false", seen.size());
        return is_valid;
    }
    


    void print_graph(){
        printf("vertex_count--- expected: %d got %ld\n", size, edges_list.size());
        for (int v = 0; v< edges_list.size(); v++){
            printf("vertex %d: ",v);
            for (auto e: edges_list[v]){
                printf(" %d",e);
            }
            printf("\n");

        }
    }

    //not my own work - taken from:
    //https://github.com/LijunChang/Cohesive_subgraph_book/blob/master/utilities/Defines.h
    //on 30/04/2023
    //credit to: Lijun Cheng
    static FILE *open_file(const char *file_name, const char *mode) {
		FILE *f = fopen(file_name, mode);
		if(f == nullptr) {
			printf("Can not open file: %s\n", file_name);
			exit(1);
		}

		return f;
	}


    //below function is not my own work - taken from:
    //https://github.com/LijunChang/Cohesive_subgraph_book/blob/master/core_decomposition/Graph.cpp
    //on 30/04/2023
    //credit to: Lijun Cheng   
    Graph(const char* dir){
        printf("# Start reading graph, Require files \"b_degree.bin\" and \"b_adj.bin\"\n");
        FILE *f = open_file((dir + string("/b_degree.bin")).c_str(), "rb");

        unsigned int tt;
        fread(&tt, sizeof(unsigned int), 1, f);
        if(tt != sizeof(unsigned int)) {
            printf("sizeof unsigned int is different: b_degree.bin(%u), machine(%lu)\n", tt, sizeof(unsigned int));
            return ;
        }

        unsigned int edge_count;
        unsigned int vertex_count;

        fread(&vertex_count, sizeof(unsigned int), 1, f);
        fread(&edge_count, sizeof(unsigned int), 1, f);
        size = vertex_count;
        edges_list = vector<vector<int>>(size, vector<int>{});

        printf("# Reading graph with %d vertixes and %d edges\n", vertex_count, edge_count);


        //printf("*\tn = %s; m = %s (undirected)\n", Utility::integer_to_string(n).c_str(), Utility::integer_to_string(m/2).c_str());

        unsigned int *degree = new unsigned int[vertex_count];
        fread(degree, sizeof(unsigned int), vertex_count, f);

        fclose(f);



        unsigned int *pstart; //start positions of neighbors of vertices in the array "edges"
	    unsigned int *edges; //concatenation of neighbors of all vertices

        f = open_file((dir + string("/b_adj.bin")).c_str(), "rb");


        pstart = new unsigned int[vertex_count+1];
        edges = new unsigned int[edge_count];

        pstart[0] = 0;
        for(unsigned int i = 0;i < vertex_count;i ++) {
            if(degree[i] > 0) fread(edges+pstart[i], sizeof(unsigned int), degree[i], f);
            for (int e = 0; e<degree[i]; e++){
                edges_list[i].push_back(edges[pstart[i]+e]);
            }
            pstart[i+1] = pstart[i] + degree[i];
        }


        fclose(f);

        //delete[] degree;
        //delete[] pstart;
   //     delete[] edges;
    }

};




