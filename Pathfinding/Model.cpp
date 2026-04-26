#include "Model.h"

#include <SFML/Graphics/Image.hpp>

bool Model::LoadMapData(const std::string& filepath)
{
	ResetData();

	m_filepath = filepath;

	sf::Image map_image;
	const bool did_load = map_image.loadFromFile(filepath);
	if (did_load == false) 
	{
		return false;
	}

	const sf::Vector2i map_size = { static_cast<int>(map_image.getSize().x), static_cast<int>(map_image.getSize().y) };
	m_map.SetDimensions(map_size);

	// Loop through every pixel from top left to bottom right
	// If it's start or end colour, weight of 0 and set as start/end
	// If not, get R value as set that as value
	for (int x = 0; x < map_size.x; ++x)
	{
		for (int y = 0; y < map_size.y; ++y)
		{
			const int data_index = m_map.GetIndexForCoords(x, y);
			const sf::Color pixel_colour = map_image.getPixel({ static_cast<unsigned>(x), static_cast<unsigned>(y) });
			if (pixel_colour == start_colour)
			{
				m_map.data[data_index] = 0.0f;
				m_start_node = { x, y };
			}
			else if (pixel_colour == end_colour)
			{
				m_map.data[data_index] = 0.0f;
				m_end_node = { x, y };
			}
			else
			{
				m_map.data[data_index] = pixel_colour.r;
			}
		}
	}

	return true;
}

void Model::ForceMapData(std::vector<float>&& data, const sf::Vector2i dimensions, const sf::Vector2i start, const sf::Vector2i end)
{
	m_map.data = std::move(data);
	m_map.SetDimensions(dimensions);
	m_start_node = start;
	m_end_node = end;
}

void Model::ResetData()
{
	m_map.data.clear();
	m_map.dimensions = {};

	m_start_node = {};
	m_end_node = {};
	m_filepath = {};
}

bool Model::IsValid() const
{
	auto is_valid_node = [&](const sf::Vector2i& node) -> bool { return node.x >= 0 && node.x < m_map.dimensions.x && node.y >= 0 && node.y < m_map.dimensions.y; };

	const bool has_map_data = m_map.data.empty() == false;
	const bool is_start_valid = is_valid_node(m_start_node);
	const bool is_end_valid = is_valid_node(m_end_node);

	return has_map_data && is_start_valid && is_end_valid;
}