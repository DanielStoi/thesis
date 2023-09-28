/*
 * common_functions.h
 *
 *  Created on: 29May,2018
 *      Author: ljchang@outlook.com
 */

#ifndef COMMON_FUNCTIONS_H_
#define COMMON_FUNCTIONS_H_

#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#ifndef NDEBUG
#define NDEBUG
#endif

#include <cassert>

namespace LJ_CommonFunctions {
	// compute independent set-equivalence classes (self_loop = 0): u and v are in the same equivalence class iff (u,v) \notin E, and N^-(u) = N^-(v)
	// compute clique-equivalence classes (self_loop = 1): u and v are in the same equivalence class iff (u,v) \in E, and N^-(u) = N^-(v)
	// the resulting equivalence classes are represented by eq_class_starts and eq_class_ids
	void compute_equivalence_classes(unsigned int &eq_class_n, unsigned int *eq_class_starts, unsigned int *eq_class_ids, const unsigned int n, const unsigned int *pstart, const unsigned int *pend, const unsigned int *edges, char self_loop = 0) {
		unsigned int *vertex2set = new unsigned int[n];
		for(unsigned int i = 0;i < n;i ++) vertex2set[i] = 0; //all vertices are assigned to set 0

		unsigned int *queue = eq_class_ids;
		unsigned int *candidate_setIDs = eq_class_starts; //available setIDs
		unsigned int *new_setID = new unsigned int[n+1]; //mapping from old setID to new setID
		unsigned int *set_size = new unsigned int[n+1]; //number of vertices in each set

		for(unsigned int i = 0;i <= n;i ++) {
			new_setID[i] = n+1;
			set_size[i] = 0;
			candidate_setIDs[i] = i;
		}
		set_size[0] = n;

		unsigned int candidate_setIDs_head = 1;

		for(unsigned int i = 0;i < n;i ++) {
			unsigned int queue_n = 0;
			/*if(self_loop) {
				unsigned int v = i;
				unsigned int old_setID = vertex2set[v];
				if(new_setID[old_setID] > n) {
					new_setID[old_setID] = candidate_setIDs[candidate_setIDs_head ++];
					queue[queue_n ++] = old_setID;
				}
				vertex2set[v] = new_setID[old_setID];
				++ set_size[vertex2set[v]];
				if(-- set_size[old_setID] == 0) candidate_setIDs[-- candidate_setIDs_head] = old_setID;
			}*/
			for(unsigned int j = pstart[i];j < pend[i];j ++) {  
				unsigned int v = edges[j];          //iterating over edges v
				unsigned int old_setID = vertex2set[v];

				if(new_setID[old_setID] > n) {    //assigning a new_setID for edge if doesn't exist
					if(set_size[old_setID] == 1) continue; //doesn't move to another set

					new_setID[old_setID] = candidate_setIDs[candidate_setIDs_head ++]; //allocate  new set
					queue[queue_n ++] = old_setID; //
				}
				vertex2set[v] = new_setID[old_setID];
				++ set_size[vertex2set[v]];

				if(-- set_size[old_setID] == 0)  candidate_setIDs[-- candidate_setIDs_head] = old_setID;
				    //re-adds oldSetID back to the "pool" if emptied
			}
			while(queue_n > 0) new_setID[queue[-- queue_n]] = n+1;   
			//resetting new_setID for next round
		}


		//setting up sizes and mapping for eventual writing	
		eq_class_n = 0;
		eq_class_starts[0] = 0;
		for(unsigned int i = 0;i < n;i ++) {
			unsigned int setID = vertex2set[i];
			if(new_setID[setID] > n) { //if set hasn't been added already: add it
				new_setID[setID] = eq_class_n ++;
				eq_class_starts[eq_class_n] = eq_class_starts[eq_class_n-1] + set_size[setID];
			}
		}



		//writing elements into output format
		for(unsigned int i = 0;i < n;i ++) {
			unsigned int setID = new_setID[vertex2set[i]];
			eq_class_ids[eq_class_starts[setID] ++] = i;
		}

		for(unsigned int i = eq_class_n;i > 0;i --) eq_class_starts[i] = eq_class_starts[i-1];
		eq_class_starts[0] = 0;

#ifndef NDEBUG
		char *dvis = new char[n];
		memset(dvis, 0, sizeof(char)*n);

		assert(eq_class_starts[eq_class_n] == n);

		for(unsigned int i = 0;i < eq_class_n;i ++) if(eq_class_starts[i+1] > eq_class_starts[i]+1) {
			unsigned int u = eq_class_ids[eq_class_starts[i]];
			for(unsigned int j = pstart[u];j < pend[u];j ++) dvis[edges[j]] = 1;
			if(self_loop) dvis[u] = 1;

			for(unsigned int j = eq_class_starts[i]+1;j < eq_class_starts[i+1];j ++) {
				unsigned int v = eq_class_ids[j];
				if(self_loop) assert(dvis[v]);
				else assert(!dvis[v]);
				assert(pend[u] - pstart[u] == pend[v] - pstart[v]);
				for(unsigned int k = pstart[v];k < pend[v];k ++) assert(dvis[edges[k]]);
			}

			for(unsigned int j = pstart[u];j < pstart[u+1];j ++) dvis[edges[j]] = 0;
			dvis[u] = 0;
		}

		delete[] dvis;
#endif

		delete[] set_size;
		delete[] new_setID;
		delete[] vertex2set;
	}

	
}

#endif /* COMMON_FUNCTIONS_H_ */
