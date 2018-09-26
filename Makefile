OBJ=o
LIB=a
CXX=/usr/gcc-4.8/bin/g++

HAXCC=haxcc
# haxcc.OBJ must come LAST, must make cparsb.o and cparsl.o first
HAXCC_OBJ=cparsb.$(OBJ) cparsl.$(OBJ) haxcc.$(OBJ)

TARGETS=$(HAXCC)

$(HAXCC): $(HAXCC_OBJ)
	$(CXX) -o $@ $^

all: $(TARGETS)

clean:
	rm -vf cparsb.cpp cparsb.cpp.h cparsl.cpp cparsl.cpp.h
	rm -vf *.$(OBJ) *.$(LIB)
	rm -vf $(TARGETS)
	rm -vf *~

cparsl.cpp: cparsl.l
	flex --header-file="$@.h" -o $@ $^

cparsb.cpp: cparsb.y
	bison "--defines=$@.h" -o $@ $^

.cpp.o:
	$(CXX) -std=gnu++11 -DLINUX -DYYDEBUG=1 -DYYERROR_VERBOSE=1 -DYYTOKEN_TABLE=1 -Wall -pedantic -g3 -O0 -c -o $@ $<

tests: $(HAXCC)
	./$(HAXCC) <test1.c

vtests: $(HAXCC)
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./$(HAXCC) <test1.c

