#include "../graph.h"

Graph create_graph_eg1(){
    Graph g = Graph();
    g.add_node();
	g.add_node();
	g.add_node();
	g.add_node();
	g.add_node();
	g.add_node();
	
	
	g.add_edge(0, 1);
	g.add_edge(0, 2);
    g.add_edge(0, 3);
    g.add_edge(0, 4);
    g.add_edge(0, 5);
	g.add_edge(1, 2);
	g.add_edge(1, 3);
	g.add_edge(3, 4);
	g.add_edge(5, 4);
	g.add_edge(3, 5);
    return g;
}

Graph create_graph_eg2(){
    Graph g = Graph();
    g.add_node(); 
	g.add_node();
	g.add_node();
	g.add_node();
	g.add_node();
    g.add_node();
    //0 -> 7
	
	
	g.add_edge(0, 1);
	g.add_edge(0, 2);
	g.add_edge(1, 2);
	g.add_edge(1, 3);
	g.add_edge(3, 4);
	g.add_edge(5, 4);
	g.add_edge(3, 5);
    return g;
}