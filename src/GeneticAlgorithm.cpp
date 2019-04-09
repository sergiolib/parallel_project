//
// Created by sliberman on 4/5/19.
//

#include "GeneticAlgorithm.h"
#include "Individual.h"
#include "Polygon.h"
#include "utils.h"
#include "Population.h"
#include <vector>

GeneticAlgorithm::GeneticAlgorithm() {
    this->pop = new Population();
    this->crossOverRate = 0; // probability of crossover
    this-> mutationRate= 1; // probability of a mutation
    this->maxNumberOfPolygons= 500;
    this->maxNumberOfVertices = 6;
    this->initialPolys = 50;
    }

vector<Individual> GeneticAlgorithm::mate(Individual ind1, Individual ind2, int numberOfPolygons,
            int numberOfVertices, int maxX,
        int maxY){
    int mutationRate  = this->mutationRate;
    int crossOverRate = this->crossOverRate;
    vector<Individual> individuals;

    vector<Polygon> polys1;
    vector<Polygon> polys2;
    double rand = ((double)arc4random() / ARC4RANDOM_MAX); //random number between [0,1)
    if(rand <= crossOverRate){
        twoPointCrossover( ind1, ind2, polys1, polys2);}
    else{
        cloneParents( ind1.dna, ind2.dna, polys1, polys2);}

    Individual *offspring1 = new Individual(numberOfPolygons, numberOfVertices, maxX, maxY, polys1);
    Individual *offspring2 = new Individual(numberOfPolygons, numberOfVertices, maxX, maxY, polys2);

    rand = ((double)arc4random() / ARC4RANDOM_MAX);
    if(rand <= mutationRate) {
        offspring1.mutate();
    }
    rand = ((double)arc4random() / ARC4RANDOM_MAX);
    if(rand <= mutationRate)  {
        offspring2.mutate();
    }
    individuals.push_back(*offspring1);
    individuals.push_back(*offspring2);

    return individuals;
}

void GeneticAlgorithm::twoPointCrossover(Individual ind1,Individual ind2, vector<Polygon *> *off1, vector<Polygon *> *off2) {

    var par1 = ind1.dna;
    var par2 = ind2.dna;

    var fittest = ind1.fitness > ind2.fitness ? par1 : par2;

    var max = par1.length < par2.length ? par1.length : par2.length;

    var r1 = nextInt(max-1);
    var r2 = nextInt(max-1);
    var i1 = Math.min(r1, r2);
    var i2 = Math.max(r1, r2);

    for(var i = 0; i < i1; i++) {
        off1.push(clonePoly(fittest[i]));
        off2.push(clonePoly(fittest[i]));
    }

    var clones1 = [];
    var clones2 = [];

    for(var i = i1; i <= i2; i++) {
        clones1.push(clonePoly(par2[i]));
        clones2.push(clonePoly(par1[i]));
    }

    for(var i = 0, len = clones1.length; i < len; i++) {
        off1.push(clones1[i]);
        off2.push(clones2[i]);
    }

    for(var i = i2 + 1; i < fittest.length; i++) {
        off1.push(clonePoly(fittest[i]));
        off2.push(clonePoly(fittest[i]));
    }
}