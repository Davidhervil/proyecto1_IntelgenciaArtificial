/*

*/

#include <vector>
#include <queue>
#include <map>
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
    int goalIter, ruleid, bestF;
    long long nodesAtD, bound;
    ruleid_iterator_t iter;
    map<std::uint64_t, short> colors;

    PriorityQueue<state_t> open; // used for the states we have generated but not yet expanded (the OPEN list)
    state_map_t *map = new_state_map(); // contains the cost-from-init-to-goal
    FILE *file; // the final state_map is written to this file if it is provided (command line argument)

    //RECORDAR QUE HAY QUE LEER UN ESTADO PARA INICIAR LA BUSQUEDA
    printf("Please enter a state followed by ENTER: ");
    if( fgets(str, sizeof str, stdin) == NULL ) {
        printf("Error: empty input line.\n");
        return 0; 
    }

    // CONVERT THE STRING TO A STATE
    nchars = read_state(str, &state);
    if( nchars <= 0 ) {
        printf("Error: invalid state entered.\n");
        return 0; 
    }

    // add goal states
    state_map_add(map, &state, 0);
    colors.insert(std::pair<uint64_t,short>(hash_state(&state),0));

    bound = 0;
    nodesAtD = 0;
    while( !open.Empty() ) {
        
        bestF = open.CurrentPriority();
        // remove top state from priority state
        state = open.Top();
        open.Pop();

        const int *known_dist = state_map_get(map, &state);
        assert(known_dist != NULL);
        if( *known_dist > bestF ){
            state_map_add(map, &state, d);
            if(is_goal(&state)){
                printf("SOLVED :D\n");
                return;
            }

            init_fwd_iter(&iter, &state);
            while( (ruleid = next_ruleid(&iter) ) >= 0 ) {
                apply_fwd_rule(ruleid, &state, &child);

                // Se supone que if h(s) < INFNIY pero en los problemas a tratar
                // siempre se satisface

                colors.insert(std::pair<uint64_t,short>(hash_state(&child),0));
                const int child_d = d + get_fwd_rule_cost(ruleid);

                open.Add(child_d, child_d, child);
                
                // check if either this child has not been seen yet or if
                // there is a new cheaper way to get to this child.
                const int *old_child_d = state_map_get(map, &child);
                if( (old_child_d == NULL) || (*old_child_d > child_d) ) {
                    // add to open with the new distance
                    state_map_add(map, &child, child_d);
                    open.Add(child_d, child_d, child);
                }
            }
        }

        nodesAtD = bounded_dfs_visit(state, 0, bound);
        printf("%lld nodes at depth %lld\n", nodesAtD, bound);
        bound++;
        
    }
    
    return 0;
}

