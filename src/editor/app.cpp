#include "app.h"
#include "window/glWindow.h"
#include "imgui/imguiCanvas.h"
#include "imgui/imguiShapePanel.h"
#include "imgui/imguiContentBrowser.h"

#include <core/core.h>
#include <ImGuiNotify.hpp>

#include "examples/examples.h"

#include "event/eventStack.h"
#include "event/events.h"

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
	mEventController = std::make_unique<editor::EventStack>();
	mWindow = std::make_unique<editor::GLWindow>(mState.resolution);

	// tvgGl.h
	glInit();
	extraGlInit();


	// mCanvasList.push_back(new tvgexam::ExampleCanvas(mWindow->mContext, {500.0f, 500.0f}, false));
	// mCanvasList.push_back(new tvgexam::ExampleCanvas(mWindow->mContext, {500.0f, 500.0f}, true));
	mCanvasList.push_back(new core::AnimationCreatorCanvas(mWindow->mContext, {500.0f, 500.0f}, false));
	float clearColor[3] = {.77f, 0.77f, 0.77f};
	for (auto& canvas : mCanvasList)
	{
		canvas->clearColor(clearColor);
		canvas->onInit();
	}
	focusCanvas(0);
	mWindow->show();
}

void App::loop()
{
	if(!processEvent())
	{
		return;
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImGuiIO& io = ImGui::GetIO();
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
			ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags | ImGuiDockNodeFlags_PassthruCentralNode);
		}
		ImGui::End();
	}

	
	editor::ImGuiCanvasView().onDrawSceneInspect();
	editor::ImGuiCanvasView().onDrawContentBrowser();
	editor::ImguiContentBrowser().draw();
	for(int i =0 ; i < mCanvasList.size(); i++)
	{
		auto& canvas = mCanvasList[i];
		std::string title = "canvas" + std::to_string(i);
		title += canvas->isSw() ? "(sw)" : "(gl)";
		editor::ImGuiCanvasView().onDraw(title.c_str(), *canvas, i);
	}
}

void App::drawend()
{
	// Notifications style setup
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);							 // Disable round borders
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);						 // Disable borders
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
