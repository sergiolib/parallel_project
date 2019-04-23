#!/bin/bash
mkdir -p /GeneticAlgorithm/{build,output}
cd /GeneticAlgorithm/build && cmake -DCMAKE_CXX_FLAGS=-pg -DCMAKE_EXE_LINKER_FLAGS=-pg -DCMAKE_SHARED_LINKER_FLAGS=-pg .. && make
cd .. && ./build/ParallelGenetic $@
gprof ./build/ParallelGenetic ./gmon.out > ./profiling_analysis.txt