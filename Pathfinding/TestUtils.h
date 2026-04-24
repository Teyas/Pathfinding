#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <string>
#include <vector>

class Model;
enum PathfinderTypes;

static inline std::vector<std::string> map_list = {};

static const std::vector<std::string> pathfinder_strings =
{
	"A_STAR",
	"BFS",
	"DFS",
	"DIJKSTRA",
	"COUNT"
};

static const std::vector<sf::Color> pathfinder_colours =
{
	sf::Color{ 255,   0, 255, 255 },
	sf::Color{ 255, 255,   0, 255 },
	sf::Color{   0, 255, 255, 255 },
	sf::Color{ 255, 255, 255, 255 },
	sf::Color{   0,   0,   0,   0 }
};

struct QueryResult
{
	PathfinderTypes type;
	bool did_find_path = false;
	sf::Time calc_time;
	std::string file;
	std::vector<int> out_path;
};

struct TestResults
{
	std::vector<QueryResult> queries;

	void Reset() { queries.clear(); }
};


void LoadAllMapFilepaths();
void RunTest(PathfinderTypes type, int num_of_runs, const std::string& map_file, TestResults& results);
void RunTest(PathfinderTypes type, int num_of_runs, const std::vector<std::string>& files, TestResults& results);