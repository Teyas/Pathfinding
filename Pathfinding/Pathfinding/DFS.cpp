#include "Pathfinding/DFS.h"
#include "Model.h"

#include <stack>

namespace dfs
{
	bool FindPath(const Model& model, std::vector<int>& out_path)
	{
		if (model.IsValid() == false) return false;

		const Map& map = model.GetMap();
		const int start_index = map.GetIndexForCoords(model.GetStartNode());
		const int end_index = map.GetIndexForCoords(model.GetEndNode());

		std::vector<NodeInfo> node_infos(map.data.size());
		std::stack<int> node_queue;
		
		auto add_node = [&node_infos, &node_queue, &map](const int node_index, const int parent_index)
		{
			if (node_infos[node_index].is_queued) return;
			if (map.data[node_index] > 0) return;
			if (node_infos[node_index].parent_index != -1) __debugbreak();

			node_infos[node_index].is_queued = true;
			node_infos[node_index].parent_index = parent_index;
			node_queue.push(node_index);
		};

		add_node(start_index, start_index);

		while (node_queue.empty() == false) 
		{
			const int current_node_index = node_queue.top();
			node_queue.pop();		

			if (current_node_index == end_index)
			{
				// backtrack path
				int parent_idx = node_infos[current_node_index].parent_index;
				while (parent_idx != start_index)
				{
					out_path.push_back(parent_idx);
					parent_idx = node_infos[parent_idx].parent_index;
				}
				std::reverse(out_path.begin(), out_path.end());
				return true;
			}

			const sf::Vector2i coords = map.GetCoordForIndex(current_node_index);
			if (coords.x > 0)
			{
				add_node(current_node_index - 1, current_node_index);
			}
			if (coords.x + 1 < map.dimensions.x)
			{
				add_node(current_node_index + 1, current_node_index);
			}
			if (coords.y > 0)
			{
				add_node(current_node_index - map.dimensions.x, current_node_index);
			}
			if (coords.y + 1 < map.dimensions.y)
			{
				add_node(current_node_index + map.dimensions.x, current_node_index);
			}
		}

		return false;
	}
}