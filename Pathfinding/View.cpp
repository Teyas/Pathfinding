#include "View.h"

#include "External/imgui/imgui.h"
#include "External/imgui/imgui-SFML.h"

#include "Map.h"
#include "TestUtils.h"
#include "Utils.h"

View::View(ViewModel& view_model) 
	: m_view_model(view_model)
	, m_window()
	, m_imgui_window()
	, m_image_path()
	, m_map_image()
	, m_map_texture()
	, m_map_sprite(m_map_texture)
	, m_delta_clock()
	, m_old_cursor_pos()
{
	m_image_path.reserve(512);

	Init();
}

void View::Init()
{
	InitWindows();
}

void View::Destroy() const
{
	ImGui::SFML::Shutdown();
}

void View::Input()
{
	ProcessMainWindowInput();
	ProcessImGuiWindowInput();
}

void View::ProcessMainWindowInput()
{
	while (const std::optional event = m_window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			m_window.close();
			return;
		}
		else if (const auto* wheel_event = event->getIf<sf::Event::MouseWheelScrolled>())
		{
			const bool is_up = wheel_event->delta > 0;
			m_map_sprite.scale(is_up ? sf::Vector2f{ 1.1f, 1.1f } : sf::Vector2f{ 0.9f, 0.9f });
		}
		else if (event->getIf<sf::Event::MouseMoved>())
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				static constexpr float modifier = 1.0f;
				const sf::Vector2i new_cursor_pos = sf::Mouse::getPosition();
				const sf::Vector2i delta = new_cursor_pos - m_old_cursor_pos;
				m_map_sprite.move({ static_cast<float>(delta.x) * modifier, static_cast<float>(delta.y) * modifier });
			}
		}
	}

	m_old_cursor_pos = sf::Mouse::getPosition();
}

void View::ProcessImGuiWindowInput()
{
	while (const std::optional imgui_event = m_imgui_window.pollEvent())
	{
		ImGui::SFML::ProcessEvent(m_imgui_window, *imgui_event);
	}
	ImGui::SFML::Update(m_imgui_window, m_delta_clock.restart());
}

void View::Update()
{
	AddImGui();
}

void View::Display()
{
	m_window.clear({ 140, 140, 140 });
	m_window.draw(m_map_sprite);
	m_window.display();

	m_imgui_window.clear();
	ImGui::SFML::Render(m_imgui_window);
	m_imgui_window.display();
}

void View::InitWindows() 
{
	m_window = { sf::VideoMode({ 1000, 1000 }), "Pathfinding" };
	m_imgui_window = { sf::VideoMode({ 800, 600 }), "Pathfinding - Debug" };

	m_imgui_window.setPosition({ 100, 200 });
	m_window.setPosition({ 1000, 200 });

	const bool did_imgui_initialise = ImGui::SFML::Init(m_imgui_window);
	assert(did_imgui_initialise);
}

void View::RefreshVisuals(const std::string& filepath)
{
	const auto old_scale = m_map_sprite.getScale();
	const auto old_position = m_map_sprite.getPosition();

	m_image_path = filepath;
	m_map_image = sf::Image{ m_image_path };
	m_map_texture = sf::Texture{ m_map_image };
	m_map_sprite = sf::Sprite{ m_map_texture };

	m_map_sprite.setScale(old_scale);
	m_map_sprite.setPosition(old_position);
}

void View::AddImGui()
{
	auto reset_data_for_test = [&](TestResults& results) -> bool
	{
		results.Reset();
		return m_map_image.loadFromFile(m_view_model.GetFilepath());
	};

	auto update_map_image = [&]() -> void
	{
		const bool did_reload = m_map_texture.loadFromImage(m_map_image);
		assert(did_reload);

		const auto old_scale = m_map_sprite.getScale();
		const auto old_position = m_map_sprite.getPosition();

		m_map_sprite = sf::Sprite{ m_map_texture };
		m_map_sprite.setScale(old_scale);
		m_map_sprite.setPosition(old_position);
	};

	if (ImGui::Begin("Editor"))
	{
		static int current_type_index = 0;
		const char* pathfinding_items[PathfinderTypes::COUNT] = { "A*", "BFS", "DFS", "Dijkstra's" };
		const char* combo_type = pathfinding_items[current_type_index];

		constexpr ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("TabBar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Map Data"))
			{
				ImGui::InputText("Filepath", m_image_path.data(), m_image_path.capacity());

				static bool did_load_correctly = true;
				if (did_load_correctly == false)
				{
					ImGui::Text("Bad path, failed to load image");
				}

				if (ImGui::Button("Reload Image"))
				{
					did_load_correctly = m_view_model.ChangeMapCommand(m_image_path);
					if (did_load_correctly)
					{
						RefreshVisuals(m_image_path);
					}
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Individual Pathfind"))
			{
				if (ImGui::BeginCombo("Type", combo_type))
				{
					for (int i = 0; i < IM_ARRAYSIZE(pathfinding_items); i++)
					{
						const bool is_selected = (current_type_index == i);
						if (ImGui::Selectable(pathfinding_items[i], is_selected))
						{
							current_type_index = i;
						}

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				static TestResults individual_results = {};
				if (ImGui::Button("Find Path"))
				{
					reset_data_for_test(individual_results);
					RunTest(static_cast<PathfinderTypes>(current_type_index), 1, m_image_path, individual_results);

					const QueryResult& query_result = individual_results.queries[0];
					const Map& map = m_view_model.GetMap();
					for (const auto& index : query_result.out_path)
					{
						const sf::Vector2u pixel_coords = { static_cast<unsigned>(map.GetCoordForIndex(index).x), static_cast<unsigned>(map.GetCoordForIndex(index).y) };
						m_map_image.setPixel(pixel_coords, pathfinder_colours[current_type_index]);
					}
					update_map_image();
				}
				
				ImGui::Separator();
				for (const auto& query : individual_results.queries)
				{
					ImGui::Text("Type - %s", pathfinder_strings[query.type].data());
					ImGui::Text("Number of moves - %u", static_cast<unsigned>(query.out_path.size()));
					ImGui::Text("Time Taken - %fs", query.calc_time.asSeconds());
					ImGui::Separator();
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Group Pathfind"))
			{
				static TestResults group_results = {};
				if (ImGui::Button("Find Path For All"))
				{
					reset_data_for_test(group_results);
					const Map& map = m_view_model.GetMap();
					for (int i = 0; i < PathfinderTypes::COUNT; ++i)
					{
						RunTest(static_cast<PathfinderTypes>(i), 1, m_image_path, group_results);

						const QueryResult& query_result = group_results.queries.back();
						for (const auto& index : query_result.out_path)
						{
							const sf::Vector2u pixel_coords = { static_cast<unsigned>(map.GetCoordForIndex(index).x), static_cast<unsigned>(map.GetCoordForIndex(index).y) };
							m_map_image.setPixel(pixel_coords, pathfinder_colours[i]);
						}
					}
					update_map_image();
				}
				
				ImGui::Separator();
				for (const auto& query : group_results.queries)
				{
					ImGui::Text("Type - %s", pathfinder_strings[query.type].data());
					ImGui::Text("Number of moves - %u", static_cast<unsigned>(query.out_path.size()));
					ImGui::Text("Time Taken - %fs", query.calc_time.asSeconds());
					ImGui::Separator();
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		ImGui::End();
	}
}