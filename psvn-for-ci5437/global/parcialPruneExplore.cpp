/*

*/

#include <vector>
#include <queue>
#include "priority_queue.hpp"

FILE *file;

void death_handler(int signal){
    printf("DYING\n");
    fclose(file);
    exit(signal);
}

long long bounded_dfs_visit (state_t state, long long d, long long bound, int hist){
    int ruleid, child_hist;
    long long nodes=0;
    state_t child;
    ruleid_iterator_t iter;

    if (d >= bound ) return 1;
    
    init_bwd_iter(&iter, &state);
    while( (ruleid = next_ruleid(&iter) ) >= 0 ) {
        if (!bwd_rule_valid_for_history(hist,ruleid))
            continue;
        child_hist = next_bwd_history(hist,ruleid);
        apply_bwd_rule(ruleid, &state, &child);
        long long current = bounded_dfs_visit(child, d+1, bound, child_hist);
        nodes += current;
    }
    return nodes;

}

int main(int argc, char **argv) {
    state_t state, child;   // NOTE: "child" will be a predecessor of state, not a successor
    int goalIter, ruleid, hist;
    long long nodesAtD, bound;
    ruleid_iterator_t iter;

    signal(SIGKILL, death_handler);
    signal(SIGINT, death_handler);
    signal(SIGTERM, death_handler);

    //PriorityQueue<state_t> open; // used for the states we have generated but not yet expanded (the OPEN list)
    //std::queue<std::pair<state_t,int> > la_queue;
    //state_map_t *map = new_state_map(); // contains the cost-to-goal for all states that have been generated
    file = fopen(argv[1], "w"); // the final state_map is written to this file if it is provided (command line argument)
    if (file==NULL){
        file = stdout;
    }

    // add goal states
    first_goal_state(&state, &goalIter);

    bound = 0;
    nodesAtD = 0;
    while( true ) {
        
        hist = init_history;        
        nodesAtD = bounded_dfs_visit(state, 0, bound, hist);
        fprintf(file, "%lld nodes at depth %lld\n", nodesAtD, bound);
        bound++;
        
    }
    
    return 0;
}

