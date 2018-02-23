#include <vector>
#include "priority_queue.hpp"
#include <utility>
#include <climits>
#include <sys/time.h>
#include <signal.h>
#include <fstream>
#include <string>
#include <ctime>

abstraction_t *abst;
state_map_t *pdb;
state_t state, child;
int d, timeout;
ssize_t nchars; 

// Variables de control
int generatedCount = 0, instancia = 0; // Numero de nodos generados
time_t start,end;       // Tiempo de inicio


unsigned int h_value()
{
    state_t abst_state;
    abstract_state(abst, &state, &abst_state);
    int *pdb_value = state_map_get(pdb, &abst_state);
    return *pdb_value;
}

std::pair<bool,unsigned int> dfs_ida(unsigned int bound,unsigned int g, int history)
{
    // Obtener valor del pdb (Heuristica)
    unsigned int t = UINT_MAX;
    unsigned int f = g + h_value(); // Costo con heuristica.
    unsigned long int cost;
    int ruleid;
    ruleid_iterator_t iter;
    generatedCount++;                                                           // Agregar cada hijo generado a la cuenta.
    if(f>bound) return std::pair<unsigned int, unsigned int>(false, f);         // Si el f > h, no hay solucion en la rama
    if(h_value()==0) return std::pair<unsigned int, unsigned int>(true, g);     // Se encontro el goal.

    init_fwd_iter(&iter, &state);
    while( (ruleid = next_ruleid(&iter) ) >= 0) {
        printf("hola\n");
        if( !fwd_rule_valid_for_history(history, ruleid )){
            continue;
        }
        int aux_history = next_fwd_history(history,ruleid);
        cost = g + get_fwd_rule_cost(ruleid);
        apply_fwd_rule(ruleid, &state, &child);   
        state = child;
        if(h_value() < UINT_MAX)
        {
            printf("Aqui\n");
            std::pair<bool, unsigned int> p = dfs_ida(bound, cost, aux_history);
            if(p.first) return p;
            t = std::min(t, p.second);
        }
        apply_bwd_rule(ruleid, &state, &child);
        //history = next_bwd_history(aux_history, ruleid);
        state = child;
    }
    return std::pair<unsigned int, unsigned int>(false, t);
}

int idaStar(){
    while(true)
    {
        unsigned int bound  = h_value();
        std::pair<bool, unsigned int> p = dfs_ida(bound, 0, init_history);
        if(p.first) return 0;
            bound = p.second;
    }
    return 1;
}

int main(int argc, char **argv) {
    // Leer argumentos
    if( argc < 2 ) {
        printf("Usage: %s <timeout> <test> <pdb>\n", argv[0]);
        exit(-1);
    }
    // Leer archivos
    char pdb_fname[1024], test_fname[1024], abst_fname[1024];
    timeout = atoi(argv[1]);          // Tiempo maximo permitido.
    const char *pdb_name  = argv[2];  // PDB a utilizar. (Debe ser una lista).

    strcpy(pdb_fname, pdb_name);      // Crear nombres con extensiones.
    strcat(pdb_fname, ".pdb");        
    strcpy(abst_fname, pdb_name);
    strcat(abst_fname, ".abst");

    // Cargar PDBs y abstracciones.
    abst = read_abstraction_from_file(abst_fname);   // Leer ABS
    FILE *pdb_file = fopen(pdb_fname, "r");          // Abrir y Leer PDB
    pdb = read_state_map(pdb_file);
    fclose(pdb_file);

    printf("MEMES\n");
    // Leer linea por linea de la libreria estandar          
    char testCase[4098];
    while(fgets(testCase, sizeof(testCase), stdin))
    {
        generatedCount = 0;
        nchars = read_state((const char *)testCase, &state);  // Crear estado
        int result = idaStar();
        printf("%d : %s : %d Result %d \n",instancia,testCase,generatedCount,result);
        instancia++;
    }
}


