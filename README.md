# to run:

g++ run/run_algs.cpp


this combines all version of the algorithm in which command line arguments can be used to specify the target dataset and exact algorithm


# configs:

X, P sets have varying implementations and run options:

- els_set.h :To change the implementation of X_P_Set: go within els_set.h and change the include statement to one of the other implementation that are commented out.
- adj lookup els_set specific configs:
  -  els_set_edgeswap_lookup_map - #define SMART_INTERSECTION_CHOICE
  - edge_lookup.h - #define USE_HASHMAP_ELS_SET_LOOKUP  




# Basic runtime tests on brock200_2
## ELS_SET PERFORMANCE ON pivotopt (pivot alg with best pivot choice)

__base:__

(simple els_set implementation that also undos moves from P->X which all versions do)

26s


__edgeswap:__

(swaps the edges so elements in P are at front)

40s

__adjlookup:__   

(has n^2 edge adj list lookuptable with relevent optimisations)

16s

__adjlookup smart:__   

(same as last but for intersection: chooses to iterate over either edge list or elements in P,X depending on which is smaller)

11s

