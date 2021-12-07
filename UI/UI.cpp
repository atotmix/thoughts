#include "UI.h"
#include "../NoteManager/NoteManager.h"
#include "../Libs/imgui-1.85/imgui.h"
#include "../Libs/imgui-1.85/imgui_internal.h"
#include "../Libs/imgui-1.85/Backends/imgui_impl_glfw.h"
#include "../Libs/imgui-1.85/Backends/imgui_impl_opengl3.h"
#include "../Libs/imgui-1.85/misc/freetype/imgui_freetype.h"
#include "../Libs/imgui-1.85/misc/cpp/imgui_stdlib.h"
#include <stdio.h>
#include <string>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include "../Libs/glfw/include/GLFW/glfw3.h"
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

bool CWindow::init() {
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    window = glfwCreateWindow(1280, 720, "Thoughts", NULL, NULL);
    if (window == NULL)
        return false;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImFontConfig config;
    config.RasterizerMultiply = 1.1f;
    //config.FontBuilderFlags = ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_ForceAutoHint; crashes :/
    smallFont = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Lato-Regular.ttf", 18.0f, &config);
    bigFont = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Lato-Bold.ttf", 48.0f, &config);
    ImGuiFreeType::BuildFontAtlas(io.Fonts);
    return true;
    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);
}
void CWindow::RenderLoop() {
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        
        glfwGetFramebufferSize(window, &display_w, &display_h);
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        //if (show_demo_window)
        

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        drawWindow();

        //ImGui::ShowDemoWindow(&show_demo_window);

        // Rendering
        ImGui::Render();

        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0490196347, 0.0490196347, 0.0490196347, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    CNoteMgr::Get().SaveToDisk();
}
std::string selected = "NA";

char titlebuf[46];
std::string editorBuf;
void CWindow::drawSidebar() {
    ImGui::SetNextWindowSize({ 256.f , (float)display_h - 32 });
    ImGui::SetNextWindowPos({ 16.f,16.f });
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.09f, 0.09f, 0.09f, 0.94f));
    ImGui::Begin("Sidebar", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);                                // Create a window called "Hello, world!" and append into it.
    ImGui::BeginGroup();
    if (ImGui::Button("Add", { 74.f,54.f })) {
        CNoteMgr::Get().AddNote(CNote());
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete", { 74.f,54.f }) && selected != "NA") {
        CNoteMgr::Get().DelNote(selected);
        selected = "NA";
    }
    ImGui::SameLine();
    ImGui::Button("Search", { 74.f,54.f });
    ImGui::EndGroup(); 
    ImGui::Separator();
   // ImGui::GetWindowDrawList()->AddLine({ 17.f,87.f }, { 266.f ,87.f}, ImColor(255, 255, 255,120), 1);
    ImGui::GetWindowDrawList()->AddLine({ 271.f,15.f }, { 271.f ,(float)display_h - 17 }, ImColor(255, 255, 255, 120), 1);
    ImGui::SetCursorPos({ 15.f, 80.f });
    {
        
        ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false);
        for (auto &[title, id, modified, iModified] : *CNoteMgr::Get().GetNotes()) {
            char buf[128];
            if (title.length() > 27)
                title = title.substr(0, 26) + "...";
            
            sprintf_s(buf, "%s\n%s##%s", title.c_str(), modified.c_str(),id.c_str());
            
            ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, {0.05,0.0});
            if (ImGui::Selectable(buf, selected == id, 0, ImVec2(0, 35))) {
                if (selected != "NA")
                    CNoteMgr::Get().UpdateNote(CNote(currentNote, selected));
               
                selected = id;
                currentNote = CNoteMgr::Get().GetNote(selected).dump();

                sprintf_s(titlebuf, "%s", currentNote["title"].get<std::string>().c_str());
                editorBuf = currentNote["content"].get<std::string>();
                
            }
            ImGui::PopStyleVar();
            ImGui::Separator();
        }
            
        ImGui::EndChild();
    }
    ImGui::End();
    ImGui::PopStyleColor();
}



void CWindow::drawEditor() {
    ImGui::SetNextWindowSize({ (float)display_w - 287 , (float)display_h - 32 });
    ImGui::SetNextWindowPos({ 271.f,16.f });
    ImGui::Begin("Editor", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);    

   
    if (selected == "NA") {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Welcome to thoughts, its really not done but feedback would be highly appreciated <3");
        ImGui::End();
        return;
        
    }
    ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.f,0.f,0.f,0.f });  ImGui::PushFont(bigFont); ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputText("##bruh", titlebuf, 46);
     ImGui::PopFont(); ImGui::PopItemWidth();
    currentNote["title"] = std::string(titlebuf);
    ImGui::Separator();
    if (editorBuf.empty())
        editorBuf.push_back(0);
    ImGui::InputTextMultiline("##MyStr", &editorBuf, ImVec2(-FLT_MIN, ImGui::GetContentRegionAvail().y));
    currentNote["content"] = editorBuf;
    ImGui::PopStyleColor();
    ImGui::End();
    
}
void CWindow::drawWindow() {

    drawSidebar();
    drawEditor();
}
