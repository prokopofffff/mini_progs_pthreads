OPTS = -O3 -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format -pthread
all: clear a.out
a.out: 1.3.o
	g++ $(OPTS) $^ -o a.out
1.3.o: 1.3.cpp
	g++ -c $(OPTS) $<
clear:
	rm -f *.o
clean:
	rm -f *.out *.o *.bak
