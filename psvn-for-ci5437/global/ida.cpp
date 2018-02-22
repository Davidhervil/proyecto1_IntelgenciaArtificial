#include <vector>
#include "priority_queue.hpp"
#include <utility>
#include <climits>
#define  MAX_LINE_LENGTH 999 

abstraction_t *abst;
state_map_t *pdb;
state_t state, child;
int d, ruleid;
ssize_t nchars; 
char str[MAX_LINE_LENGTH + 1];
ruleid_iterator_t iter;
int history = init_history;



unsigned int h_value(){
    state_t abst_state;
    abstract_state(abst, &state, &abst_state);
    const int *pdb_value = state_map_get(pdb, &abst_state);
    assert(pdb_value != 0);
    return *pdb_value;
}

std::pair<bool,unsigned int> dfs_ida(unsigned int bound,unsigned int g)
{
    // Obtener valor del pdb (Heuristica)
    unsigned int h = h_value();
    unsigned int f = g + h; // Costo con heuristica.
    //printf("holas");
    if(f>bound) return std::pair<unsigned int, unsigned int>(false, f);
    if(h==0) return std::pair<unsigned int, unsigned int>(true, g);

    unsigned int t = UINT_MAX;
    init_fwd_iter(&iter, &state);
    while( (ruleid = next_ruleid(&iter) ) >= 0 ) {
        //printf("holiwis");
        apply_fwd_rule(ruleid, &state, &child);
        const int cost = g + get_fwd_rule_cost(ruleid);
        state = child;
        if(h_value() < t)
        {
            history = next_fwd_history(int history, int ruleid);
            std::pair<bool, unsigned int> p = dfs_ida(bound, cost);
            if(p.first) return p;
            t = std::min(t, p.second);
        }
        history = next_bwd_history(int history, int ruleid);
        apply_bwd_rule(ruleid, &state, &child);
        state = child;
    }
    return std::pair<unsigned int, unsigned int>(false, t);
}



int main(int argc, char **argv) {
    // read args
    if( argc < 2 ) {
        printf("Usage: %s <pdb>\n", argv[0]);
        exit(-1);
    }
    const char *pdb_name = argv[1];
    // READ A LINE OF INPUT FROM stdin
    printf("Please enter a state followed by ENTER: ");
    if( fgets(str, sizeof str, stdin) == NULL ) {
        printf("Error: empty input line.\n");
        return 0; 
    }
    // Preprocesamiento de strings.
    char pdb_fname[1024], abst_fname[1024];
    strcpy(pdb_fname, pdb_name);
    strcat(pdb_fname, ".pdb");
    strcpy(abst_fname, pdb_name);
    strcat(abst_fname, ".abst");
    nchars = read_state(str,&state);
    printf("PDB: abst=%s", abst_fname);

    // Abrir archivos
    abst = read_abstraction_from_file(abst_fname);   // Leer ABS
    FILE *pdb_file = fopen(pdb_fname, "r");          // Abrir y Leer PDB
    pdb = read_state_map(pdb_file);
    fclose(pdb_file);

    int bound = h_value();
    while(true){
        std::pair<bool, unsigned int> p = dfs_ida(bound, 0);
        if(p.first); return 0;
        bound = p.second;
    }
}


