#include <vector>
#include "priority_queue.hpp"
#include <utility>
#include <climits>
#include <sys/time.h>
#include <signal.h>
#include <fstream>
#include <string>
#include <ctime>
#include <unistd.h>
#include <vector>

state_t state, child;
int timeout;
ssize_t nchars;
std::vector<abstraction_t *> abstractions;    // Vector de abstracciones
std::vector<state_map_t *>   pdbs;            // Vector de pdbs
double dif;
// Variables de control
long long generatedCount, expandedCount;
int instancia = 0; // Numero de nodos generados
time_t start,end;  // Tiempo de inicio
int heur_type = 0; // 0 para MAX, otro para ADD.

unsigned int h_value()
{
    unsigned int value = 0;
    if(heur_type==0){
        for (unsigned int i=0; i<pdbs.size(); i++){
            state_t abst_state;
            abstract_state((abstractions)[i], &state, &abst_state);
            int *pdb_value = state_map_get((pdbs)[i], &abst_state);
            if(*pdb_value > value){
                value = *pdb_value;
            }
        }
        return value;
    }
    for (unsigned int i=0; i<pdbs.size(); i++){
        state_t abst_state;
        abstract_state((abstractions)[i], &state, &abst_state);
        int *pdb_value = state_map_get((pdbs)[i], &abst_state);
        value += *pdb_value;
    }
    return value;

}

std::pair<bool,unsigned int> dfs_ida(unsigned int bound,unsigned int g, int history)
{
    // Obtener valor del pdb (Heuristica)
    unsigned int t = UINT_MAX;
    unsigned int f = g + h_value(); // Costo con heuristica.
    unsigned long int cost;
    int ruleid;
    ruleid_iterator_t iter;
    expandedCount++;
    if(f > bound){
        return std::pair<unsigned int, unsigned int>(false, f);    // Si el f > h, no hay solucion en la rama
    } 
    if(h_value()==0){
        return std::pair<unsigned int, unsigned int>(true, g);     // Se encontro el goal.
    } 

    init_fwd_iter(&iter, &state);

    while( (ruleid = next_ruleid(&iter) ) >= 0 && difftime(end,start)<timeout) {
        if( !fwd_rule_valid_for_history(history, ruleid )){
            continue;
        }
        generatedCount++;                                          // Agregar cada hijo generado a la cuenta.
        int aux_history = next_fwd_history(history,ruleid);
        cost = g + get_fwd_rule_cost(ruleid);
        apply_fwd_rule(ruleid, &state, &child);   
        state = child;
        if(h_value() < UINT_MAX)
        {
            std::pair<bool, unsigned int> p = dfs_ida(bound, cost, aux_history);
            if(p.first) return p;
            t = std::min(t, p.second);
        }
        apply_bwd_rule(ruleid, &state, &child);
        aux_history = next_bwd_history(aux_history, ruleid);
        state = child;
        time (&end);
    }
    return std::pair<unsigned int, unsigned int>(false, t);
}

unsigned int idaStar(){
    unsigned int bound  = h_value();
    while(difftime(end,start) < timeout)
    {
        std::pair<bool, unsigned int> p = dfs_ida(bound, 0, init_history);
        if(p.first) return p.second;
        bound = p.second;
        time (&end);
    }
    return bound;
}

int main(int argc, char **argv) {
    // Leer argumentos
    if( argc < 2 ) {
        printf("Usage: %s <timeout> <pdb>\n", argv[0]);
        exit(-1);
    }
    // Leer archivos
    char pdb_fname[1024], test_fname[1024], abst_fname[1024];
    timeout = atoi(argv[1]);          // Tiempo maximo permitido.

    char line[1024];
    std::ifstream infile(argv[2]);
    if(argc == 4){
        heur_type = atoi(argv[3]);
    }
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
        pFile = fopen ("resultsIda.txt","a");
        generatedCount = 0;
        expandedCount = 0;
        nchars = read_state((const char *)testCase, &state);  // Crear estado
        time (&start);
        time (&end);
        int result = idaStar();
        time (&end);
        dif = difftime(end,start);
        printf("%d : %s : %lld %lld %lf %d %g \n",instancia, testCase ,expandedCount, generatedCount, dif, result, generatedCount/dif);
        fprintf(pFile, "%d : %s : %lld %lld %lf %d %g \n",instancia, testCase,expandedCount, generatedCount, dif, result, generatedCount/dif);
        fclose (pFile);
        instancia++;
    }

}


