FROM archlinux/base

# Install dependencies
RUN pacman -Sy --noconfirm base-devel
RUN pacman -Sy --noconfirm cmake
RUN pacman -Sy --noconfirm opencv
RUN pacman -Sy --noconfirm libglvnd
RUN pacman -Sy --noconfirm openmpi
RUN pacman -Sy --noconfirm vtk
RUN pacman -Sy --noconfirm gtk3
RUN pacman -Sy --noconfirm hdf5
RUN pacman -Sy --noconfirm glew

# Copy source files
COPY . /GeneticAlgorithm