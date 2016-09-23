CXX = g++
CXXFLAG = -std=c++14 -Wall -Wextra -Werror -Wpedantic -Wsign-conversion \
				  -Wold-style-cast -Wsign-promo -Wsign-promo -Wswitch-enum \
					-Weffc++ -Wshadow -Wno-missing-braces -Os

%.o: %.cc
	${CXX} ${CXXFLAG} -c $?

test: test.o Pentastate.o
	${CXX} ${CXXFLAG} -o test $?
	./test

asm: test.cc Pentastate.cc
	${CXX} ${CXXFLAG} -fverbose-asm -S -g $?

clean:
	rm -f *.o test *.s
