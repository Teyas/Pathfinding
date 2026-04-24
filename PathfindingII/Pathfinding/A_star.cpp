#include "Pathfinding/A_star.h"
#include "Model.h"

#include <queue>

namespace astar
{
	auto queue_compare_func = [](const OpenNodeInfo& lhs, const OpenNodeInfo& rhs) { return lhs.f > rhs.f; };
	using OpenNodeQueue = std::priority_queue<OpenNodeInfo, std::vector<OpenNodeInfo>, decltype(queue_compare_func)>;

	bool FindPath(const Model& model, std::vector<int>& out_path)
	{
		if (model.IsValid() == false) return false;

		const Map& map = model.GetMap();
		const sf::Vector2i& end = model.GetEndNode();
		const int start_index = map.GetIndexForCoords(model.GetStartNode());
		const int end_index = map.GetIndexForCoords(end);

		OpenNodeQueue open_list;

		std::vector<StoredNodeInfo> node_info_list;
		node_info_list.resize(map.data.size());

		OpenNodeInfo start_node = {};
		start_node.index = start_index;
		open_list.push(start_node);

		while (open_list.empty() == false)
		{
			const OpenNodeInfo current_node = open_list.top();
			open_list.pop();
			node_info_list[current_node.index].is_closed = true;

			if (current_node.index == end_index)
			{
				// backtrack path
				unsigned parent_idx = node_info_list[current_node.index].parent_index;
				while (parent_idx != start_index)
				{
					out_path.push_back(parent_idx);
					parent_idx = node_info_list[parent_idx].parent_index;
				}
				std::reverse(out_path.begin(), out_path.end());
				return true;
			}
			else
			{
				auto add_neighbour = [&](const int current_x, const int current_y, const int neighbour_index)
				{
					if (node_info_list[neighbour_index].is_closed) return;
					if (map.data[neighbour_index] > 0.0f) return;

					const int neighbour_g = current_node.g + 1;
					if (node_info_list[neighbour_index].total_cost == invalid_cost || neighbour_g < node_info_list[neighbour_index].total_cost)
					{
						OpenNodeInfo neighbour_info = {};
						neighbour_info.index = neighbour_index;
						neighbour_info.g = neighbour_g;
						neighbour_info.h = std::abs(current_x - end.x) + std::abs(current_y - end.y);
						neighbour_info.f = neighbour_info.g + neighbour_info.h;

						node_info_list[neighbour_index].total_cost = neighbour_info.g;
						node_info_list[neighbour_index].parent_index = current_node.index;

						open_list.push(neighbour_info);
					}
				};

				const sf::Vector2i coords = map.GetCoordForIndex(current_node.index);
				if (coords.x > 0)
				{
					add_neighbour(coords.x - 1, coords.y, current_node.index - 1);
				}
				if (coords.x + 1 < map.dimensions.x)
				{
					add_neighbour(coords.x + 1, coords.y, current_node.index + 1);
				}
				if (coords.y > 0)
				{
					add_neighbour(coords.x, coords.y - 1, current_node.index - map.dimensions.x);
				}
				if (coords.y + 1 < map.dimensions.y)
				{
					add_neighbour(coords.x, coords.y + 1, current_node.index + map.dimensions.x);
				}
			}
		}

		return false;
	}
}