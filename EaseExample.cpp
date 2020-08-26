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
}

void EaseExample::OnMouseButtonPressed(float x, float y)
{
	for(const auto& btn : m_easeButtons)
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
	for(const auto& btn : m_easeButtons)
	{
//		std::cout << btn.getPosition().x << " " << btn.getPosition().y << std::endl;
		window.draw(btn);
	}
}

void EaseExample::TweenExample1()
{
	std::vector<tweeny::tween_action> actions;
	actions.reserve(m_easeButtons.size());
	for(size_t idx = 0; idx < m_easeButtons.size(); idx++)
	{
		auto& btn = m_easeButtons[idx];

		auto delay = tweeny::delay(idx * 100ms);
		auto fadeIn = tweeny::fade_from_to(btn, 0, 255, 1s, m_sentinel, tweeny::ease::smooth_stop3);
		auto zoomIn = tweeny::scale_from_to(btn, sf::Vector2f(0.0f, 0.0f), {1.0f, 1.0f}, 1s, m_sentinel, tweeny::ease::smooth_stop3);
		auto showUp = tweeny::together(fadeIn, zoomIn);

		actions.emplace_back(tweeny::sequence(delay, showUp));
	}
	tweeny::start(tweeny::together(actions));
}

void EaseExample::TweenExample2()
{
	std::vector<tweeny::tween_action> actions;
	actions.reserve(m_easeButtons.size());
	for(size_t idx = 0; idx < m_easeButtons.size(); idx++)
	{
		auto& btn = m_easeButtons[idx];

		auto delay = tweeny::delay(idx * 100ms);

		auto moveLeftPos = btn.getPosition();
		moveLeftPos.x -= 250.0f;
		auto moveLeft = tweeny::move_to(btn, moveLeftPos, 500ms, m_sentinel, tweeny::ease::smooth_start2);

		sf::Vector2f moveDownPos = btn.getPosition();
		moveDownPos.x += idx * 100.0f;
		moveDownPos.y -= 500.0f;
		auto moveDown = tweeny::move_from_to(btn, moveDownPos, btn.getPosition(), 1s, m_sentinel, tweeny::ease::smooth_stop2);

		actions.emplace_back(tweeny::sequence(delay, moveLeft, delay, moveDown));
	}
	tweeny::start(tweeny::together(actions));
}

void EaseExample::TweenExample3()
{
	std::vector<tweeny::tween_action> actions;
	actions.reserve(m_easeButtons.size());
	for(size_t idx = 0; idx < m_easeButtons.size(); idx++)
	{
		auto& btn = m_easeButtons[idx];
		auto delay = tweeny::delay(idx * 100ms);
		auto shake = tweeny::shake(btn, sf::Vector2f(25, 0.0f), m_sentinel, 1500ms, 8);

		actions.emplace_back(tweeny::sequence(delay, shake));
	}
	tweeny::start(tweeny::together(actions));
}

void EaseExample::TweenExample4()
{
	std::vector<tweeny::tween_action> actions;
	actions.reserve(m_easeButtons.size());
	for(size_t idx = 0; idx < m_easeButtons.size(); idx++)
	{
		auto& btn = m_easeButtons[idx];
		auto delay = tweeny::delay(idx * 100ms);

		auto scaleDown = tweeny::scale_by(btn, sf::Vector2f(-0.2f, -0.2f), 250ms, m_sentinel);
		auto scaleUp = tweeny::scale_by(btn, sf::Vector2f(0.2f, 0.2f), 250ms, m_sentinel);
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
	btn.label.setCharacterSize(30);
	btn.label.setFillColor(sf::Color::White);

	btn.setFillColor(sf::Color::Green);
	btn.setSize({180, 40});
	btn.setOrigin(90, 20);
	btn.onClick = onClick;

	if(m_easeButtons.empty())
	{
		btn.setPosition({95, 25});
	}
	else
	{
		auto pos = m_easeButtons.back().getPosition();
		btn.setPosition(pos);
		btn.move(0.0f, 45.0f);
	}
	m_easeButtons.emplace_back(btn);
}
