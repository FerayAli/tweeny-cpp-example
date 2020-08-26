#include <SFML/Graphics.hpp>
#include <tweeny/tweeny.h>
#include <thread>

namespace tweeny
{
template<typename T>
struct tween_access<T, std::enable_if_t<std::is_base_of<sf::Transformable, T>::value>>
{
	static void set_position(T& object, const sf::Vector2f& pos)
    {
        object.setPosition(pos);
    }

	static sf::Vector2f get_position(const T& object)
    {
        return object.getPosition();
    }

	static void set_scale(T& object, const sf::Vector2f& scale)
	{
		object.setScale(scale);
	}

	static sf::Vector2f get_scale(const T& object)
	{
		return object.getScale();
	}

	static void set_rotation(T& object, const float& rotation)
	{
		object.setRotation(rotation);
	}

	static float get_rotation(const T& object)
	{
		return object.getRotation();
	}

	static void set_opacity(T& object, const int32_t& opacity)
	{
		auto c = object.getFillColor();
		c.a = opacity;
		object.setFillColor(c);
	}

	static int32_t get_opacity(const T& object)
	{
		return object.getFillColor().a;
	}
};
} //end of namespace tweeny

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

	sf::RectangleShape shape;

	shape.setSize({30.0f, 30.0f});
	shape.setFillColor(sf::Color::Green);
	shape.setPosition(300, 300);

    auto sentinel = std::make_shared<int>();

	const auto stepDuration = 500ms;
	auto moveByAction = [&]()
	{
		auto action1 = tweeny::move_by(shape, sf::Vector2f(100, 0), stepDuration, sentinel, tweeny::ease::smooth_stop3);
		auto action2 = tweeny::move_by(shape, sf::Vector2f(0, 100), stepDuration, sentinel, tweeny::ease::smooth_stop3);
		auto action3 = tweeny::move_by(shape, sf::Vector2f(-100, 0), stepDuration, sentinel, tweeny::ease::smooth_stop3);
		auto action4 = tweeny::move_by(shape, sf::Vector2f(0, -100), stepDuration, sentinel, tweeny::ease::smooth_stop3);
		return tweeny::sequence(action1, action2, action3, action4);
	}();

	auto scaleByAction = [&]()
	{
		auto action1 = tweeny::scale_by(shape, sf::Vector2f(0.5, 0.5), stepDuration, sentinel, tweeny::ease::smooth_stop3);
		auto action2 = tweeny::scale_by(shape, sf::Vector2f(-0.5, -0.5), stepDuration, sentinel, tweeny::ease::smooth_stop3);
		auto action3 = tweeny::scale_by(shape, sf::Vector2f(1, 1), stepDuration, sentinel, tweeny::ease::smooth_stop3);
		auto action4 = tweeny::scale_by(shape, sf::Vector2f(-1, -1), stepDuration, sentinel, tweeny::ease::smooth_stop3);
		return tweeny::sequence(action1, action2, action3, action4);
	}();

	auto rotateByAction = [&]()
	{
		auto action = tweeny::rotate_by(shape, 90.0f, stepDuration, sentinel, tweeny::ease::smooth_stop3);
		return tweeny::repeat(action, 4);
	}();

	auto fadeAction = [&]()
	{
		auto action1 = tweeny::fade_from_to(shape, 0, 255, stepDuration, sentinel, tweeny::ease::smooth_stop3);
		auto action2 = tweeny::fade_to(shape, 0, stepDuration, sentinel, tweeny::ease::smooth_stop3);
		auto action3 = tweeny::fade_by(shape, 255, stepDuration, sentinel, tweeny::ease::smooth_stop3);
		auto action4 = tweeny::delay(stepDuration);

		return tweeny::sequence(action1, action2, action3, action4);
	}();

	auto togetherAction = tweeny::together(moveByAction, scaleByAction, rotateByAction, fadeAction);
	auto repeatAction = tweeny::repeat(togetherAction);

	tweeny::start(repeatAction);

	auto speedControlAction = [&]()
	{
		auto tweenId = repeatAction.get_id();

		auto speederAction = tweeny::delay(stepDuration);
		speederAction.on_end = [tweenId]()
		{
			auto speed = tweeny::get_speed_multiplier(tweenId);
			tweeny::set_speed_multiplier(tweenId, speed + 0.2f);
		};

		auto slowerAction = tweeny::delay(stepDuration);
		slowerAction.on_end = [tweenId]()
		{
			auto speed = tweeny::get_speed_multiplier(tweenId);
			tweeny::set_speed_multiplier(tweenId, speed - 0.2f);
		};

		return tweeny::sequence(tweeny::repeat(speederAction, 5), tweeny::repeat(slowerAction, 5));
	}();
	tweeny::start(tweeny::repeat(speedControlAction));

    while (window.isOpen())
    {
        std::this_thread::sleep_for(16ms);
        tweeny::detail::update(16ms);

        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
			if(event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        window.clear();

		window.draw(shape);
        DrawEaseFunction(window, {50, 50}, tweeny::ease::smooth_start2);


        window.display();
    }

    return 0;
}
