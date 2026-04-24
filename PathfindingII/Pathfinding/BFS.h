#pragma once

#include <vector>

class Model;

namespace bfs
{
	bool FindPath(const Model& model, std::vector<int>& out_path);
}