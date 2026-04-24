#pragma once

#include <vector>

class Model;

namespace dfs
{
	static constexpr int invalid_index = -1;
	struct NodeInfo
	{
		bool is_queued = false;
		int parent_index = invalid_index;
	};

	bool FindPath(const Model& model, std::vector<int>& out_path);
}