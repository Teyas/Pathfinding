#pragma once

#include <vector>
#include <SFML/System/Vector2.hpp>

struct Map
{
	static constexpr int invalid_node = -1;

	sf::Vector2i dimensions;
	std::vector<float> data;

	void SetDimensions(const sf::Vector2i dims)
	{
		dimensions = { dims.x, dims.y };
		data.resize(dims.x * dims.y);
	}

	__forceinline int GetIndexForCoords(const int x, const int y) const
	{
		return (y * dimensions.x) + x;
	}

	__forceinline int GetIndexForCoords(const sf::Vector2i coord) const
	{
		return GetIndexForCoords(coord.x, coord.y);
	}

	__forceinline sf::Vector2i GetCoordForIndex(const int index) const
	{
		return { index % dimensions.x, index / dimensions.x };
	}
};
