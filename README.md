# to run:

g++ run/run_algs.cpp


this combines all version of the algorithm in which command line arguments can be used to specify the target dataset and exact algorithm

read comments in run_algs.cpp for additional reference


# configs:

X, P sets have varying implementations and run options:

- els_set.h :To change the implementation of X_P_Set: go within els_set.h and change the include statement to one of the other implementation that are commented out.
- adj lookup els_set specific configs:
  -  els_set_edgeswap_lookup_map - #define SMART_INTERSECTION_CHOICE
  - edge_lookup.h - #define USE_HASHMAP_ELS_SET_LOOKUP  


