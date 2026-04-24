#include "Utils.h"

#include <assert.h>

// Don't like including everything here but can't be bothered to do fancy stuff to avoid it for now
#include "Pathfinding/BFS.h"
#include "Pathfinding/A_Star.h"
#include "Pathfinding/DFS.h"
#include "Pathfinding/Dijkstra.h"

PathfindFunc GetFuncForType(const PathfinderTypes type) 
{
	switch (type)
	{
		case PathfinderTypes::A_STAR: return astar::FindPath;
		case PathfinderTypes::BFS: return bfs::FindPath;
		case PathfinderTypes::DFS: return dfs::FindPath;
		case PathfinderTypes::DIJKSTRA: return djk::FindPath;
		default: assert(false); break;
	}
	return nullptr;
}