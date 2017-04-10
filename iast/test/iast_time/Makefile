all: libiast.a test.x test2.x

libiast.a:
	cd ../../ && make

test.x: test.cpp ../../libiast.a
	g++ test.cpp -o test.x -std=c++11 -I ../../ -L ../../ -l iast -O3

test2.x: test2.cpp ../../libiast.a
	g++ test2.cpp -o test2.x -std=c++11 -I ../../ -L ../../ -l iast -O3
