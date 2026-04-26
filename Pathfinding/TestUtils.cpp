#include "TestUtils.h"
#include "Model.h"
#include "Utils.h"

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <filesystem>

void LoadAllMapFilepaths()
{
	for (const auto& map_file : std::filesystem::recursive_directory_iterator("Assets\\Maps"))
	{
		if (map_file.path().extension() == ".png")
		{
			map_list.emplace_back(map_file.path().string());
		}
	}
}

void RunTest(const PathfinderTypes type, const int num_of_runs, const std::string& map_file, TestResults& results)
{
	Model model;

	for (int i = 0; i < num_of_runs; ++i)
	{
		const bool did_load = model.LoadMapData(map_file);
		assert(did_load);

		QueryResult& result = results.queries.emplace_back();
		result.type = type;
		result.file = map_file;
		
		if (PathfindFunc func = GetFuncForType(type)) 
		{
			sf::Clock timer;
			result.did_find_path = func(model, result.out_path);
			result.calc_time = timer.getElapsedTime();
		}
	}
}

void RunTest(const PathfinderTypes type, const int num_of_runs, const std::vector<std::string>& files, TestResults& results)
{
	for (const auto& map_file : map_list)
	{
		RunTest(type, num_of_runs, map_file, results);
	}
}