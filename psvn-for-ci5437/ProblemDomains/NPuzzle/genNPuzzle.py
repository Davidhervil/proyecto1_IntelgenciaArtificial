import argparse
import sys

def gen_empty_HR(N):
	return ["-"]*(N*N)

def write_rule(file,LHR,RHR):
	file.write(" ".join(LHR+["=>"]+RHR)+'\n')

def gen_rules_for_position(N,file,position):
	row = position // N
	col = position %  N
	if row < N - 1:
		LHR = gen_empty_HR(N)
		RHR = gen_empty_HR(N)
		LHR[position] = "0"
		move_pos = (row+1)*N + col
		LHR[move_pos] = "X"
		RHR[position] = "X"
		RHR[move_pos] = "0"
		write_rule(file, LHR, RHR)

	if row > 0:
		LHR = gen_empty_HR(N)
		RHR = gen_empty_HR(N)
		LHR[position] = "0"
		move_pos = (row-1)*N + col
		LHR[move_pos] = "X"
		RHR[position] = "X"
		RHR[move_pos] = "0"
		write_rule(file, LHR, RHR)

	if col < N - 1:
		LHR = gen_empty_HR(N)
		RHR = gen_empty_HR(N)
		LHR[position] = "0"
		move_pos = (row)*N + col + 1
		LHR[move_pos] = "X"
		RHR[position] = "X"
		RHR[move_pos] = "0"
		write_rule(file, LHR, RHR)

	if col > 0 :
		LHR = gen_empty_HR(N)
		RHR = gen_empty_HR(N)
		LHR[position] = "0"
		move_pos = (row)*N + col - 1
		LHR[move_pos] = "X"
		RHR[position] = "X"
		RHR[move_pos] = "0"
		write_rule(file, LHR, RHR)

if __name__ == '__main__':
	parser = argparse.ArgumentParser(description="Generador de PSVN para N-Puzzle")
	parser.add_argument('dimension', help='Dimension de los lados del puzzle', type=int)

	parser = parser.parse_args()

	N = parser.dimension

	first_comment = "# This file was generated by " \
					+ sys.argv[0] + ".py\n"
	state_vector_length_comment = "# Number of state variables (" \
								+str(N)+"*"+str(N)+"-1 pieces + blank space)\n"
	state_vector_length = N*N
	state_vector_descripton = "# All state variables have " \
								+ str(state_vector_length-1) \
								+ " possible values + blank which is 0\n"
	
	aux = " "+str(state_vector_length)
	state_vector = str(state_vector_length)+aux*(state_vector_length-1)

	psvn_file = open(str(N*N-1)+"puzzle.psvn",'w')
	psvn_file.write(first_comment+"\n")
	psvn_file.write(state_vector_length_comment)
	psvn_file.write(str(state_vector_length)+"\n\n")
	psvn_file.write(state_vector_descripton)
	psvn_file.write(state_vector+"\n\n")

	for i in range(state_vector_length):
		gen_rules_for_position(N,psvn_file,i)
	
	goal = []
	for i in range(state_vector_length):
		goal += [str(i)]
	goal = " ".join(goal)
	psvn_file.write("\nGOAL "+goal+"\n")


''' 4*2 + (N-2)*3*4 + (N-2)*(N-2)*4 
0 # - - | # - - - | - - - - | - - - - 

# 0 # - | - # - - | - - - - | - - - -

- # 0 # | - - # - | - - - - | - - - -

- - # 0 | - - - # | - - - - | - - - -

# - - - | 0 # - - | # - - - | - - - -

- # - - | # 0 # - | - # - - | - - - -

- - # - | - # 0 # | - - # - | - - - -

- - - # | - - # 0 | - - - # | - - - -

- - - - | - - - - | 0 - - - | - - - -

- - - - | - - - - | - 0 - - | - - - -

- - - - | - - - - | - - 0 - | - - - -

- - - - | - - - - | - - - 0 | - - - -

- - - - | - - - - | - - - - | - - - -

- - - - | - - - - | - - - - | - - - -

- - - - | - - - - | - - - - | - - - -

- - - - | - - - - | - - - - | - - - -

'''

