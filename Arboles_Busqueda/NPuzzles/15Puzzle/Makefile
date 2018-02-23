# to make a PDB starting from foo.psvn  type the command:     make pdb ss=foo
# if $(absname).txt exists it will be used as the input (abstraction commands) to build the abstraction,
#   otherwise the user will enter the abstraction commands interactively.
CC = gcc
CXX = g++
OPT = -g -Wall -O3 -Wno-unused-function -Wno-unused-variable
PSVNOPT = --state_map --abstraction --backwards_moves --history_len=1

psvn2c_core.c:
	cp ../../../psvn-for-ci5437/src/psvn2c_core.c ./psvn2c_core.c

psvn2c_state_map.c:
	cp ../../../psvn-for-ci5437/src/psvn2c_state_map.c ./psvn2c_state_map.c

psvn2c_abstraction.c:
	cp ../../../psvn-for-ci5437/src/psvn2c_abstraction.c ./psvn2c_abstraction.c

%.c: %.psvn psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c
	../../../psvn-for-ci5437/bin/psvn2c $(PSVNOPT) --name=$(*F) < $< > $@
	rm -f ./psvn2c_core.c ./psvn2c_state_map.c ./psvn2c_abstraction.c

.PRECIOUS: %.c

%.succ: %.c ../../../psvn-for-ci5437/global/succ.c
	$(CC) $(OPT) ../../../psvn-for-ci5437/global/succ.c -include $< -o $@
	rm -f $*.c

%.dist: %.c ../../../psvn-for-ci5437/global/dist.cpp
	$(CXX) $(OPT) ../../../psvn-for-ci5437/global/dist.cpp -include $< -o $@
	rm -f $*.c

%.distSummary: %.c ../../../psvn-for-ci5437/global/distSummary.cpp
	$(CXX) $(OPT) ../../../psvn-for-ci5437/global/distSummary.cpp -include $< -o $@
	rm -f $*.c

%.noPruneSearch: %.c ../../../psvn-for-ci5437/global/noPruneSearch.cpp
	$(CXX) $(OPT) ../../../psvn-for-ci5437/global/noPruneSearch.cpp -include $< -o $@
	rm -f $*.c

abstractor:
	$(CXX) $(OPT) ../../../psvn-for-ci5437/src/abstractor.cpp ../../../psvn-for-ci5437/src/psvn.cpp -o $@

generator: $(ss).c
	$(CXX) $(OPT) ../../../psvn-for-ci5437/global/generator.cc -include $< -o $@
	#rm -f ./generator.cc 

%.pdb: abstractor
	@rm -f `basename $*`.abst `basename $*`.pdb `basename $*`.psvn
	./abstractor `dirname $*`.psvn `basename $*` < `basename $*`.txt
	make `basename $*`.distSummary
	echo Calculating `basename $*.pdb` ...
	@./`basename $*`.distSummary `basename $*`.pdb

%.dist_pdb: %.c ../../../psvn-for-ci5437/global/dist_pdb.cpp
	$(CXX) $(OPT) ../../../psvn-for-ci5437/global/dist_pdb.cpp -include $< -o $@
	rm -f $*.c

%.breathFirstSearch: %.c ../../../psvn-for-ci5437/global/breathFirstSearch.cpp
	$(CXX) $(OPT) ../../../psvn-for-ci5437/global/breathFirstSearch.cpp -include $< -o $@
	rm -f $*.c

%.ida: %.c ../../../psvn-for-ci5437/global/ida.cpp
	$(CXX) $(OPT) ../../../psvn-for-ci5437/global/ida.cpp -include $< -o $@
	rm -f $*.c

%.noPruneExplore: %.c ../../../psvn-for-ci5437/global/noPruneExplore.cpp
	$(CXX) $(OPT) ../../../psvn-for-ci5437/global/noPruneExplore.cpp -include $< -o $@
	rm -f $*.c

%.parcialPruneExplore: %.c ../../../psvn-for-ci5437/global/parcialPruneExplore.cpp
	$(CXX) $(OPT) ../../../psvn-for-ci5437/global/parcialPruneExplore.cpp -include $< -o $@
	rm -f $*.c

.PHONY: clean
clean:
	rm -fr *.succ *.dist *.distSummary *.noPruneSearch *.dist_pdb *.breathFirstSearch *.ida *.parcialPruneExplore *noPruneExplore psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c *.dSYM *.o *~
	
