#pragma once
#include <SFML/Graphics/Transformable.hpp>
#include <tweeny/tween_common.h>

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
}
