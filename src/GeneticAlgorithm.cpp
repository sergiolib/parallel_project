//
// Created by liberman and ayin on 4/5/19.
//

#include "GeneticAlgorithm.h"
#include "Individual.h"
#include "Polygon.h"
#include "utils.h"
#include "Population.h"
#include "conf.h"
#include "mpi.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <unistd.h>
#include <algorithm>

using namespace std;

GeneticAlgorithm::GeneticAlgorithm(unsigned char *pixels, int width, int height, int channels) {
    this->data = pixels;
    this->width = width;
    this->height = height;
    this->channels = channels;
    this->num_polygons = 1;
    this->indivs = 3;
    this->polys = initial_polys;
    this->vertices = 3;
    this->pop = new Population(3, initial_polys, 3, width, height);
}

void sortByFitness(vector<Individual *> indArr) {
    sort(indArr.begin(), indArr.end());
    reverse(indArr.begin(), indArr.end());
}

vector<Individual *> *GeneticAlgorithm::mate(Individual *ind1, Individual *ind2,
                                             int numberOfVertices, int maxX, int maxY) {
    double mutationRate = mutation_rate;
    double crossOverRate = cross_over_rate;
    auto individuals = new vector<Individual *>();

    auto polys1 = new list<Polygon *>();
    auto polys2 = new list<Polygon *>();
    double rand = utils::random();
    if (rand <= crossOverRate) {
        twoPointCrossover(ind1, ind2, polys1, polys2);
    } else {
        cloneParents(ind1, ind2, polys1, polys2);
    }

    auto offspring1 = new Individual(numberOfVertices, maxX, maxY, polys1);
    auto offspring2 = new Individual(numberOfVertices, maxX, maxY, polys2);

    rand = utils::random();
    if (rand <= mutationRate) {
        offspring1->mutate();
    }
    rand = utils::random();
    if (rand <= mutationRate) {
        offspring2->mutate();
    }
    individuals->push_back(offspring1);
    individuals->push_back(offspring2);

    return individuals;
}

void GeneticAlgorithm::twoPointCrossover(Individual *ind1, Individual *ind2,
                                         list<Polygon *> *off1, list<Polygon *> *off2) {

    Individual *par1 = ind1;
    Individual *par2 = ind2;

    Individual *fittest = ind1->fitness > ind2->fitness ? par1 : par2;

    int max = par1->get_len_dna() < par2->get_len_dna() ? par1->get_len_dna() : par2->get_len_dna();

    int r1 = utils::next_int(max - 1);
    int r2 = utils::next_int(max - 1);
    int i1 = std::min(r1, r2);
    int i2 = std::max(r1, r2);
    for (int i = 0; i < i1; i++) {
        off1->push_back(new Polygon(fittest->get_dna(i)));
        off2->push_back(new Polygon(fittest->get_dna(i)));
    }

    auto clones1 = vector<Polygon *>();
    auto clones2 = vector<Polygon *>();

    for (int i = i1; i <= i2; i++) {
        clones1.push_back(new Polygon(par2->get_dna(i)));
        clones2.push_back(new Polygon(par1->get_dna(i)));
    }
    int len = clones1.size();
    for (int i = 0; i < len; i++) {
        off1->push_back(new Polygon(clones1[i]));
        off2->push_back(new Polygon(clones2[i]));
    }

    for (int i = i2 + 1; i < fittest->get_len_dna(); i++) {
        off1->push_back(new Polygon(fittest->get_dna(i)));
        off2->push_back(new Polygon(fittest->get_dna(i)));
    }

    for (auto &p : clones1) {
        delete p;
    }
    for (auto &p : clones2) {
        delete p;
    }
}

void
GeneticAlgorithm::cloneParents(Individual *par1, Individual *par2, list<Polygon *> *polys1, list<Polygon *> *polys2) {
    int len = par1->get_len_dna();
    for (int i = 0; i < len; ++i) {
        polys1->push_back(new Polygon(par1->get_dna(i)));
    }
    len = par2->get_len_dna();
    for (int i = 0; i < len; ++i) {
        polys2->push_back(new Polygon(par2->get_dna(i)));
    }
}

/**
 * probabilistically select an individual from the population based on their
 * fitness. this uses rank selection. although could use
 * roulette(indArr, fitnessSum) for roulette wheel selection.
 */
Individual *GeneticAlgorithm::fps(vector<Individual *> indArr, double fitnessSum) {
    double nSum = 1.0;
    double r = utils::random();
    int n = indArr.size();
    int last = n - 1;
    for (int i = 0; i < last; i++) {
        nSum -= n / fitnessSum;
        n--;
        if (r >= nSum) {
            return indArr.at(i);
        }
    }
    return indArr.at(last);
}

Individual *GeneticAlgorithm::evolve(int max_epochs, bool use_mpi) {
    auto bytes = new unsigned char[this->width * this->height * this->channels];
    Individual *bestInd = nullptr;
    int j = 0;
    int P;
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    auto buf = new unsigned char[this->width * this->height / P * this->channels];
    int buf_ind[100000];

    for (int epoch = 0; epoch < max_epochs; ++epoch) {
        if (j != this->indivs) {
            vector<Individual *> individuals = this->pop->get_individuals();
            Individual *ind = individuals.at(j);
            double fitness;
            if (use_mpi) {
                ind->draw_CV_parallel(bytes, buf, buf_ind, width, height, channels);
                fitness = utils::diff_parallel(bytes, this->data, buf, width, height, channels);
            } else {
                ind->draw_CV(bytes, width, height, channels);
                fitness = utils::diff(bytes, this->data, width, height, channels);
            }

            if (fitness > this->pop->max) {
                bestInd = ind;
                this->pop->max = fitness;
                this->pop->elite = ind;
            }

            ind->fitness = fitness;
            j++;
        } else {
            if (bestInd != nullptr) {
                int polys_len = bestInd->get_len_dna();
                if (polys_len != this->num_polygons) {
                    this->num_polygons = polys_len;
                }
            }
            sortByFitness(this->pop->get_individuals());
            auto nextGeneration = vector<Individual *>();
            nextGeneration.push_back(this->pop->elite);
            for (int i = 1; i < j; i += 2) {
                Individual *parent1 = fps(this->pop->get_individuals(), this->pop->s);
                Individual *parent2 = fps(this->pop->get_individuals(), this->pop->s);
                auto offspring = mate(parent1, parent2,
                                      max_number_of_vertices, this->width, this->height);
                nextGeneration.push_back(offspring->at(0));
                nextGeneration.push_back(offspring->at(1));
            }
            this->pop->set_individuals(nextGeneration);
            j = 0;
        }
    }
    free(bytes);
    delete[] buf;
    bestInd = new Individual(bestInd);
    clean_population();
    cout << "Best fitness: " << bestInd->fitness << endl;
    return bestInd;
}

void GeneticAlgorithm::clean_population() {
    vector<Individual *> individuals = this->pop->get_individuals();
    for (auto &individual : individuals) {
        delete individual;
    }
}

void GeneticAlgorithm::check_equal(unsigned char *bytes1, unsigned char *bytes2, int i) {
    for (int n = 0; n < i; n++) {
        if (bytes1[n] != bytes2[n]) {
            cout << "They are different at " << n << "/" << i << "!" << endl;
            return;
        }
    }
    cout << "They are equal!" << endl;
}
