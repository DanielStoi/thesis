to run:

g++ run/run_algs.cpp


this combines all version of the algorithm in which command line arguments can be used to specify the target dataset and exact algorithm


configs:

X, P sets have varying implementations. To change the implementation of X_P_Set:  go within els_set.h and change the include statement to one of the other implementation that are commented out.



ELS_SET PERFORMANCE ON pivotopt (pivot alg with best pivot choice)

base:
(simple els_set implementation that also undos moves from P->X which all versions do)
26s


edgeswap:
(swaps the edges so elements in P are at front)
40s

adjlookup:        
(has n^2 edge adj list lookuptable with relevent optimisations)
16s

adjlookup smart:   
(same as last but for intersection: chooses to iterate over either edge list or elements in P,X depending on which is smaller)
11s

