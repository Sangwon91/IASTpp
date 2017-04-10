all: libiast.a test.x

libiast.a:
	cd ../../ && make

test.x: test.cpp ../../libiast.a
	g++ test.cpp -o test.x -std=c++11 -L ../../ -l iast
