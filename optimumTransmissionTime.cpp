#include <iostream>
#include "optimumTransmissionTime.h"

using namespace std;

int main() {
    float a;
    bool valid = false;
    while (!valid) {
        cout << "Enter the a value in the range [0,1]: ";
        cin >> a; // as the user to enter a value for a, which is the mode of the pdf
        if (a >= 0 && a <= 1)
            valid = true;
        else
            cerr << "Invalid input. ";
    }

    srand(time(NULL));
    vector<chrom> pop; // population of chromosomes
    initializeRandPop(pop); // initialize the population
    evaluate(pop, a); // evaluate the fitness of each chromosome
    for (int i = 0; i < T; i++) {
        vector<chrom> off = selection(pop); // select the parents
        variation(off); // apply crossover and mutation
        evaluate(off, a); // evaluate the fitness of the offspring
        survival_elitist(pop, off); // select the survivors
        cout << i << ") Penalty = " << pop[0].fitness << " at t = " << pop[0].t << endl;
    }
    cout << "Optimal transmission time: " << pop[0].t << endl;

    return 0;
}
