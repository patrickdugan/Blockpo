CXXFLAGS=-DNDEBUG -O3 -Wall -Wno-unused-variable -lm -std=c++11
LDFLAGS=
OBJS_CODE=operators_algo_clearing.o globales.o

clearing_algo:  main_clearing_algo.o $(OBJS_CODE)
	$(CXX) $(LDFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm   *.o clearing_algo