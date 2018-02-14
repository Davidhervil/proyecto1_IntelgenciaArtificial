CC = gcc
CXX = g++
OPT = -g -Wall -O3 -Wno-unused-function -Wno-unused-variable
PSVNOPT = --state_map --no_abstraction --backwards_moves --history_len=0

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

.PHONY: clean
clean:
	rm -fr *.succ *.dist *.distSummary *.noPruneSearch psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c *.dSYM *.o *~
