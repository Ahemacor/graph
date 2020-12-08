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
#include <unordered_set>
#include <set>

template<typename Iterator>
void check_sort(Iterator begin, Iterator end)
{
    for (Iterator prev = begin, it = begin; it != end; ++it)
    {
        auto current = it;
        if (*current < *prev)
        {
            std::cout << "NOT SORTED!" << std::endl;
            break;
        }
        prev = current;
    }
}

template<typename Iterator> Iterator find_min(Iterator begin, Iterator end)
{
    Iterator min_it = begin;
    for (Iterator it = begin; it != end; ++it)
    {
        if (*it < *min_it) min_it = it;
    }
    return min_it;
}

template<typename Iterator> void selection_sort(Iterator begin, Iterator end)
{
    for (Iterator it = begin; it != end; ++it)
    {
        Iterator min = find_min(it, end);
        std::swap(*min, *it);
    }
}

template<typename Iterator> Iterator find_greater_equal(Iterator value, Iterator begin, Iterator end)
{
    Iterator greater = end;
    for (Iterator it = begin; it != end; ++it)
    {
        if (*it >= *value)
        {
            greater = it;
            break;
        }
    }
    return greater;
}

void insertion_sort(std::vector<int>& vector)
{
    std::vector<int> sorted;
    sorted.reserve(vector.size());
    for(auto begin = vector.begin(), end = vector.end(); begin != end; ++begin)
    {
        auto placeToInsert = find_greater_equal(begin, sorted.begin(), sorted.end());
        sorted.insert(placeToInsert, *begin);
    }
    vector = sorted;
}

void buble_sort_0(std::vector<int>& vector)
{
    const int numOfElements = vector.size();
    bool sorted = false;
    while (!sorted)
    {
        sorted = true;
        for (int index = 0; index < (numOfElements - 1); ++index)
        {
            if (vector[index] > vector[index + 1])
            {
                std::swap(vector[index], vector[index + 1]);
                sorted = false;
            }
        }
    }
}

void buble_sort_1(std::vector<int>& vector)
{
    const int numOfElements = vector.size();
    bool sorted = false;
    int prev_last_swap = numOfElements;
    int last_swap = numOfElements;
    bool order = true;
    while (!sorted)
    {
        sorted = true;
        for (int index = 0; index < (numOfElements - 1); ++index)
        {
            if (vector[index] > vector[index + 1])
            {
                std::swap(vector[index], vector[index + 1]);
                sorted = false;
                last_swap = index + 1;
            }

            if (index > prev_last_swap)
            {
                break;
            }
        }

        prev_last_swap = last_swap;
    }
}

int main()
{
    const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    const int N = 5'000;
    const float ms_max = 500;
    const int step = 1;
    const int repet = 1;

    auto initVector = [seed](std::vector<int>& vector, int n) {
        vector.resize(n);
        std::iota(vector.begin(), vector.end(), 0);
        std::shuffle(vector.begin(), vector.end(), std::default_random_engine(seed));
    };

    std::vector<int> test_vector;
    initVector(test_vector, 10);
    buble_sort_1(test_vector);
    check_sort(test_vector.begin(), test_vector.end());

    Tester tester(N, ms_max, step, repet);

    std::vector<int> random_0(N);
    std::iota(random_0.begin(), random_0.end(), 0);
    std::shuffle(random_0.begin(), random_0.end(), std::default_random_engine(seed));
    {
        Tester::Test test;
        test.test_precondition = [&](int n) { initVector(random_0, n); };
        test.test_operation = [&](int n) { std::sort(random_0.begin(), random_0.end()); };
        test.test_postcondition = [&](int n) { check_sort(random_0.begin(), random_0.end()); };
        test.test_color = sf::Color::Black;
        test.test_name = "std::sort";
        tester.addTest(test);
    }


    std::vector<int> random_1(N);
    std::iota(random_1.begin(), random_1.end(), 0);
    std::shuffle(random_1.begin(), random_1.end(), std::default_random_engine(seed));
    {
        Tester::Test test;
        test.test_precondition = [&](int n) { initVector(random_1, n); };
        test.test_operation = [&](int n) { selection_sort(random_1.begin(), random_1.end()); };
        test.test_postcondition = [&](int n) { check_sort(random_1.begin(), random_1.end()); };
        test.test_color = sf::Color::Green;
        test.test_name = "selection sort";
        tester.addTest(test);
    }

    std::vector<int> random_2(N);
    std::iota(random_2.begin(), random_2.end(), 0);
    std::shuffle(random_2.begin(), random_2.end(), std::default_random_engine(seed));
    {
        Tester::Test test;
        test.test_precondition = [&](int n) { initVector(random_2, n); };
        test.test_operation = [&](int n) { insertion_sort(random_2); };
        test.test_postcondition = [&](int n) { check_sort(random_2.begin(), random_2.end()); };
        test.test_color = sf::Color::Red;
        test.test_name = "insertion sort";
        tester.addTest(test);
    }

    std::vector<int> random_3(N);
    std::iota(random_3.begin(), random_3.end(), 0);
    std::shuffle(random_3.begin(), random_3.end(), std::default_random_engine(seed));
    {
        Tester::Test test;
        test.test_precondition = [&](int n) { initVector(random_3, n); };
        test.test_operation = [&](int n) { buble_sort_0(random_3); };
        test.test_postcondition = [&](int n) { check_sort(random_3.begin(), random_3.end()); };
        test.test_color = sf::Color::Blue;
        test.test_name = "buble_sort_0";
        tester.addTest(test);
    }

    std::vector<int> random_4(N);
    std::iota(random_4.begin(), random_4.end(), 0);
    std::shuffle(random_4.begin(), random_4.end(), std::default_random_engine(seed));
    {
        Tester::Test test;
        test.test_precondition = [&](int n) { initVector(random_4, n); };
        test.test_operation = [&](int n) { buble_sort_1(random_4); };
        test.test_postcondition = [&](int n) { check_sort(random_4.begin(), random_4.end()); };
        test.test_color = sf::Color::Yellow;
        test.test_name = "buble_sort_1";
        tester.addTest(test);
    }

    tester.start();

}