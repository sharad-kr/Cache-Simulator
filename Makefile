all: cache_simulate 

cache_simulate: a3.cpp
	g++ a3.cpp -o cache_simulate

clean:
	rm -f cache_simulate
