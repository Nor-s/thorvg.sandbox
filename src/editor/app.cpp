#include "app.h"
#include "window/glWindow.h"
#include "imgui/imguiCanvas.h"
#include "imgui/imguiContentBrowser.h"

#include <SDL.h>

#include <tvgGl.h>
#include <thorvg.h>
#include <tvgCommon.h>
#include <core/core.h>
#include <ImGuiNotify.hpp>

#include "examples/examples.h"

App& App::GetInstance()
{
	static App instance;
	return instance;
}

void App::InitInstance(const AppState& state)
{
	tvg::Initializer::init(0);

	GetInstance().mState = state;
	GetInstance().init();

	// tvgGl.h
	glInit();
	extraGlInit();
}

void App::DestroyInstance()
{
	tvg::Initializer::term();
}

void App::MainLoop()
{
	App& app = GetInstance();
	while (app.mState.running)
	{
		app.loop();
	}
}

void App::init()
{
	mWindow = std::make_unique<editor::GLWindow>(mState.resolution);
	mWindow->show();
}

void App::loop()
{
	SDL_Event event;
	ImGuiIO& io = ImGui::GetIO();
	(void) io;
	core::io::deltaTime = io.DeltaTime;

	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch (event.type)
		{
			case SDL_QUIT:
				mState.running = false;
				break;
			default:
				break;
		}
	}
	if (SDL_GetWindowFlags(mWindow->mWindow) & SDL_WINDOW_MINIMIZED)
	{
		SDL_Delay(10);
		return;
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, (int) io.DisplaySize.x, (int) io.DisplaySize.y);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
				 clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto& canvas : mCanvasList)
	{
		canvas->onUpdate();
	}

	draw();
	drawgui();
	drawend();
}

void App::draw()
{
	// temp code
	static bool bIsInit = false;
	if (!bIsInit)
	{
		float clearColor[3] = {.1f, 0.2f, 0.2f};

		mCanvasList.push_back(new tvgexam::ExampleCanvas(mWindow->mContext, {500.0f, 500.0f}, false));
		mCanvasList.push_back(new tvgexam::ExampleCanvas(mWindow->mContext, {500.0f, 500.0f}, true));
		for (auto& canvas : mCanvasList)
		{
			canvas->clearColor(clearColor);
			canvas->onInit();
		}
		bIsInit = true;
	}
	for (auto& canvas : mCanvasList)
	{
		canvas->draw();
	}
}

void App::drawgui()
{
	static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

	{
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
					   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace", NULL, windowFlags);

		ImGui::PopStyleVar(3);

		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
		}
		ImGui::End();
	}

	// temp code
	int i = 0;
	for (auto* canvas : mCanvasList)
	{
		std::string title = "canvas" + std::to_string(i++);
		title += canvas->isSw() ? "(sw)" : "(gl)";
		editor::ImGuiCanvasView().onDraw(title.c_str(), *canvas);
	}
	editor::ImGuiCanvasView().onDrawSceneInspect();
	editor::ImGuiCanvasView().onDrawContentBrowser();
	editor::ImguiContentBrowser().draw();
}

void App::drawend()
{
	// Notifications style setup
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);		 // Disable round borders
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);	 // Disable borders
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.10f, 1.00f));	 // Background color
	ImGui::RenderNotifications();
	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(1);

	// ImGUI Rendering
	ImGui::Render();
	SDL_GL_MakeCurrent(mWindow->mWindow, mWindow->mContext);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	mWindow->refresh();
}
