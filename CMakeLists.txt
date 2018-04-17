cmake_minimum_required(VERSION 3.5)
project(final_project_hassang2)

set(CMAKE_CXX_STANDARD 11)

include_directories(neat.1.1/finaldir)
include_directories(NEAT.1.2.1)

add_executable(final_project_hassang2
        NEAT.1.2.1/experiments.cpp
        NEAT.1.2.1/experiments.h
        NEAT.1.2.1/gene.cpp
        NEAT.1.2.1/gene.h
        NEAT.1.2.1/genome.cpp
        NEAT.1.2.1/genome.h
        NEAT.1.2.1/innovation.cpp
        NEAT.1.2.1/innovation.h
        NEAT.1.2.1/link.cpp
        NEAT.1.2.1/link.h
        NEAT.1.2.1/neat.cpp
        NEAT.1.2.1/neat.h
        NEAT.1.2.1/neatmain.cpp
        NEAT.1.2.1/neatmain.h
        NEAT.1.2.1/network.cpp
        NEAT.1.2.1/network.h
        NEAT.1.2.1/nnode.cpp
        NEAT.1.2.1/nnode.h
        NEAT.1.2.1/organism.cpp
        NEAT.1.2.1/organism.h
        NEAT.1.2.1/population.cpp
        NEAT.1.2.1/population.h
        NEAT.1.2.1/species.cpp
        NEAT.1.2.1/species.h
        NEAT.1.2.1/trait.cpp)

add_executable(main src/main.cpp)
