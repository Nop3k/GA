#include <iostream>
#include "Classes.h"

int main()
{

    Population pop(1000);
    pop.GenerateInitialPopulation();
    pop.UpdateFitness();
    float fitness, fitness_last;
    int generation = 1;
    do
    {
        fitness_last = pop.GetFitnessAvg();
        pop.Selection();
        pop.Crossover();
        pop.Mutation();
        pop.UpdateFitness();
        fitness = pop.GetFitnessAvg();

        std::cout << "Generation " << generation << " - ";
        std::cout << "Last fitness: " << fitness_last << ' ';
        std::cout << "New fitness: " << fitness << '\t';
        std::cout << fitness - fitness_last << std::endl;
        generation++;
    } while ((fitness - fitness_last) > 0.01);

    pop.Print();
    std::cout << "\nTARGET:\n";
    std::cout << "To be, or not to be, that is the question.\n";
    std::cout << "\nBEST:\n";
    std::cout << pop.GetBest() << std::endl;
}