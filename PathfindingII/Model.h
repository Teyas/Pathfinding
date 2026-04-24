#pragma once

#include <string>
#include <SFML/Graphics/Color.hpp>
#include "Map.h"//lol

class Model 
{
public:
	static constexpr sf::Color start_colour = { 0, 255, 0 };
	static constexpr sf::Color end_colour = { 0, 0, 255 };

	bool LoadMapData(const std::string& filepath);
	void ForceMapData(std::vector<float>&& data, const sf::Vector2i dimensions, const sf::Vector2i start, const sf::Vector2i end);
	void ResetData();
	bool IsValid() const;

	[[nodiscard]] const Map& GetMap() const { return m_map; }
	[[nodiscard]] sf::Vector2i GetStartNode() const { return m_start_node; }
	[[nodiscard]] sf::Vector2i GetEndNode() const { return m_end_node; }

	[[nodiscard]] const std::string& GetFilepath() const { return m_filepath; }

private:
	sf::Vector2i m_start_node;
	sf::Vector2i m_end_node;
	Map m_map;
	std::string m_filepath;
};