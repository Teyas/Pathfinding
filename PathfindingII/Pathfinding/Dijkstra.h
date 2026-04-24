#pragma once

#include <vector>

class Model;

namespace djk
{
	static constexpr int invalid_index = -1;
	static constexpr int invalid_cost = INT_MAX;

	struct QueueInfo 
	{
		int index = invalid_index;
		int cost = invalid_cost;
	};

	struct NodeInfo
	{
		int parent_index = invalid_index;
		int cost = invalid_cost;
	};

	bool FindPath(const Model& model, std::vector<int>& out_path);
}