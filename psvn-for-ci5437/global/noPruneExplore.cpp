/*
This program computes the distance to goal (i.e. the cost of the least-cost path to the goal)
of every state from which the goal can be reached.
It does this by executing Dijkstra's algorithm backwards from the goal.
It prints on stdout each state and its distance (distance first, then the state) and, if a filename is
provided as a command line argument, it prints the state_map it builds to that file.

Copyright (C) 2013 by the PSVN Research Group, University of Alberta
*/

#include <vector>
#include <queue>
#include "priority_queue.hpp"

long long bounded_dfs_visit (state_t state, long long d, long long bound){
    int ruleid;
    long long nodes=0;
    state_t child;
    ruleid_iterator_t iter;

    if (d >= bound ) return 1;
    
    init_bwd_iter(&iter, &state);
    while( (ruleid = next_ruleid(&iter) ) >= 0 ) {
        apply_bwd_rule(ruleid, &state, &child);
        long long current = bounded_dfs_visit(child, d+1, bound);
        nodes += current;
    }
    return nodes;

}

int main(int argc, char **argv) {
    state_t state, child;   // NOTE: "child" will be a predecessor of state, not a successor
    int goalIter, ruleid;
    long long nodesAtD, bound;
    ruleid_iterator_t iter;

    //PriorityQueue<state_t> open; // used for the states we have generated but not yet expanded (the OPEN list)
    //std::queue<std::pair<state_t,int> > la_queue;
    //state_map_t *map = new_state_map(); // contains the cost-to-goal for all states that have been generated
    FILE *file; // the final state_map is written to this file if it is provided (command line argument)

    // add goal states
    first_goal_state(&state, &goalIter);

    bound = 0;
    nodesAtD = 0;
    while( true ) {
        
        nodesAtD = bounded_dfs_visit(state, 0, bound);
        printf("%lld nodes at depth %lld\n", nodesAtD, bound);
        bound++;
        
    }
    
    return 0;
}
