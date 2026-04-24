#pragma once

#include <vector>
#include <string>

class Model;
struct Map;

enum PathfinderTypes;

class ViewModel
{
public:
	ViewModel(Model& model);

	const std::string& GetFilepath() const;
	const Map& GetMap() const;

	bool RunPathfinderEvent(const PathfinderTypes type, std::vector<int>& out_path);
	bool ChangeMapEvent(const std::string& filepath);

private:
	Model& m_model;
};