#include "../include/randint.hpp"
#include <random>
int randint(const int a, const int b)
{
    const int range = std::abs(b - a) + 1;
    const int random = std::rand();
    const int return_value = (random % range) + a;
    return return_value;
}
