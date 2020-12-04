#include "algorithm.h"
#include <cmath>
#include <cstdint>
#include <chrono>

int fibonacci(int n)
{
    return (n > 1) ? fibonacci(n - 1) + fibonacci(n - 2) : 1;
}

int fibonacci_iterative(int n)
{
    if (n == 1) return 0;
    else if (n == 2) return 1;

    int p_prev = 0;
    int prev = 1;
    int result = 0;

    for (int i = 3; i <= n; ++i)
    {
        result = p_prev + prev;
        p_prev = prev;
        prev = result;
    }
    return result;
}

int gcd_iterative(int lhs, int rhs)
{
    while (rhs)
    {
        int remainder = lhs % rhs;
        lhs = rhs;
        rhs = remainder;
    }
    return lhs;
}

int gcd(int lhs, int rhs)
{
    return (rhs) ? gcd(rhs, lhs % rhs) : lhs;
}

// Linear congruential generator
int getRandom(int from, int to)
{
    static std::uint64_t X = std::chrono::system_clock::now().time_since_epoch().count();
    const int multiplier = 1664525;
    const int increment = 1013904223;
    const std::uint64_t modulus = std::powl(2, 32);
    X = (multiplier * X + increment) % modulus;
    return from + (X / (double)modulus) * (to - from);
}