#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <functional>
#include <memory>

struct ButtonExample : public sf::RectangleShape
{
	sf::Text label;
	std::function<void()> onClick;
};

class EaseExample
{
public:
	void Init();
	void OnMouseButtonPressed(float x, float y);
	void Draw(sf::RenderWindow& window);
private:
	void TweenExample1();
	void TweenExample2();
	void TweenExample3();
	void TweenExample4();
	void CreateButton(const std::string& text, const std::function<void()>& onClick);

	std::vector<ButtonExample> m_tweenButtons;
	std::vector<sf::RectangleShape> m_shapes;
	std::shared_ptr<int> m_sentinel = std::make_shared<int>();
};
