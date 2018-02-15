#include <vector>
#include "priority_queue.hpp"

int main(int argc, char **argv) {
	pair <int, char> PAIR1 ;
	state_t state, child;   	// NOTE: "child" will be a predecessor of state, not a successor
	ruleid_iterator_t iter;

	// read args
	if( argc < 2 ) {
	    printf("Usage: %s <pdb>\n", argv[0]);
	    exit(-1);
	}
	const char *pdb_name = argv[1];

	// Open PDBs
	char pdb_name[1024], abst_fname[1024];	// Nombres de los archivos.
	FILE *pdb_file = fopen(pdb_name, "r");	// Abrir archivo PDB.

	// Cargar cosas.
	len = read_state(string, &state);							  	// Cargar Estado Inicial
	abstraction_t *abst = read_abstraction_from_file(abst_fname);	// Cargar ABS
	state_map_t *pdb 	= read_state_map(pdb_file);					// Cargar PDB