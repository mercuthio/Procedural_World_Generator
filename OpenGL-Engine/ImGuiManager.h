#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using namespace ImGui;

class ImGuiManager
{
public:
	ImGuiManager() {}
	~ImGuiManager();

	void Initialise(GLFWwindow* mainWindow);
	void StartWindow(const char* text, float width, float height);

	void AddText(const char* text) { Text(text); }
	void AddSliderInt(const char* text, int* value, int start, int end) { SetNextItemWidth(100); SliderInt(text, value, start, end); }
	void AddSliderFloat(const char* text, float* value, float start, float end) { SetNextItemWidth(100); SliderFloat(text, value, start, end); }
	void AddColorPicker(const char* buttonName, const char* popupName, ImVec4& color);
	void AddCheckbox(const char* text, bool* value) { Checkbox(text, value); }
	bool AddButton(const char* text) { return Button(text); }
	void AddTextField(const char* text, char* content) { InputText(text, content, sizeof(content), 20); }
	void AddSameLine() { SameLine(); }
	void AddSeparator() { Separator(); }

	void FinishWindow();
	void RenderWindow() { ImGui_ImplOpenGL3_RenderDrawData(GetDrawData()); }

private:
};

