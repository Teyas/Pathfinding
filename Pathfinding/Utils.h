#pragma once

#include <string>
#include <vector>

enum PathfinderTypes
{
	A_STAR,
	BFS,
	DFS,
	DIJKSTRA,

	COUNT
};

class Model;

using PathfindFunc = bool (*)(const Model& model, std::vector<int>& out_path);
PathfindFunc GetFuncForType(const PathfinderTypes type);