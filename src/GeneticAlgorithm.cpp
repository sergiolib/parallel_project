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
#include <algorithm>
using namespace std;

GeneticAlgorithm::GeneticAlgorithm(unsigned char *pixels, int width, int height) {
    this->pop = new Population(3, conf::initial_polys, 3, width, height);
    this->data = pixels;
    this->width = width;
    this->height = height;
    this->numPolygons = 1;

}

void sortByFitness(vector<Individual *> indArr) {
    sort(indArr.begin(),indArr.end());
}

vector<Individual *> GeneticAlgorithm::mate(Individual *ind1, Individual *ind2, int numberOfPolygons,
        int numberOfVertices, int maxX, int maxY){
    double mutationRate = conf::mutation_rate;
    double crossOverRate = conf::cross_over_rate;
    vector<Individual*> individuals;

    list<Polygon *> polys1;
    list<Polygon *> polys2;
    double rand = utils::random(); //random number between [0,1)
    if(rand <= crossOverRate){
        twoPointCrossover(ind1, ind2, polys1, polys2);
    } else{
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
    individuals.push_back(offspring1);
    individuals.push_back(offspring2);

    return individuals;
}

void GeneticAlgorithm::twoPointCrossover(Individual *ind1, Individual *ind2,
        list<Polygon *> off1, list<Polygon *> off2) {

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
/**
 * probabilistically select an individual from the population based on their
 * fitness. this uses rank selection. although could use
 * roulette(indArr, fitnessSum) for roulette wheel selection.
 */
Individual * GeneticAlgorithm::fps(vector<Individual *> indArr, double fitnessSum) {
    double nSum = 1.0;
    double r = random();
    int n = indArr.size();
    int last = n-1;
    int i=0;
    for( i = 0; i < last; i++) {
        nSum -= n / fitnessSum;
        n--;
        if(r >= nSum)
            return indArr.at(i);
    }
    return indArr.at(last);
}

void GeneticAlgorithm::evolve(int max_epochs) {
    auto *bytes = new unsigned char[this->width * this->height * 4];
    Individual *bestInd = nullptr;
    int j = 0;
    for (int epoch = 0; epoch < max_epochs; ++epoch) {
        if (j != this->indivs) {
            bytes = {0};
            vector<Individual *> individuals = this->pop->get_individuals();
            Individual *ind = individuals.back();
            ind->draw(bytes, width, height);

            double fitness = utils::diff(bytes, this->data, width, height);
            if (fitness > this->pop->max) {
                bestInd = ind;
                this->pop->max = fitness;
                this->pop->elite = ind; //I think this is not used so I commented it
            }
            j++;
        }
        else{
            if(bestInd != nullptr) {
                //unsigned char *bestPixels = utils::draw_individuals(this->pop->get_individuals());
                //
                int polys_len = bestInd->get_len_dna();
                if(polys_len != this->numPolygons) {
                    this->numPolygons = polys_len;
                }
            }
            sortByFitness(this->pop->get_individuals());
            vector<Individual *> nextGeneration;
            nextGeneration.push_back(this->pop->elite);
            int i ;
            for( i = 1; i < j; i += 2) {
                Individual *parent1 = fps(this->pop->get_individuals(), this->pop->s);
                Individual *parent2 = fps(this->pop->get_individuals(), this->pop->s);
                vector<Individual*> offspring = mate(parent1, parent2, this->numPolygons,
                                                    this->vertices, this->width, this->height);
                nextGeneration.push_back(offspring.at(0));
                nextGeneration.push_back(offspring.at(1));
            }
            this->pop->set_individuals(nextGeneration);
        }

    }
}