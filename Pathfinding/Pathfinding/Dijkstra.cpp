#include "Pathfinding/Dijkstra.h"
#include "Model.h"

#include <queue>

namespace
{
    auto queue_compare_func = [](const djk::QueueInfo& lhs, const djk::QueueInfo& rhs) { return lhs.cost > rhs.cost; };
}

namespace djk
{
    bool FindPath(const Model& model, std::vector<int>& out_path)
    {
        if (model.IsValid() == false) return false;

        const Map& map = model.GetMap();
        const int map_size = static_cast<int>(map.data.size());
        const int start_idx = map.GetIndexForCoords(model.GetStartNode());
        const int end_idx = map.GetIndexForCoords(model.GetEndNode());

        std::vector<NodeInfo> node_info_list;
        node_info_list.resize(map_size);

        std::priority_queue<QueueInfo, std::vector<QueueInfo>, decltype(queue_compare_func)> node_queue;

        node_info_list[start_idx].cost = 0;
        node_queue.push(QueueInfo{.index = start_idx, .cost = 0});

        while (node_queue.empty() == false)
        {
            // Find index with lowest cost
            const int current_node_idx = node_queue.top().index;
            const int current_cost = node_queue.top().cost;
            node_queue.pop();

            if (current_cost > node_info_list[current_node_idx].cost)
            {
                continue;
            }

            if (current_node_idx == end_idx)
            {
                // backtrack path
                int parent_idx = node_info_list[current_node_idx].parent_index;
                while (parent_idx != start_idx)
                {
                    out_path.push_back(parent_idx);
                    parent_idx = node_info_list[parent_idx].parent_index;
                }
                std::reverse(out_path.begin(), out_path.end());
                return true;
            }
            else
            {
                auto add_neighbour = [&](const int neighbour_idx)
                {
                    if (map.data[neighbour_idx] > 0.0f) return;

                    static constexpr int node_weight = 1;
                    const int neighbour_cost = node_info_list[current_node_idx].cost + node_weight;
                    if (neighbour_cost < node_info_list[neighbour_idx].cost)
                    {
                        node_info_list[neighbour_idx].cost = neighbour_cost;
                        node_info_list[neighbour_idx].parent_index = current_node_idx;
                        node_queue.push(QueueInfo{.index = neighbour_idx, .cost = neighbour_cost});
                    }
                };

                const sf::Vector2i coords = map.GetCoordForIndex(current_node_idx);
                if (coords.x > 0)
                {
                    add_neighbour(current_node_idx - 1);
                }
                if (coords.x + 1 < map.dimensions.x)
                {
                    add_neighbour(current_node_idx + 1);
                }
                if (coords.y > 0)
                {
                    add_neighbour(current_node_idx - map.dimensions.x);
                }
                if (coords.y + 1 < map.dimensions.y)
                {
                    add_neighbour(current_node_idx + map.dimensions.x);
                }
            }
        }

        return false;
    }
}
