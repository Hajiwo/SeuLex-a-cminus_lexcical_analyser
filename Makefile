CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2

.PHONY: smoke
smoke:
	mkdir -p build output inter_File Automata_Info
	$(CXX) $(CXXFLAGS) main.cpp Read_Lex.cpp Method_RE.cpp Method_Automata.cpp Test.cpp -o build/generator
	./build/generator
	$(CXX) $(CXXFLAGS) output/lex.yy.cpp -o build/scanner
	python3 tests/smoke.py
