all: LinkAnalysis.h TraceAnalyzer.cpp misc.h
	g++ *.cpp -Wall -O0 -o TraceAnalyzer -g -v -da -Q
