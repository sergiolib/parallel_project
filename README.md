# Parallel Programming project
Parallel implementation of a genetic algorithm for 
the reconstruction of an image using the minimal possible amount of polygons.

## Compilation

To compile, be sure to have the following dependencies:

- CMake >= 2.8
- OpenCV >= 3.0.0
- Either:
    + GNU C++ Compiler + OpenMPI
    + Intel C++ Compiler + Intel MPI
    
To compile, use the standard CMake commands.
In the root directory of the repository:
```bash
mkdir -p {build,output}
cd build && rm -rf *
cmake ..
make -j4 && cd ..
```

## Running

To run the algorithm, it can be done either with `mpirun` or without it. If only 1 process is specified, the program still runs, disabling any parallel code.

```bash
export P=<number of processes>
export INPUT_IMAGE=<path to input file>
export OUTPUT_IMAGE=<path to output file>
export EPOCHS=<expected number of epochs>
mpirun -np $P ./ParallelGenetic -e $EPOCHS -i $INPUT_IMAGE -o $OUTPUT_IMAGE 
```

For the program to work as expected, the `output` directory must exist.

## Credits
* Abgeiba Isunza (ayin@kth.se) 
* Sergio Liberman (liberman@kth.se) 