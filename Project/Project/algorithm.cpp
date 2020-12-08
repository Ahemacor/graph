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

std::vector<int> findPrimeFactors(int number)
{
    std::vector<int> factors;
    for (int d = 2; d < number; ++d)
    {
        while (number % d == 0)
        {
            factors.push_back(d);
            number /= d;
        }
    }

    if (number > 1) factors.push_back(number);

    return factors;
}

std::vector<int> findPrimeFactorsFast(int number)
{
    std::vector<int> factors;

    if (number > 1)
    {
        while (number % 2 == 0)
        {
            factors.push_back(2);
            number /= 2;
        }

        for (int d = 3, limit = std::sqrt(number); d < limit; d += 2)
        {
            while (number % d == 0)
            {
                factors.push_back(d);
                number /= d;
                limit = std::sqrt(number);
            }
        }

        if (number > 1) factors.push_back(number);
    }
    return factors;
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

std::set<int> findPrimeNumbersEratosthenesSet(int number)
{
    std::set<int> primes;
    for (int i = 2; i < number; ++i) primes.insert(primes.end(), i);

    const int sq_number = std::sqrt(number);
    for (auto primes_it = primes.begin(); primes_it != primes.end() && *primes_it <= sq_number; ++primes_it)
    {
        auto it = primes_it;
        while (++it != primes.end())
        {
            if (*it % *primes_it == 0)
            {
                it = primes.erase(it);

                if (it == primes.end()) break;
            }
        }
    }
    return primes;
}

std::vector<int> findPrimeNumbersEratosthenes(int number)
{
    std::vector<bool> numbers(number, true);
    if (number > 1) numbers[0] = numbers[1] = false;

    const int sq_number = std::ceil(std::sqrt(number));
    int prime_counter = 0;
    std::vector<int> primes(number);
    for (int n = 2; n <= sq_number; ++n)
    {
        if (numbers[n] == true)
        {
            const int divider = n;
            for (int candidate = divider + divider; candidate < number; candidate += divider)
            {
                numbers[candidate] = false;
            }
        }
    }

    for (int i = 0; i < numbers.size(); ++i)
    {
        if (numbers[i]) primes[prime_counter++] = i;
    }
    primes.resize(prime_counter);

    return primes;
}

bool isPrime(int number)
{
    if (number == 2 || number == 3) return true;

    if (number % 2 == 0) return false;

    for (int d = 3; d * d <= number; d += 2)
    {
        if (number % d == 0)  return false;
    }
    return true;
}

std::vector<int> findPrimeNumbers(int number)
{
    std::vector<int> primes;
    primes.reserve(number);
    for (int i = 2; i < number; ++i)
    {
        if (isPrime(i))
        {
            primes.push_back(i);
        }
    }
    return primes;
}

bool isPrimeFermat(int number)
{
    const std::uint32_t numChecks = 3;
    if (number < 2) return false;
    else if (number == 2) return true;

    for (int check = 2; check < numChecks; ++check)
    {
        const std::uint64_t n = check;
        const std::uint64_t p = number;
        const std::uint64_t n_pow_p_minus_one = std::powl(n, p - 1);
        if (n_pow_p_minus_one % p != 1) return false;
    }
    return true;
}

std::vector<int> findPrimeNumbersFermat(int number)
{
    std::vector<int> primes;
    primes.reserve(number);
    for (int i = 2; i < number; ++i)
    {
        if (isPrimeFermat(i))
        {
            primes.push_back(i);
        }
    }
    return primes;
}