#!/bin/bash
mkdir -p /GeneticAlgorithm/{build,output}
cd /GeneticAlgorithm/build && cmake .. && make
cd .. && ./build/ParallelGenetic $@