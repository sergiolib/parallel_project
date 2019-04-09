//
// Created by sliberman on 4/5/19.
//

#include "GeneticAlgorithm.h"
#include "Individual.h"
#include "Polygon.h"
#include "utils.h"
#include "Population.h"
#include "conf.h"
#include <vector>

GeneticAlgorithm::GeneticAlgorithm(unsigned char *pixels, int width, int height) {
    this->pop = new Population(3, conf::initial_polys, 3, width, height);
    this->data = pixels;
    this->width = width;
    this->height = height;
}

void GeneticAlgorithm::evolve(int max_epochs) {
    auto *res = new unsigned char[this->width * this->height];
    for (int epoch = 0; epoch < max_epochs; ++epoch) {
        res = {0};

        vector<Individual *> individuals = this->pop->get_individuals();
        Individual *ind = individuals.back();
        ind->draw(res, width, height);

        unsigned char *bytes = utils::draw_individuals(this->pop->get_individuals());
        double fitness = utils::diff(bytes, this->data, width, height);
    }
}

vector<Individual> GeneticAlgorithm::mate(Individual *ind1, Individual *ind2, int numberOfPolygons,
            int numberOfVertices, int maxX,
        int maxY){
    double mutationRate = conf::mutation_rate;
    double crossOverRate = conf::cross_over_rate;
    vector<Individual> individuals;

    list<Polygon *> polys1;
    list<Polygon *> polys2;
    double rand = utils::random(); //random number between [0,1)
    if(rand <= crossOverRate){
        twoPointCrossover( ind1, ind2, polys1, polys2);}
    else{
        cloneParents( ind1, ind2, polys1, polys2);}

    Individual *offspring1 = new Individual(numberOfPolygons, numberOfVertices, maxX, maxY, &polys1);
    Individual *offspring2 = new Individual(numberOfPolygons, numberOfVertices, maxX, maxY, &polys2);

    rand = utils::random();
    if(rand <= mutationRate) {
        offspring1->mutate();
    }
    rand = utils::random();
    if(rand <= mutationRate)  {
        offspring2->mutate();
    }
    individuals.push_back(*offspring1);
    individuals.push_back(*offspring2);

    return individuals;
}

void GeneticAlgorithm::twoPointCrossover(Individual *ind1,Individual *ind2, list<Polygon *> off1, list<Polygon *> off2) {

    Individual par1 = *ind1; //here it was ind1.dna
    Individual par2 = *ind2; //here it was ind2.dna

    Individual fittest = ind1->fitness > ind2->fitness ? par1 : par2;

    int max = par1.get_len_dna() < par2.get_len_dna() ? par1.get_len_dna() : par2.get_len_dna();

    int r1 = utils::next_int(max-1);
    int r2 = utils::next_int(max-1);
    int i1 = std::min(r1, r2);
    int i2 = std::max(r1, r2);
    int i = 0;
    for( i = 0; i < i1; i++) {
        off1.push_back(clonePoly(fittest.get_dna(i)));
        off2.push_back(clonePoly(fittest.get_dna(i)));
    }

    vector<Polygon*> clones1;
    vector<Polygon*> clones2;

    for( i = i1; i <= i2; i++) {
        clones1.push_back(clonePoly(par2.get_dna(i)));
        clones2.push_back(clonePoly(par1.get_dna(i)));
    }
    int len = clones1.size();
    for(i = 0; i < len; i++) {
        off1.push_back(clones1[i]);
        off2.push_back(clones2[i]);
    }

    for(i = i2 + 1; i < fittest.get_len_dna(); i++) {
        off1.push_back(clonePoly(fittest.get_dna(i)));
        off2.push_back(clonePoly(fittest.get_dna(i)));
    }
}