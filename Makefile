CXXFLAGS=-DNDEBUG -O3 -Wall -Wno-unused-variable -lm -std=c++11
LDFLAGS=
OBJS_CODE=bellmanford.o

test:  main_bellmanford.o $(OBJS_CODE)
	$(CXX) $(LDFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm   *.o test 
