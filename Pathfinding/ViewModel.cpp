#include "ViewModel.h"

#include "Model.h"
#include "Utils.h"

ViewModel::ViewModel(Model& model)
	: m_model(model)
{
}

bool ViewModel::RunPathfinderCommand(const PathfinderTypes type, std::vector<int>& out_path) const
{
	if (PathfindFunc func = GetFuncForType(type))
	{
		func(m_model, out_path);
		return true;
	}

	return false;
}

bool ViewModel::ChangeMapCommand(const std::string& filepath) const
{
	return m_model.LoadMapData(filepath);
}

const std::string& ViewModel::GetFilepath() const
{
	return m_model.GetFilepath();
}

const Map& ViewModel::GetMap() const
{
	return m_model.GetMap();
}