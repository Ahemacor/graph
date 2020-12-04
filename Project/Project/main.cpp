#include "algorithm.h"

#include "tool.h"

#include <SFML/System.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <functional>
#include <thread>
#include <mutex>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <climits>

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

int main()
{
    const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    const int N = 500000;
    const float ms_max = 30;
    const int step = 100;
    const int repet = 1;

    std::vector<int> random(N);
    std::iota(random.begin(), random.end(), 0);
    std::shuffle(random.begin(), random.end(), std::default_random_engine(seed));

    Tester tester(N, ms_max, step, repet);

    for (const auto& num : findPrimeFactorsFast(31525))
    {
        std::cout << num << std::endl;
    }


    {
        Tester::Test test;
        test.test_operation = [&](int n) { for (int i = 0; i < 99; ++i) findPrimeFactorsFast(n+i); };
        test.test_color = sf::Color::Red;
        test.test_name = "prime factors fast";
        tester.addTest(test);
    }

    {
        Tester::Test test;
        test.test_operation = [&](int n) { for (int i = 0; i < 99; ++i) findPrimeFactors(n+i); };
        test.test_color = sf::Color::Blue;
        test.test_name = "prime factors slow";
        tester.addTest(test);
    }


    /*
    {
        Tester::Test test;
        test.test_operation = [&](int n) {for (int i = 0; i < n; ++i) gcd_iterative(random[i] % (n+1), random[(n - 1) - i] % (n+1)); };
        test.test_color = sf::Color::Blue;
        test.test_name = "gcd iterative";
        tester.addTest(test);
    }

    {
        Tester::Test test;
        test.test_operation = [&](int n) {for (int i = 0; i < n; ++i) gcd(random[i] % (n + 1), random[(n - 1) - i] % (n + 1)); };
        test.test_color = sf::Color::Green;
        test.test_name = "gcd recursive";
        tester.addTest(test);
    }
    */

    tester.start();

}