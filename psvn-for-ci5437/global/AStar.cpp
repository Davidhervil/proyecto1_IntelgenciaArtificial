/*

*/

#include <vector>
#include <queue>
#include <map>
#include <ctime>
#include <sys/time.h>
#include <fstream>
#include <unistd.h>
#include "priority_queue.hpp"

using namespace std;

typedef struct 
{
    state_t state;
    int g;
    int hist;
}node;


vector<abstraction_t *> abstractions;    // Vector de abstracciones
vector<state_map_t *>   pdbs;
long long generatedCount, expandedCount;
time_t start,end;  // Tiempo de inicio
int timeout;

int h(int additive_h, state_t state){

    int value = 0;
    
    if (additive_h){
        for (unsigned int i=0; i<pdbs.size(); i++){
            state_t abst_state;
            abstract_state((abstractions)[i], &state, &abst_state);
            int *pdb_value = state_map_get((pdbs)[i], &abst_state);
            value += *pdb_value;
        }
    }
    else {
        for (unsigned int i=0; i<pdbs.size(); i++){
            state_t abst_state;
            abstract_state((abstractions)[i], &state, &abst_state);
            int *pdb_value = state_map_get((pdbs)[i], &abst_state);
            if(*pdb_value > value){
                value = *pdb_value;
            }
        }   
    }
    return value;
}

unsigned int astar(int additive_h, state_t root_state){
    state_t state, child;   // NOTE: "child" will be a predecessor of state, not a successor
    int ruleid;
    ruleid_iterator_t iter;
    map<uint64_t, short> colors;
    node init, current, child_node;

    PriorityQueue<node> open; // used for the states we have generated but not yet expanded (the OPEN list)
    state_map_t *map = new_state_map(); // contains the cost-from-init-to-goal

    // add root state
    init.state = root_state;
    init.g = 0;

    state_map_add(map, &(init.state), 0);
    colors.insert(pair<uint64_t,short>(hash_state(&init.state),0));
    open.Add(h(additive_h,init.state),0,init);

    while( !open.Empty() || difftime(end,start)<timeout )  {
        
        // remove top state from priority state
        current = open.Top();
        state = current.state;
        open.Pop();

        const int *known_dist = state_map_get(map, &state);
        assert(known_dist != NULL);
        if( *known_dist > current.g ){
            state_map_add(map, &state, current.g);
            if(is_goal(&state)){
                printf("SOLVED :D\n");
                return current.g;
            }

            init_fwd_iter(&iter, &state);
            while( (ruleid = next_ruleid(&iter) ) >= 0 ) {
                apply_fwd_rule(ruleid, &state, &child);

                // Se supone que if h(s) < INFNIY pero en los problemas a tratar
                // siempre se satisface

                colors.insert(pair<uint64_t,short>(hash_state(&child),0));
                const int child_g = current.g + get_fwd_rule_cost(ruleid);

                child_node.state = child;
                child_node.g = child_g;
                open.Add(child_g + h(additive_h, child), child_g, child_node);
                generatedCount++;
                
                // check if either this child has not been seen yet or if
                // there is a new cheaper way to get to this child.
                const int *old_child_g = state_map_get(map, &child);
                if( (old_child_g == NULL) ) { //|| (*old_child_g > child_g) ) {
                    // add to open with the new distance
                    state_map_add(map, &child, child_g);
                    //open.Add(child_d, child_d, child);
                }
            }
            expandedCount++;
        }
    }

    return UINT_MAX;
}

int main(int argc, char **argv) {
    state_t state;
    if( argc < 3 ) {
        printf("Usage: %s <timeout> <pdb> <additive_h>\n", argv[0]);
        exit(-1);
    }
    // Leer archivos
    char pdb_fname[1024], test_fname[1024], abst_fname[1024];
    timeout = atoi(argv[1]);          // Tiempo maximo permitido.

    int instancia=0, additive_h = atoi(argv[3]);
    double dif;
    ssize_t nchars;

    printf("%d\n",additive_h );

    char line[1024];
    std::ifstream infile(argv[2]);
    while (infile >> line){
        // Cargar PDBs y abstracciones.
        const char *pdb_name  = line;  // PDB a utilizar. (Debe ser una lista).
        strcpy(pdb_fname, pdb_name);      // Crear nombres con extensiones.
        strcat(pdb_fname, ".pdb");        
        strcpy(abst_fname, pdb_name);
        strcat(abst_fname, ".abst");
        FILE *pdb_file = fopen(pdb_fname, "r");          // Abrir y Leer PDB
        pdbs.push_back(read_state_map(pdb_file));        // Guardar PDB y ABS. 
        abstractions.push_back(read_abstraction_from_file(abst_fname));
        fclose(pdb_file);
    }

    // Leer linea por linea de la entrada estandar          
    char testCase[4098];
    while(fgets(testCase, sizeof(testCase), stdin))
    {
        FILE * pFile;
        pFile = fopen ("resultsAStar.txt","a");
        generatedCount = 0;
        expandedCount = 0;
        nchars = read_state(testCase, &state);  // Crear estado
        time (&start);
        time (&end);
        int result = astar(additive_h,state);
        time (&end);
        dif = difftime(end,start);
        printf("%d : %s : %lld %lld %lf %d %g \n",instancia, testCase ,expandedCount, generatedCount, dif, result, generatedCount/dif);
        fprintf(pFile, "%d : %s : %lld %lld %lf %d %g \n",instancia, testCase,expandedCount, generatedCount, dif, result, generatedCount/dif);
        fclose (pFile);
        instancia++;
    }
    
    return 0;
}

