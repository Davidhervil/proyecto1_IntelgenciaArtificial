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

int main(int argc, char **argv) {
    state_t state, child;   // NOTE: "child" will be a predecessor of state, not a successor
    int d, ruleid;
    long long nodesAtD;
    ruleid_iterator_t iter;
    std::pair<state_t,int> current; 

    //PriorityQueue<state_t> open; // used for the states we have generated but not yet expanded (the OPEN list)
    std::queue<std::pair<state_t,int> > la_queue;
    state_map_t *map = new_state_map(); // contains the cost-to-goal for all states that have been generated
    FILE *file; // the final state_map is written to this file if it is provided (command line argument)

    // add goal states
    first_goal_state(&state, &d);
    do {
        state_map_add(map, &state, 0);
        la_queue.push(std::make_pair(state, 0));
    } while( next_goal_state(&state, &d) );

    d = 0;
    nodesAtD = 0;
    while( !la_queue.empty() ) {
        // get current distance from goal; since operator costs are
        // non-negative this distance is monotonically increasing
        //d = open.CurrentPriority();

        // remove top state from priority state
        current = la_queue.front();
        state = current.first;
        if(d!=current.second){
            printf("%lld nodes at depth %d\n", nodesAtD, d);
            nodesAtD = 0;
        }
        d = current.second;
        la_queue.pop();
        nodesAtD++;
        
        // check if we already expanded this state.
        // (entries on the open list are not deleted if a cheaper path to a state is found)
        //const int *best_dist = state_map_get(map, &state);
        //assert(best_dist != NULL);
        //if( *best_dist < d ) continue;
        
        // print the distance then the state
        //printf("%d  ",d);
        //print_state(stdout,&state);
        //printf(" \n");

        // look at all predecessors of the state
        init_bwd_iter(&iter, &state);
        while( (ruleid = next_ruleid(&iter) ) >= 0 ) {
            apply_bwd_rule(ruleid, &state, &child);
            const int child_d = d + 1;//get_bwd_rule_cost(ruleid);

            // check if either this child has not been seen yet or if
            // there is a new cheaper way to get to this child.
            const int *color = state_map_get(map, &child);
            if( (color == NULL) ) {
                // add to open with the new distance
                state_map_add(map, &child, 0);
                la_queue.push(std::make_pair(child,child_d));
                //open.Add(child_d, child_d, child);
            }
        }
        state_map_add(map, &state, 1);
    }
    
    // write the state map to a file
    /*if( argc >= 2 ) {
        file = fopen(argv[1], "w");
        if( file == NULL ) {
            fprintf(stderr, "could not open %s for writing\n", argv[1]);
            exit(-1);
        }
        write_state_map(file, map);
        fclose(file);
    }*/
    
    return 0;
}

