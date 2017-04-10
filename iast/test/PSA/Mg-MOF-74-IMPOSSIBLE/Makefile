INCPATH=~/libraries/iast2/
all: libiast.a test.x test2.x

libiast.a:
	cd ${INCPATH} && make

test.x: test.cpp ${INCPATH}libiast.a
	g++ test.cpp -o test.x -std=c++11 -I ${INCPATH} -L ${INCPATH} -l iast -O3

test2.x: test2.cpp ${INCPATH}libiast.a
	g++ test2.cpp -o test2.x -std=c++11 -I ${INCPATH} -L ${INCPATH} -l iast -O3

clean:
	rm test.x test2.x
