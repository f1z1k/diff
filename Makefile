SRC = main.cpp edit_instruction.cpp string_vector.cpp argvector.cpp
HEADER = edit_instruction.h string_vector.h argvector.h
FLAGS = -g -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm

diff: $(SRC) $(HEADER) 
	g++ $(FLAGS) $(SRC) -o $@

clean:
	rm ./diff
