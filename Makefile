
# hack for my sys
CXX=/usr/gcc-4.8/bin/g++

all: opcc

opcc: opcc.cpp
	$(CXX) -Wall -Wextra -pedantic -std=gnu++11 -o $@ $<

clean:
	rm -f opcc

