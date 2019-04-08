FROM archlinux/base

# Install dependencies
RUN pacman -Sy --noconfirm base-devel cmake opencv libglvnd openmpi vtk gtk3 hdf5 glew

COPY . /GeneticAlgorithm
RUN mkdir -p /GeneticAlgorithm/build
RUN cd /GeneticAlgorithm/build && cmake .. && make
CMD mpirun --allow-run-as-root -np 4 /GeneticAlgorithm/GeneticAlgorithm
