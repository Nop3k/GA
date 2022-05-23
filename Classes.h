#pragma once
#include <vector>

class Chromosome
{
private:
    std::string genes_;
    int length_;
    int fitness_score_ = 0;

public:
    Chromosome(const int length);
    Chromosome(const std::string genes = "");
    ~Chromosome();
    void CalculateFitness();
    std::string GetGenes() const;
    int GetFitness();
    void SetGenes(const std::string genes);
};

class Population
{
private:
    float fitness_avg_;
    std::vector<Chromosome> population_;
    const int population_size_;

public:
    Population(const int pop_size = 10);
    ~Population();
    void GenerateInitialPopulation();
    void UpdateFitness();
    void Selection();
    void Crossover();
    std::pair<Chromosome, Chromosome> OnePointCrossover(const Chromosome &parent1, const Chromosome &parent2);
    void Mutation();
    Chromosome OnePointMutation(const Chromosome &c);
    void Print();
    float GetFitnessAvg();
    std::string GetBest();
};
