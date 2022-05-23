#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "Classes.h"

const char CHARACTER_SET[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    ",.?! ";
const std::string TARGET = "To be, or not to be, that is the question.";
const int TARGET_LENGTH = TARGET.size();

Chromosome::Chromosome(const int length) : length_(length)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, sizeof(CHARACTER_SET) - 1);

    for (int i = 0; i < length_; ++i)
        genes_ += CHARACTER_SET[distribution(gen)];
    std::cout << genes_ << std::endl;
}
Chromosome::Chromosome(const std::string genes) : genes_(genes)
{
    length_ = TARGET_LENGTH;
}

Chromosome::~Chromosome()
{
}

void Chromosome::CalculateFitness()
{
    fitness_score_ = 0;
    for (int i = 0; i < TARGET_LENGTH; ++i)
        if (TARGET[i] == genes_[i])
            fitness_score_++;
}

std::string Chromosome::GetGenes() const { return genes_; }
int Chromosome::GetFitness() { return fitness_score_; };
void Chromosome::SetGenes(const std::string genes) { genes_ = genes; }

Population::Population(const int pop_size) : population_size_(pop_size)
{
}

Population::~Population()
{
    population_.clear();
}

void Population::GenerateInitialPopulation()
{
    population_.reserve(population_size_);
    for (int i = 0; i < population_size_; ++i)
    {
        Chromosome *chromosome = new Chromosome(TARGET_LENGTH);
        population_.push_back(*chromosome);
    }
}

void Population::UpdateFitness()
{
    for (Chromosome &c : population_)
        c.CalculateFitness();
}

void Population::Selection()
{
    std::sort(population_.begin(), population_.end(), [](Chromosome c1, Chromosome c2)
              { return c1.GetFitness() > c2.GetFitness(); });
    // Elitism - best 50% stay
    for (int i = 0; i < std::round(population_size_ / 2.0); ++i)
        population_.pop_back();
}

void Population::Crossover()
{
    auto end = population_.end() - 1;
    for (auto itr = population_.begin(); itr < end; itr += 2)
    {
        auto [child1, child2] = OnePointCrossover(*itr, *(itr + 1));
        population_.push_back(child1);
        population_.push_back(child2);
    }
}

std::pair<Chromosome, Chromosome> Population::OnePointCrossover(const Chromosome &parent1, const Chromosome &parent2)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(1, TARGET_LENGTH - 2);
    int crossover_point = distribution(gen);

    Chromosome child1, child2;
    child1.SetGenes(parent1.GetGenes().substr(0, crossover_point) + parent2.GetGenes().substr(crossover_point, TARGET_LENGTH - crossover_point));
    child2.SetGenes(parent2.GetGenes().substr(0, crossover_point) + parent1.GetGenes().substr(crossover_point, TARGET_LENGTH - crossover_point));

    return std::make_pair(child1, child2);
}
void Population::Mutation()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution_chromosome(0, population_.size());
    int random_chromosome;

    for (int i = population_.size(); i < population_size_; ++i)
    {
        random_chromosome = distribution_chromosome(gen);

        Chromosome mutated_chromosome = OnePointMutation(population_[random_chromosome]);
        population_.push_back(mutated_chromosome);
    }
}

Chromosome Population::OnePointMutation(const Chromosome &c)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> distribution_point(0, TARGET_LENGTH - 1);
    std::size_t mutation_point = distribution_point(gen);

    std::uniform_int_distribution<int> distribution_gene(0, sizeof(CHARACTER_SET) - 1);
    int mutated_gene = distribution_gene(gen);

    std::string mutated_genes = c.GetGenes();
    mutated_genes[mutation_point] = CHARACTER_SET[mutated_gene];
    Chromosome mutated(mutated_genes);

    return mutated;
}

void Population::Print()
{
    // for (Chromosome &c : population_)
    //     std::cout << c.GetGenes() << " - " << c.GetFitness() << '\n';
    std::cout << "Average fitness: " << fitness_avg_ << '\n';
    std::cout << "Size: " << population_.size() << std::endl;
}

float Population::GetFitnessAvg()
{
    fitness_avg_ = std::accumulate(population_.begin(), population_.end(), 0, [](int total, Chromosome c)
                                   { return total + c.GetFitness(); }) /
                   (float)population_.size();
    return fitness_avg_;
}

std::string Population::GetBest() { return population_.front().GetGenes(); }