#pragma once

#include <vector>
#include <set>

template<typename Key, typename Iterator>
const Iterator linear_search(const Iterator& cbegin, const Iterator& cend, const Key& key)
{
    for (Iterator i = cbegin; i != cend; ++i)
    {
        if (*i == key)
        {
            return i;
        }
    }

    return cend;
}

int fibonacci(int n);

int fibonacci_iterative(int n);

int gcd_iterative(int lhs, int rhs);

int gcd(int lhs, int rhs);

std::vector<int> findPrimeFactors(int number);

std::vector<int> findPrimeFactorsFast(int number);

std::set<int> findPrimeNumbersEratosthenesSet(int number);

std::vector<int> findPrimeNumbersEratosthenes(int number);

bool isPrime(int number);

std::vector<int> findPrimeNumbers(int number);

bool isPrimeFermat(int number);

std::vector<int> findPrimeNumbersFermat(int number);

// Linear congruential generator
int getRandom(int from, int to);

template<typename Iterator>
void shuffle(Iterator begin, Iterator end)
{
    const std::size_t size = end - begin;
    for (int idx = 0; idx < size; ++idx)
    {
        const std::size_t swap_idx = getRandom(idx, size);
        std::swap(*(begin + idx), *(begin + swap_idx));
    }
}