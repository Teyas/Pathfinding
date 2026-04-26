#include "Model.h"
#include "View.h"

#include "TestUtils.h"

int main()
{
	LoadAllMapFilepaths();

	static constexpr const char* filepath = "Assets/Maps/map_c.png";
	Model model;
	model.LoadMapData(filepath);

	ViewModel view_model(model);
	View view(view_model);
	view.RefreshVisuals(filepath);

	while (view.IsOpen())
	{
		view.Input();
		view.Update();
		view.Display();
	}
	view.Destroy();

	// add random image generation
	// add graphs and tests etc. to imgui

	//TestResults results;
	//RunTest(PathfinderTypes::DFS, 10, map_list, results);

	return 0;
}