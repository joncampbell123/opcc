
# hack for my sys
CXX=/usr/gcc-4.8/bin/g++

all: opcc

opcc: opcc.cpp
	$(CXX) -Wall -Wextra -pedantic -std=gnu++11 -lm -o $@ $<

clean:
	rm -f opcc

progress-report:
	mkdir -p progress-report-lists
	for i in 8086 80186 286 386 486 pentium pentium-mmx pentium-pro pentium-pro-mmx pentium-2 amd-k6 cyrix-6x86-mmx everything; do \
		./opcc -i test -march "$$i" -lop >"progress-report-lists/$$i" || exit 1; \
	done
