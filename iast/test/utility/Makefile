CXXFLAGS = -std=c++1z -O3 -fconcepts

all: libiast.a test.x fitiso alliso

libiast.a:
	cd ../../ && make

test.x: test.cpp ../../libiast.a
	g++ test.cpp -o test.x $(CXXFLAGS) -L ../../ -l iast

fitiso: fitiso.cpp ../../libiast.a
	g++ fitiso.cpp -o $(@) $(CXXFLAGS) -L ../../ -l iast -I ../../

alliso: alliso.cpp ../../libiast.a
	g++ alliso.cpp -o $(@) $(CXXFLAGS) -L ../../ -l iast -I ../../
