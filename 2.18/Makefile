OPTS = -O3 -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format -pthread
all: clear a.out
a.out: 2.18.o funcs.o
	g++ $(OPTS) $^ -o a.out
2.18.o: 2.18.cpp funcs.h
	g++ -c $(OPTS) $<
funcs.o: funcs.cpp funcs.h
	g++ -c $(OPTS) $<
clear:
	rm -f *.o
clean:
	rm -f *.out *.o *.bak
