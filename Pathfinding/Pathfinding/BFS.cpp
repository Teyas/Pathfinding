#include "Pathfinding/BFS.h"
#include "Model.h"

namespace bfs
{
	bool FindPath(const Model& model, std::vector<int>& out_path)
	{
		if (model.IsValid() == false) return false;

		const Map& map = model.GetMap();
		const int start_index = map.GetIndexForCoords(model.GetStartNode());
		const int end_index = map.GetIndexForCoords(model.GetEndNode());

		std::vector<int> node_queue;
		node_queue.reserve(map.data.size());
		node_queue.push_back(start_index);

		std::vector<int> connection_lookup;
		connection_lookup.resize(map.data.size(), Map::invalid_node);

		for (int queue_head = 0u; queue_head < node_queue.size(); ++queue_head)
		{
			const int current_node_index = node_queue[queue_head];
			if (current_node_index == end_index)
			{
				// backtrack path
				unsigned parent_idx = connection_lookup[current_node_index];
				while (parent_idx != start_index)
				{
					out_path.push_back(parent_idx);
					parent_idx = connection_lookup[parent_idx];
				}
				std::reverse(out_path.begin(), out_path.end());
				return true;
			}
			else
			{
				auto add_neighbour = [&](const int neighbour_index) 
				{
					if (map.data[current_node_index] > 0.0f) return;

					// Has it been visited?
					if (connection_lookup[neighbour_index] == Map::invalid_node)
					{
						connection_lookup[neighbour_index] = current_node_index;
						node_queue.push_back(neighbour_index);
					}
				};

				const sf::Vector2i coords = map.GetCoordForIndex(current_node_index);
				if (coords.x > 0)
				{
					add_neighbour(current_node_index - 1);
				}
				if (coords.x + 1 < map.dimensions.x)
				{
					add_neighbour(current_node_index + 1);
				}
				if (coords.y > 0)
				{
					add_neighbour(current_node_index - map.dimensions.x);
				}
				if (coords.y + 1 < map.dimensions.y)
				{
					add_neighbour(current_node_index + map.dimensions.x);
				}
			}
		}

		return false;
	}
}