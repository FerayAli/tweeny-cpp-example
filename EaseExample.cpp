#include "EaseExample.h"
#include "SFTweenSpecialization.h"
#include <tweeny/tweeny.h>
#include <iostream>

namespace
{
	sf::Font DefaultFont;
}

void EaseExample::Init()
{
	if(!DefaultFont.loadFromFile("../tweeny-cpp-example/assets/OpenSans-Regular.ttf"))
	{
		std::cout << "Could not load font" << std::endl;
	}

	CreateButton("Tween Example 1", [this](){ TweenExample1(); });
	CreateButton("Tween Example 2", [this](){ TweenExample2(); });
	CreateButton("Tween Example 3", [this](){ TweenExample3(); });
	CreateButton("Tween Example 4", [this](){ TweenExample4(); });
	CreateButton("Tween Example 5", [](){});

	auto createShape = [](const sf::Vector2f& pos)
	{
		sf::RectangleShape shape;
		shape.setFillColor(sf::Color(16, 52, 166));
		shape.setOutlineColor(sf::Color(126, 249, 255));
		shape.setOutlineThickness(2.0f);
		shape.setSize({90, 40});
		shape.setOrigin(45, 20);
		shape.setPosition(pos);
		return shape;
	};

	m_shapes.emplace_back(createShape({290, 30}));
	for(size_t i = 1; i < 15; i++)
	{
		auto pos = m_shapes.back().getPosition();
		auto hspacing = m_shapes.back().getGlobalBounds().width + 10.0f;
		pos.x += hspacing;
		m_shapes.emplace_back(createShape(pos));

		if(i % 5 == 0)
		{
			auto vspacing = m_shapes.back().getGlobalBounds().height + 10.0f;
			m_shapes.back().setPosition({m_shapes.front().getPosition().x, pos.y + vspacing});
		}
	}
}

void EaseExample::OnMouseButtonPressed(float x, float y)
{
	for(const auto& btn : m_tweenButtons)
	{
		if(btn.getGlobalBounds().contains(x, y))
		{
			btn.onClick();
			break;
		}
	}
}

void EaseExample::Draw(sf::RenderWindow& window)
{
	for(auto& btn : m_tweenButtons)
	{
		window.draw(btn);
		window.draw(btn.label);
	}

	for(const auto& shape : m_shapes)
	{
		window.draw(shape);
	}
}

void EaseExample::TweenExample1()
{
	std::vector<tweeny::tween_action> actions;
	actions.reserve(m_shapes.size());
	for(size_t idx = 0; idx < m_shapes.size(); idx++)
	{
		auto& shape = m_shapes[idx];

		auto delay = tweeny::delay(idx * 100ms);
		auto fadeIn = tweeny::fade_from_to(shape, 0, 255, 1s, m_sentinel, tweeny::ease::smooth_stop3);
		auto zoomIn = tweeny::scale_from_to(shape, sf::Vector2f(0.0f, 0.0f), {1.0f, 1.0f}, 1s, m_sentinel, tweeny::ease::smooth_stop3);
		auto showUp = tweeny::together(fadeIn, zoomIn);

		actions.emplace_back(tweeny::sequence(delay, showUp));
	}
	tweeny::start(tweeny::together(actions));
}

void EaseExample::TweenExample2()
{
	std::vector<tweeny::tween_action> actions;
	actions.reserve(m_shapes.size());
	for(size_t idx = 0; idx < m_shapes.size(); idx++)
	{
		auto& shape = m_shapes[idx];

		auto delay = tweeny::delay((idx+1) * 100ms);

		auto moveUpPos = shape.getPosition();
		moveUpPos.y -= 250.0f;
		auto moveUp = tweeny::move_to(shape, moveUpPos, 500ms, m_sentinel, tweeny::ease::smooth_start2);

		sf::Vector2f moveLeftPos = shape.getPosition();
		moveLeftPos.x += (idx+1) * 50.0f;
		auto moveLeft = tweeny::move_from_to(shape, moveLeftPos, shape.getPosition(), 1s, m_sentinel, tweeny::ease::smooth_stop3);

		actions.emplace_back(tweeny::sequence(delay, moveUp, delay, moveLeft));
	}
	tweeny::start(tweeny::together(actions));
}

void EaseExample::TweenExample3()
{
	std::vector<tweeny::tween_action> actions;
	actions.reserve(m_shapes.size());
	for(size_t idx = 0; idx < m_shapes.size(); idx++)
	{
		auto& shape = m_shapes[idx];
		auto delay = tweeny::delay(idx * 100ms);
		auto shake = tweeny::shake(shape, sf::Vector2f(25, 0.0f), m_sentinel, 1500ms, 8);

		actions.emplace_back(tweeny::sequence(delay, shake));
	}
	tweeny::start(tweeny::together(actions));
}

void EaseExample::TweenExample4()
{
	std::vector<tweeny::tween_action> actions;
	actions.reserve(m_shapes.size());
	for(size_t idx = 0; idx < m_shapes.size(); idx++)
	{
		auto& shape = m_shapes[idx];
		auto delay = tweeny::delay(idx * 100ms);

		auto scaleDown = tweeny::scale_by(shape, sf::Vector2f(-0.2f, -0.2f), 250ms, m_sentinel);
		auto scaleUp = tweeny::scale_by(shape, sf::Vector2f(0.2f, 0.2f), 250ms, m_sentinel);
		auto pulse = tweeny::sequence(scaleDown, scaleUp);

		actions.emplace_back(tweeny::sequence(delay, pulse));
	}
	tweeny::start(tweeny::together(actions));
}

void EaseExample::CreateButton(const std::string& text, const std::function<void()>& onClick)
{
	ButtonExample btn;
	btn.label.setFont(DefaultFont);
	btn.label.setString(text);
	btn.label.setCharacterSize(20);
	btn.label.setFillColor(sf::Color::White);

	btn.setFillColor(sf::Color(16, 52, 166));
	btn.setOutlineColor(sf::Color(126, 249, 255));
	btn.setOutlineThickness(2.0f);
	btn.setSize({180, 40});
	btn.setOrigin(90, 20);
	btn.onClick = onClick;

	if(m_tweenButtons.empty())
	{
		btn.setPosition({100, 30});
	}
	else
	{
		auto pos = m_tweenButtons.back().getPosition();
		btn.setPosition(pos);
		btn.move(0.0f, 50.0f);
	}
	btn.label.setOrigin(btn.label.getGlobalBounds().width / 2, btn.label.getGlobalBounds().height / 2);
	btn.label.setPosition(btn.getPosition());
	m_tweenButtons.emplace_back(btn);
}

