#include "tool.h"

#include <iostream>
#include <string>
#include <iostream>
#include <chrono>
#include <random>
#include <numeric>

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
        text.setPosition(5, -(y_pos + char_size + 5));
        target.draw(text, states);
    }

    int graphId = 0;
    for (auto [name, vertices] : data)
    {
        sf::VertexArray scaledVertices;
        for (int vertex_id = 0; vertex_id < vertices.getVertexCount(); ++vertex_id)
        {
            scaledVertices.append(vertices[vertex_id]);
            scaledVertices[vertex_id].position.x *= x_scale;
            scaledVertices[vertex_id].position.y *= y_scale;
        }
        scaledVertices.setPrimitiveType(sf::PrimitiveType::LinesStrip);
        target.draw(scaledVertices, states);

        const sf::Vertex lastVertex = scaledVertices[scaledVertices.getVertexCount() - 1];
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
        text.setPosition(x_max*x_scale - text.getGlobalBounds().width, -(height/2.0 - char_size*graphId));
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

    std::sort(data.begin(), data.end(), [](auto& lhs, auto& rhs) {
        return (lhs.second[lhs.second.getVertexCount() - 1].position.x) < (rhs.second[rhs.second.getVertexCount() - 1].position.x);
    });
}

void Graph::addPoint(const std::string& name, float x, float y)
{
    sf::Vertex vertex({ x, -y}, foreground_color);
    int found_idx = -1;
    for (int i = 0; i < data.size(); ++i)
    {
        if (data[i].first == name)
        {
            found_idx = i;
            break;
        }
    }

    if (found_idx < 0)
    {
        found_idx = data.size();
        data.push_back({ name, {} });
    }

    data[found_idx].second.append(vertex);
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
    graph.update();
}


void Tester::addTest(const Test& test)
{
    tests.push_back(test);
}

void Tester::runTest(int testId)
{
    Test& test = tests[testId];

    std::vector<int> testResults(REPET, 0);
    for (int n = 0; n < N; n += STEP)
    {

        for (int t = 0; t < REPET; ++t)
        {
            test.test_precondition(n);

            sf::Time begin = test.clock.getElapsedTime();
            test.test_operation(n);
            sf::Time end = test.clock.restart();
            testResults[t] = ((end - begin).asMicroseconds());

            test.test_postcondition(n);
        }
        std::sort(testResults.begin(), testResults.end());
        const float medianTime_ms = testResults[REPET / 2]/1000.0f;

        std::unique_lock<std::mutex> lock(graphLock);
        graph.foreground_color = test.test_color;
        graph.addPoint(test.test_name, n, medianTime_ms);
    }
}

void Tester::testFunction(std::function<double(double)> test_function, const std::string& name, sf::Color color)
{
    for (int n = 0; n < N; n += STEP)
    {
        std::unique_lock<std::mutex> lock(graphLock);

        graph.foreground_color = color;
        graph.addPoint(name, n, test_function(n));
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
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseWheelScrolled)
            {
                const float bias = 0.1;
                if (event.mouseWheelScroll.delta > bias)
                {
                    std::unique_lock<std::mutex> lock(graphLock);
                    graph.y_max *= 1.1;
                    graph.x_max *= 1.1;
                }
                else if(event.mouseWheelScroll.delta < bias)
                {
                    std::unique_lock<std::mutex> lock(graphLock);
                    graph.y_max *= 0.9;
                    graph.x_max *= 0.9;
                }
            }
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