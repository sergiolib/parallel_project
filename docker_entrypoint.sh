#!/bin/bash
mkdir -p /GeneticAlgorithm/build
cd /GeneticAlgorithm/build && cmake .. && make
cd .. && ./build/ParallelGenetic $@