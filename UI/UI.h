#pragma once
#include "../Libs/json.hpp"
struct GLFWwindow;
struct ImFont;
class CWindow {
public:
	bool init();
	void RenderLoop();


private:
	int display_w, display_h;
	GLFWwindow* window;
	nlohmann::json currentNote;
	ImFont* smallFont;
	ImFont* bigFont;
	void drawWindow();
	void drawSidebar();
	void drawEditor();
};