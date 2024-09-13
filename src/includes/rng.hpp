#pragma once
#include <random>


// Returns a random index based on weights
int RandomWeighedIndex(std::vector<float>& weights)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<int> dd(weights.begin(), weights.end());

    return dd(gen);
}
