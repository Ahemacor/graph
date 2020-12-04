#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <mutex>
#include <functional>
#include <vector>

class Graph : public sf::Transformable, public sf::Drawable
{
public:
    Graph(float width, float height);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void update();
    void addPoint(const std::string& name, float x, float y);

    float width = 400;
    float height = 400;

    float x_max = 10;
    float y_max = 1.1;

    float y_scale = height / (y_max * 2.0);
    float x_scale = width / (x_max * 2.0);
    float grid_step = height / 20.0;
    float char_size = grid_step * 0.5;

    std::string x_name = "x";
    std::string y_name = "y";

    sf::Color foreground_color = sf::Color::Blue;
    sf::Color background_color = sf::Color::White;
    sf::Color descr_color = sf::Color::Black;
    sf::Color grid_color = sf::Color(0, 0, 0, 32);

private:
    sf::RectangleShape background_rect;

    sf::RectangleShape x_axis;
    sf::RectangleShape y_axis;

    std::unordered_map<std::string, sf::VertexArray> data;

    sf::Font font;
};

class Tester
{
public:
    Tester(int N, float ms, int step = 1, int repet = 1);

    struct Test
    {
        Test() = default;
        Test(const Test&) = default;
        std::function<void(int)> test_precondition = [](int) {};
        std::function<void(int)> test_operation = [](int) {};
        std::function<void(int)> test_postcondition = [](int) {};
        sf::Color test_color = sf::Color::Black;
        std::string test_name = "function";
        sf::Clock clock;
    };

    void addTest(const Test& test);
    void runTest(int testId);
    void testFunction(std::function<double(double)> test_function, const std::string& name = "function", sf::Color color = sf::Color::Black);
    void showResult();
    void start();

private:
    sf::RenderWindow window;
    Graph graph;

    const std::size_t N;
    const std::size_t STEP;
    const std::size_t REPET;

    std::vector<Test> tests;

    std::mutex graphLock;
};
