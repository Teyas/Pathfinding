#pragma once

#include <vector>
#include <string>

class Model;
struct Map;

enum PathfinderTypes : int;

class ViewModel
{
public:
	ViewModel(Model& model);

	[[nodiscard]] const std::string& GetFilepath() const;
	[[nodiscard]] const Map& GetMap() const;

	[[nodiscard]] bool RunPathfinderCommand(const PathfinderTypes type, std::vector<int>& out_path) const;
	[[nodiscard]] bool ChangeMapCommand(const std::string& filepath) const;

private:
	Model& m_model;
};