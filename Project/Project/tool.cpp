#include "tool.h"

#include <iostream>
#include <string>
#include <iostream>
#include <chrono>
#include <random>

Graph::Graph(float width, float height) : width(width), height(height)
{
    background_rect.setSize(sf::Vector2f(width, height));
    background_rect.setOrigin(width / 2.0, height / 2.0);
    background_rect.setFillColor(background_color);

    x_axis.setFillColor(descr_color);
    x_axis.setSize(sf::Vector2f(width, 1));
    x_axis.setOrigin(width / 2.0, 1);

    y_axis.setFillColor(descr_color);
    y_axis.setSize(sf::Vector2f(1, height));
    y_axis.setOrigin(1, height/2.0);

    if (!font.loadFromFile("../arial.ttf"))
    {
        std::cout << "CAN NOT LOAD FONT" << std::endl;
    }
}

void Graph::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Text text;
    text.setCharacterSize(char_size);
    text.setFont(font);

    states.transform *= getTransform();
    target.draw(background_rect, states);

    target.draw(x_axis, states);
    sf::RectangleShape grid_rect = y_axis;
    grid_rect.setFillColor(grid_color);
    const float x_abs = width / 2.0;
    for (float x_pos = 0; x_pos <= x_abs; x_pos += grid_step)
    {
        grid_rect.setPosition(x_pos, 0);
        target.draw(grid_rect, states);

        grid_rect.setPosition(-x_pos, 0);
        target.draw(grid_rect, states);

        text.setFillColor(grid_color);
        text.setString(std::to_string(x_pos / x_scale));
        text.setPosition(x_pos, grid_step/5.0);
        const auto oldRotation = text.getRotation();
        text.rotate(90);
        target.draw(text, states);
        text.setRotation(oldRotation);
    }

    target.draw(y_axis, states);
    grid_rect = x_axis;
    grid_rect.setFillColor(grid_color);
    const float y_abs = height / 2.0;
    for (float y_pos = 0; y_pos <= y_abs; y_pos += grid_step)
    {
        grid_rect.setPosition(0, y_pos);
        target.draw(grid_rect, states);

        grid_rect.setPosition(0, -y_pos);
        target.draw(grid_rect, states);

        text.setFillColor(grid_color);
        text.setString(std::to_string(y_pos/y_scale));
        text.setPosition(grid_step / 2.0, -(y_pos + char_size + 5));
        target.draw(text, states);
    }

    int graphId = 0;
    for (auto [name, vertices] : data)
    {
        vertices.setPrimitiveType(sf::PrimitiveType::LinesStrip);
        target.draw(vertices, states);

        const sf::Vertex lastVertex = vertices[vertices.getVertexCount() - 1];
        sf::Color vertcolor = lastVertex.color;
        vertcolor.a = 64;
        text.setFillColor(vertcolor);
        std::string xposString = std::to_string(lastVertex.position.x/x_scale);
        xposString = std::string(xposString.begin(), xposString.begin() + xposString.find(".") + 3);
        std::string yposString = std::to_string(-lastVertex.position.y/y_scale);
        yposString = std::string(yposString.begin(), yposString.begin() + yposString.find(".") + 3);
        std::string lastVertexString = name;
        lastVertexString += " [" + xposString + ":" + yposString + "] ";
        text.setString(lastVertexString);
        text.setPosition(x_max*x_scale - text.getGlobalBounds().width, 60 - (y_max*y_scale + char_size*graphId));
        target.draw(text, states);

        ++graphId;
    }

    text.setFillColor(descr_color);
    text.setString(x_name);
    text.setPosition(width/2.0 - (text.getGlobalBounds().width + 5), -(char_size + 5));
    target.draw(text, states);

    text.setString(y_name);
    text.setPosition(5, height / -2.0);
    target.draw(text, states);
}

void Graph::update()
{
    y_scale = height / (y_max * 2.0);
    x_scale = width / (x_max * 2.0);
    grid_step = height / 20.0;
    char_size = grid_step * 0.5;
}

void Graph::addPoint(const std::string& name, float x, float y)
{
    const float y_to_pixels = y * y_scale;
    const float x_to_pixels = x * x_scale;

    sf::Vertex vertex({x_to_pixels, -y_to_pixels }, foreground_color);
    data[name].append(vertex);
}


Tester::Tester(int N, float ms, int step, int repet)
: window(sf::VideoMode(800, 600), "Result graph")
, graph(window.getSize().x*2, window.getSize().y*2)
, N(N)
, STEP(step)
, REPET(repet)
{
    std::unique_lock<std::mutex> lock(graphLock);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Result graph");
    graph.setPosition(0, window.getSize().y);
    graph.x_max = N;
    graph.x_name = "N";
    graph.y_max = ms;
    graph.y_name = "ms";
}


void Tester::addTest(const Test& test)
{
    tests.push_back(test);
}

void Tester::runTest(int testId)
{
    Test& test = tests[testId];
    for (int n = 0; n < N; n += STEP)
    {
        test.test_precondition(n);

        sf::Time avgDt;
        for (int t = 0; t < REPET; ++t)
        {
            sf::Time begin = clock.getElapsedTime();
            test.test_operation(n);
            sf::Time end = clock.restart();
            avgDt += end - begin;
        }

        test.test_postcondition(n);

        std::unique_lock<std::mutex> lock(graphLock);
        graph.foreground_color = test.test_color;
        graph.addPoint(test.test_name, n, avgDt.asMicroseconds() / REPET / 1000.0);
    }
}

void Tester::showResult()
{
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        {
            std::unique_lock<std::mutex> lock(graphLock);
            graph.update();
            window.draw(graph);
        }
        window.display();
    }
}

void Tester::start()
{
    std::vector<std::thread> threads;
    for (int testId = 0; testId < tests.size(); ++testId)
    {
        threads.emplace_back([&, id = testId]() {runTest(id); });
    }

    showResult();

    for (auto& thread : threads)
    {
        thread.join();
    }
}