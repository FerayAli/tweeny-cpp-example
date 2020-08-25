#include <SFML/Graphics.hpp>
#include <tweeny/tweeny.h>
#include <thread>

namespace tweeny
{

template<>
struct tween_access<sf::CircleShape>
{
    static void set_position(sf::CircleShape& object, const sf::Vector2f& pos)
    {
        object.setPosition(pos);
    }

    static sf::Vector2f get_position(const sf::CircleShape& object)
    {
        return object.getPosition();
    }
};

}

void DrawEaseFunction(sf::RenderWindow& window, sf::Vector2f pos, tweeny::ease_t ease_func)
{
    std::vector<float> easeValue;
    for(float i = 0.0f; i < 1.0f; i += 0.05f)
    {
        easeValue.emplace_back(ease_func(i));
    }
    auto v1 = easeValue.front();
    for(size_t i = 1; i < easeValue.size(); i++)
    {
        auto v2 = easeValue[i];
        sf::VertexArray line(sf::LineStrip, 2);
        line[0].position = sf::Vector2f(i * 10.0f, v2 * 250.0f);
        line[1].position = sf::Vector2f((i - 1) * 10.0f, v1 * 250.0f);

        line[0].position += pos;
        line[1].position += pos;

        v1 = v2;

        window.draw(line);
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    sf::CircleShape circle;
    circle.setRadius(30.0f);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(300, 300);

    auto sentinel = std::make_shared<int>();

    auto action1 = tweeny::delay(0s);
    auto action2 = tweeny::move_by(circle, sf::Vector2f(100, 0), 250ms, sentinel, tweeny::ease::smooth_stop3);
    auto action3 = tweeny::move_by(circle, sf::Vector2f(0, 100), 250ms, sentinel, tweeny::ease::smooth_stop3);
    auto action4 = tweeny::move_by(circle, sf::Vector2f(-100, 0), 250ms, sentinel, tweeny::ease::smooth_stop3);
    auto action5 = tweeny::move_by(circle, sf::Vector2f(0, -100), 250ms, sentinel, tweeny::ease::smooth_stop3);

    auto action6 = tweeny::sequence(action2, action3, action4, action5);

    tweeny::start(tweeny::repeat(action6));



    while (window.isOpen())
    {
        std::this_thread::sleep_for(16ms);
        tweeny::detail::update(16ms);

        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        window.clear();

        window.draw(circle);
        DrawEaseFunction(window, {50, 50}, tweeny::ease::smooth_start2);


        window.display();
    }

    return 0;
}
