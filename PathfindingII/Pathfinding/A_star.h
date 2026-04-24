#pragma once

#include <vector>

class Model;

namespace astar
{
	static constexpr int invalid_index = -1;
	static constexpr int invalid_cost = -1;
	struct OpenNodeInfo
	{
		int index = invalid_index;

		int f = 0;
		int g = 0;
		int h = 0;
	};

	struct StoredNodeInfo
	{
		bool is_closed = false;
		int parent_index = invalid_index;
		int total_cost = -1;
	};

	bool FindPath(const Model& model, std::vector<int>& out_path);
}