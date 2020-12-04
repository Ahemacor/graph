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


int main()
{
    const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    const int N = 1000;
    const float ms_max = 5;
    const int step = 10;
    const int repet = 10;

    std::vector<int> random(N);
    std::iota(random.begin(), random.end(), 0);
    std::shuffle(random.begin(), random.end(), std::default_random_engine(seed));

    Tester tester(N, ms_max, step, repet);

    std::map<int, int> map;
    {
        Tester::Test test;
        test.test_precondition = [&](int n)
        {
            if (map.size() > 0)
            {
                map.clear();
            }

            for (int i = 0; i < n; ++i)
            {
                map[i] = i;
            }
        };

        test.test_operation = [&](int n)
        {
            if (map.size() > 0)
            {
                for(const auto& r : random) map.find(r);
            }
        };

        test.test_color = sf::Color::Blue;
        test.test_name = "map";
        tester.addTest(test);
    }

    std::unordered_map<int, int> u_map;
    {
        Tester::Test test;
        test.test_precondition = [&](int n)
        {
            if (u_map.size() > 0)
            {
                u_map.clear();
            }

            for (int i = 0; i < n; ++i)
            {
                u_map[i] = i;
            }
        };

        test.test_operation = [&](int n)
        {
            if (u_map.size() > 0)
            {
                for(const auto& r : random) u_map.find(r);
            }
        };

        test.test_color = sf::Color::Red;
        test.test_name = "u_map";
        tester.addTest(test);
    }

    std::vector<int> vector(N);
    std::iota(vector.begin(), vector.end(), 0);
    {
        Tester::Test test;
        test.test_precondition = [&](int n)
        {
            vector.resize(n);
        };

        test.test_operation = [&](int n)
        {
            if (vector.size() > 0)
            {
                for (const auto& r : random) linear_search(vector.cbegin(), vector.cend(), r);
            }
        };

        test.test_color = sf::Color::Green;
        test.test_name = "vector";
        tester.addTest(test);
    }


    tester.start();
}