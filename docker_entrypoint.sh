#!/bin/bash
mkdir -p /GeneticAlgorithm/{build,output}
cd /GeneticAlgorithm/build && cmake -DCMAKE_CXX_FLAGS=-O3 -DCMAKE_EXE_LINKER_FLAGS=-O3 -DCMAKE_SHARED_LINKER_FLAGS=-O3 .. && make
cd .. && ./build/ParallelGenetic $@