#pragma once

#include <SFML/Graphics.hpp>
#include "ViewModel.h"

class View
{
public:
	View(ViewModel& view_model);

	void Init();
	void Destroy();

	void Input();
	void Update();
	void Display();

	void RefreshVisuals(const std::string& filepath);

	bool IsOpen() const { return m_window.isOpen(); }
private:
	void InitWindows();

	void ProcessMainWindowInput();
	void ProcessImGuiWindowInput();

	void AddImGui();

private:
	ViewModel& m_view_model;

	sf::RenderWindow m_window;
	sf::RenderWindow m_imgui_window;

	std::string m_image_path;
	sf::Image m_map_image;
	sf::Texture m_map_texture;
	sf::Sprite m_map_sprite;

	sf::Clock m_delta_clock;
	sf::Vector2i m_old_cursor_pos;
};