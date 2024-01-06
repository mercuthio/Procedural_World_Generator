#include "ImGuiManager.h"

ImGuiManager::~ImGuiManager()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    DestroyContext();
}

void ImGuiManager::Initialise(GLFWwindow* mainWindow)
{
    CreateContext();
    ImGuiIO& io = GetIO();
    (void)io;

    StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImGuiStyle& style = GetStyle();
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.FrameBorderSize = 0.000001f;
    style.WindowBorderSize = 0.000001f;

    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
    style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 1.0f, 1.0f, 0.3f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);

    style.Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
}

void ImGuiManager::StartWindow(const char* text, float width, float height)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    NewFrame();

    SetNextWindowPos(ImVec2(0, 0));
    SetNextWindowSize(ImVec2(width, height));
    SetNextWindowSizeConstraints(ImVec2(width, height), ImVec2(width, height));

    Begin(text, NULL, ImGuiWindowFlags_NoTitleBar);
}

void ImGuiManager::FinishWindow()
{
    End();
    Render();
}

void ImGuiManager::AddColorPicker(const char* buttonName, const char* popupName, ImVec4& color)
{
    ImVec2 buttonSize(28.5f, 20.0f);
    if (ImGui::ColorButton(buttonName, color, 0, buttonSize)) {
        ImGui::OpenPopup(popupName);
    }

    if (ImGui::BeginPopup(popupName)) {
        ImGui::ColorPicker4("Pick a Color", (float*)&color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB);

        ImGui::EndPopup();
    }
}

