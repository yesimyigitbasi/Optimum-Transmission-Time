#include <iostream>
#include <ctime>
#include <random>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

#define N 100 // size of population
#define T 50 // max number of generations
#define PC 0.9 // probability of crossover
#define PM 0.2 // probability of mutation

// DS of Chromosome
struct chrom {
    double t; // transmission time
    double fitness; // fitness of the individual
};

// compare chromosomes considering their fitness
struct compareChroms {
    inline bool operator() (const chrom& obj1, const chrom& obj2) {
        return (obj1.fitness < obj2.fitness);
    }
};

// prototypes
chrom initializeRandIndividual();
void initializeRandPop(vector<chrom> &);
double penaltyFunction(double t, double a);
void evaluate(vector<chrom> &, float);
vector<chrom> selection(vector<chrom> &);
void crossover(chrom &, chrom &, vector<chrom> &);
void mutation(chrom &);
void variation(vector<chrom> &);
void survival_elitist(vector<chrom> &, vector<chrom> &);

// Initialize a random value for transmission time to each chromosome
// Time Complexity: O(1)
// Space Complexity: O(1)
chrom initializeRandIndividual() {
    chrom ind;
    ind.t = (rand() % 1000) / 1000.0; // Random value between 0 and 1
    return ind;
}

// Initialize the population of individuals. Each individual represents the transmission time
// Time Complexity: O(N) 
// Space Complexity: O(N) -> O(1) for the vector, O(N) for the individuals
void initializeRandPop(vector<chrom> &pop) {
    for (int i = 0; i < N; i++)
        pop.push_back(initializeRandIndividual());
}

// Penalty function (this is the original penalty function from the paper)
// Time Complexity: O(1)
// Space Complexity: O(1)
double penaltyFunction(double t, double a) { // t is the transmission time, a is mode of the pdf
    if (t <= a)
        return (-(2*pow(t,4)) + 8*pow(t,3) - 6*pow(t,2) + pow(a,3) - 3*pow(a,2) + 3*a)/(12*a);
    else
        return(-(2*pow(t,4)) + 8*pow(t,3) + 6*a*pow(t,2) - 18*pow(t,2) - 4*pow(a,2)*t + 12*t + pow(a,3) -3)/(12*(a-1));
}

// Evaluate the fitness of each chromosome
// Time Complexity: O(N)
// Space Complexity: O(1) -> pop is passed by reference
void evaluate(vector<chrom> &pop, float a) {
    for (int i = 0; i < N; i++)
        pop[i].fitness = penaltyFunction(pop[i].t, a);
}

// Binary tournament selection operator
// Time Complexity: O(N) 
// Space Complexity: O(N) -> pop is passed by reference O(1), matingPool is O(N)
vector<chrom> selection(vector<chrom> &pop) {
    vector<chrom> matingPool;
    for (int i = 0; i < N; i++) { // for each individual in the population
        // choosing two random individuals as parents
        unsigned int i1 = rand() % N;
        unsigned int i2 = rand() % N;
        // pushing the individual with smallest fitness into the mating pool
        if (pop[i1].fitness < pop[i2].fitness) 
            matingPool.push_back(pop[i1]);
        else
            matingPool.push_back(pop[i2]);
    }
    return matingPool;
}

// Linear crossover operator
// Time Complexity: O(1) 
// Space Complexity: O(1) 
void crossover(chrom &ind1, chrom &ind2, vector<chrom> &off) {
    float r = (rand() % 100) / 100.0f; // generate a random probablity between 0 and 0.99
    if (r <= PC) {
        // crossover between the pair and produce three offspring
        double y1 = 0.5 * (ind1.t + ind2.t);
        double y2 = 1.5 * ind1.t - 0.5 * ind2.t;
        double y3 = -0.5 * ind1.t + 1.5 * ind2.t;

        chrom ind3;
        ind3.fitness = 0.0; // fitness is not yet calculated so just give it a dummy value
    
        // assign the new values to the offspring, and bound check
        ind1.t = max(min(y1, 1.0), 0.0);
        ind2.t = max(min(y2, 1.0), 0.0);
        ind3.t = max(min(y3, 1.0), 0.0);
        
        // Add the third offspring to the population as well
        off.push_back(ind3);
    }
}

// (modified) random mutation operator
// Time Complexity: O(1)
// Space Complexity: O(1)
void mutation(chrom &ind) {
    float r = (rand() % 100) / 100.0f; // generate a random probablity between 0 and 0.99
    int delta = 8;
    if (r <= PM) {
        float y = ind.t + (r-0.5)*delta;
        ind.t = max(min(y, 1.0f), 0.0f); // bound check
    }
}

// Variation = crossover + mutation
// Time Complexity: O(N) -> N/2 pairs of individuals
// Space Complexity: O(1) -> off is passed by reference
void variation(vector<chrom> &off) {
    for (int i = 0; i < N; i += 2) {
        crossover(off[i], off[i + 1],off); // crossover between the pair
        mutation(off[i]); // mutation on the first individual
        mutation(off[i + 1]); // mutation on the second individual
    }
}

// Elitist survival strategy
// Time Complexity: O(NlogN) -> sort() takes O(NlogN) time
// Space Complexity: O(1) -> pop is passed by reference
void survival_elitist(vector<chrom> &pop, vector<chrom> &off) {
    for (int i = 0; i < N; i++)
        pop.push_back(off[i]); // add the offspring to the population
    sort(pop.begin(), pop.end(), compareChroms()); // sort the population based on fitness
    while (pop.size() > N)
        pop.pop_back(); // remove the worst individuals
}
